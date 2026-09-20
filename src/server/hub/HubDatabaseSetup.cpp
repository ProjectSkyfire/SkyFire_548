/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifdef _WIN32
#include <winsock2.h>
#endif
#include <mysql.h>
#include <memory>
#include <exception>

#include "HubDatabaseSetup.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
#include "Database/DatabaseSetup/DatabaseSetup.h"
#include "Database/DatabaseSetup/DatabaseSetupRuntime.h"
#include "Log.h"

namespace
{
    bool QuerySetupFlag(MYSQL* connection, char const* query, bool& value)
    {
        if (mysql_query(connection, query))
            return false;
        std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> result(
            mysql_store_result(connection), mysql_free_result);
        MYSQL_ROW row = result ? mysql_fetch_row(result.get()) : nullptr;
        if (!row || !row[0])
            return false;
        value = std::string(row[0]) != "0";
        return true;
    }

    bool RunHubDatabaseSetup(MySQLConnectionInfo const& connectionInfo,
        Skyfire::Database::SetupOptions const& options)
    {
        if (options.SqlPath.empty())
        {
            if (options.AutoSetup)
            {
                SF_LOG_ERROR("server.hub", "HubDatabase.AutoSetup requires HubDatabase.SqlPath.");
                return false;
            }

            SF_LOG_INFO("server.hub",
                "HubDatabase.SqlPath is not configured; hub database update check skipped.");
            return true;
        }

        if (!std::filesystem::is_directory(std::filesystem::path(options.SqlPath) / options.UpdatesDirectory) ||
            (options.ImportPendingUpdates && !std::filesystem::is_directory(
                std::filesystem::path(options.SqlPath) / options.PendingUpdatesDirectory)))
        {
            SF_LOG_ERROR("server.hub", "HubDatabase.SqlPath is missing its hub update directories.");
            return false;
        }

        if (connectionInfo._database.empty())
        {
            SF_LOG_ERROR("server.hub",
                "HubDatabase setup and update checks require a hub database name.");
            return false;
        }

        Skyfire::Database::SetupRuntimeContext context =
            { "server.hub", "HubDatabase", "hub", "a hub", "Hub", "Failed while executing hub setup SQL" };
        if (!Skyfire::Database::EnsureDatabaseExists(connectionInfo, options, context))
            return false;

        MYSQL* setupConnectionRaw = NULL;
        if (!Skyfire::Database::ConnectToMySQLServer(connectionInfo, connectionInfo._database.c_str(),
            setupConnectionRaw, context))
            return false;

        std::unique_ptr<MYSQL, decltype(&mysql_close)> setupConnection(setupConnectionRaw, mysql_close);

        // Keep concurrent hub startups from applying the same migration. MySQL
        // releases this advisory lock when the setup connection closes.
        if (mysql_query(setupConnection.get(),
            "SELECT GET_LOCK(CONCAT('skyfire.hub.setup.', SHA1(DATABASE())), 0)"))
            return false;
        std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> lockResult(
            mysql_store_result(setupConnection.get()), mysql_free_result);
        MYSQL_ROW lockRow = lockResult ? mysql_fetch_row(lockResult.get()) : nullptr;
        if (!lockRow || !lockRow[0] || std::string(lockRow[0]) != "1")
        {
            SF_LOG_ERROR("server.hub", "Another hub database setup is already running.");
            return false;
        }
        lockResult.reset();

        std::filesystem::path baseSqlPath = Skyfire::Database::GetDatabaseBaseSqlPath(options);
        bool baseSqlExists = std::filesystem::exists(baseSqlPath);
        std::vector<Skyfire::Database::SqlUpdateFile> updates = Skyfire::Database::DiscoverSqlUpdates(options);

        Skyfire::Database::SetupState state;
        if (!Skyfire::Database::LoadDatabaseSetupState(setupConnection.get(), options, state, context))
            return false;

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildHubDatabaseSetupPlan(options, state, baseSqlExists, updates);
        if (!plan.IsValid())
        {
            SF_LOG_ERROR("server.hub", "%s", plan.Error.c_str());
            return false;
        }
        if (plan.ShouldInstallBase || plan.ShouldBaselineUpdates || !plan.PendingUpdates.empty())
        {
            bool hasWorker = false;
            bool busy = false;
            if (!QuerySetupFlag(setupConnection.get(),
                "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() "
                "AND table_name='hub_backup_worker'", hasWorker))
                return false;
            if (hasWorker && (!QuerySetupFlag(setupConnection.get(),
                "SELECT COUNT(*) FROM hub_backup_worker WHERE lease_until>NOW() OR maintenance<>0 OR recovery_safe=0",
                busy) || busy))
            {
                SF_LOG_ERROR("server.hub", "Stop the backup worker and resolve recovery maintenance before hub database updates.");
                return false;
            }
            bool hasJobs = false;
            if (!QuerySetupFlag(setupConnection.get(),
                "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema=DATABASE() "
                "AND table_name='hub_backup_jobs'", hasJobs))
                return false;
            if (hasJobs && (!QuerySetupFlag(setupConnection.get(),
                "SELECT COUNT(*) FROM hub_backup_jobs WHERE state IN ('queued','running')", busy) || busy))
            {
                SF_LOG_ERROR("server.hub", "Finish or recover active backup jobs before hub database updates.");
                return false;
            }
        }
        Skyfire::Database::LogSetupPlan(plan, updates.size(), false, context);

        if (plan.ShouldInstallBase)
        {
            std::string baseSql;
            SF_LOG_INFO("server.hub", "Installing hub database base SQL from %s.",
                baseSqlPath.string().c_str());
            if (!Skyfire::Database::ExecuteSqlFile(setupConnection.get(), baseSqlPath, baseSql, context))
                return false;
        }

        if (!Skyfire::Database::EnsureSetupTrackingTables(setupConnection.get(), context))
            return false;

        if (plan.ShouldInstallBase)
        {
            if (mysql_query(setupConnection.get(), "START TRANSACTION"))
                return false;
            for (auto const& update : plan.BaselineUpdates)
                if (mysql_query(setupConnection.get(), Skyfire::Database::BuildUpdateTrackingInsertSql(
                    options.Domain, update.Name, update.Hash).c_str()))
                {
                    SF_LOG_ERROR("server.hub", "Cannot record hub base migration %s: %s",
                        update.Name.c_str(), mysql_error(setupConnection.get()));
                    mysql_query(setupConnection.get(), "ROLLBACK");
                    return false;
                }
            if (mysql_query(setupConnection.get(), "COMMIT"))
                return false;
        }
        else if (!Skyfire::Database::BaselineSetupUpdates(setupConnection.get(), options, plan, context))
            return false;

        if (!Skyfire::Database::ApplyPendingSetupUpdates(setupConnection.get(), options, plan, context))
            return false;

        SF_LOG_INFO("server.hub",
            "Hub database setup/update complete. Base installed: %s, updates applied: %u, updates baselined: %u.",
            plan.ShouldInstallBase ? "yes" : "no", uint32(plan.PendingUpdates.size()),
            uint32(plan.BaselineUpdates.size()));
        return true;
    }

}

bool SetupHubDatabase(std::string const& connectionString)
{
    try
    {
        auto options = Skyfire::Database::MakeHubDatabaseSetupOptions(
            sConfigMgr->GetBoolDefault("HubDatabase.AutoSetup", false),
            sConfigMgr->GetBoolDefault("HubDatabase.AutoCreate", false),
            sConfigMgr->GetBoolDefault("HubDatabase.AutoBaseline", false),
            sConfigMgr->GetStringDefault("HubDatabase.SqlPath", ""));
        options.AllowUpdateHashMismatch = sConfigMgr->GetBoolDefault("HubDatabase.AllowUpdateHashMismatch", false);
        options.ImportPendingUpdates = sConfigMgr->GetBoolDefault("HubDatabase.ImportPendingUpdates", false);
        return RunHubDatabaseSetup(MySQLConnectionInfo(connectionString), options);
    }
    catch (std::exception const&)
    {
        SF_LOG_ERROR("server.hub", "Hub database setup failed; check the configured SQL directory and database settings.");
        return false;
    }
}
