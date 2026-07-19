/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DatabaseSetupRuntime.h"

#ifdef _WIN32
#include <winsock2.h>
#endif
#include <mysql.h>

#include "Common.h"
#include "Log.h"
#include "MySQLConnection.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <system_error>

namespace Skyfire
{
namespace Database
{
    namespace
    {
        bool StartsWithCaseInsensitive(std::string const& text, char const* prefix)
        {
            std::size_t prefixLength = std::strlen(prefix);
            if (text.length() < prefixLength)
                return false;

            for (std::size_t i = 0; i < prefixLength; ++i)
            {
                if (std::tolower(static_cast<unsigned char>(text[i])) !=
                    std::tolower(static_cast<unsigned char>(prefix[i])))
                    return false;
            }

            return true;
        }

        std::string ExtractSqlTableName(std::string const& statement)
        {
            std::string::size_type begin = 0;
            while (begin < statement.length() && std::isspace(static_cast<unsigned char>(statement[begin])))
                ++begin;

            std::string trimmed = statement.substr(begin);
            if (!StartsWithCaseInsensitive(trimmed, "DROP TABLE") &&
                !StartsWithCaseInsensitive(trimmed, "CREATE TABLE") &&
                !StartsWithCaseInsensitive(trimmed, "INSERT INTO") &&
                !StartsWithCaseInsensitive(trimmed, "DELETE FROM") &&
                !StartsWithCaseInsensitive(trimmed, "ALTER TABLE"))
                return "";

            std::string::size_type tableBegin = trimmed.find('`');
            if (tableBegin == std::string::npos)
                return "";

            std::string::size_type tableEnd = trimmed.find('`', tableBegin + 1);
            if (tableEnd == std::string::npos)
                return "";

            return trimmed.substr(tableBegin + 1, tableEnd - tableBegin - 1);
        }

        uint32 CalculateSqlProgressPercent(SqlStatementContext const& sqlContext)
        {
            if (!sqlContext.TotalBytes)
                return 0;

            std::uintmax_t percent = sqlContext.BytesRead * 100 / sqlContext.TotalBytes;
            return uint32(percent > 100 ? 100 : percent);
        }

        bool ExecuteSetupQuery(MYSQL* setupConnection, std::string const& sql, char const* queryContext,
            SetupRuntimeContext const& context)
        {
            if (mysql_query(setupConnection, sql.c_str()))
            {
                SF_LOG_ERROR(context.LogFilter, "%s: %s", queryContext, mysql_error(setupConnection));
                return false;
            }

            while (true)
            {
                MYSQL_RES* result = mysql_store_result(setupConnection);
                if (result)
                    mysql_free_result(result);
                else if (mysql_field_count(setupConnection) != 0)
                {
                    SF_LOG_ERROR(context.LogFilter, "%s: %s", queryContext, mysql_error(setupConnection));
                    return false;
                }

                int nextResult = mysql_next_result(setupConnection);
                if (nextResult > 0)
                {
                    SF_LOG_ERROR(context.LogFilter, "%s: %s", queryContext, mysql_error(setupConnection));
                    return false;
                }

                if (nextResult < 0)
                    break;
            }

            return true;
        }

        bool QuerySetupUInt32(MYSQL* setupConnection, char const* sql, uint32& value, char const* queryContext,
            SetupRuntimeContext const& context)
        {
            if (mysql_query(setupConnection, sql))
            {
                SF_LOG_ERROR(context.LogFilter, "%s: %s", queryContext, mysql_error(setupConnection));
                return false;
            }

            MYSQL_RES* result = mysql_store_result(setupConnection);
            if (!result)
            {
                SF_LOG_ERROR(context.LogFilter, "%s: %s", queryContext, mysql_error(setupConnection));
                return false;
            }

            std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> resultGuard(result, mysql_free_result);
            MYSQL_ROW row = mysql_fetch_row(result);
            if (!row || !row[0])
            {
                SF_LOG_ERROR(context.LogFilter, "%s returned no value.", queryContext);
                return false;
            }

            value = uint32(std::strtoul(row[0], NULL, 10));
            return true;
        }

        bool RecordUpdateMetadata(MYSQL* setupConnection, SetupOptions const& options, SqlUpdateFile const& update,
            std::string const& hash, SetupRuntimeContext const& context)
        {
            if (hash.empty())
            {
                SF_LOG_ERROR(context.LogFilter, "%s database update %s has no content hash.",
                    context.DatabaseNameTitle, update.Name.c_str());
                return false;
            }

            std::string queryContext = "Could not record " + std::string(context.DatabaseName) + " database update";
            return ExecuteSetupQuery(setupConnection,
                BuildUpdateTrackingInsertSql(options.Domain, update.Name, hash), queryContext.c_str(), context);
        }

        bool RecordAppliedUpdate(MYSQL* setupConnection, SetupOptions const& options, SqlUpdateFile const& update,
            std::string const& hash, SetupRuntimeContext const& context)
        {
            if (!RecordUpdateMetadata(setupConnection, options, update, hash, context))
                return false;

            std::string queryContext = "Could not record " + std::string(context.DatabaseName) +
                " database update audit row";
            return ExecuteSetupQuery(setupConnection, BuildDbUpdateAuditInsertSql(update.Name), queryContext.c_str(),
                context);
        }
    }

    std::filesystem::path GetDatabaseBaseSqlPath(SetupOptions const& options)
    {
        return GetDatabaseBaseSqlPath(options, options.BaseFileName);
    }

    std::filesystem::path GetDatabaseBaseSqlPath(SetupOptions const& options, std::string const& baseFileName)
    {
        std::filesystem::path path = std::filesystem::path(options.SqlPath) / "base" / baseFileName;
        path.make_preferred();
        return path;
    }

    bool ConnectToMySQLServer(MySQLConnectionInfo const& connectionInfo, char const* databaseName, MYSQL*& handle,
        SetupRuntimeContext const& context)
    {
        MYSQL* mysqlInit = mysql_init(NULL);
        if (!mysqlInit)
        {
            SF_LOG_ERROR(context.LogFilter, "Could not initialize MySQL setup connection.");
            return false;
        }

        mysql_options(mysqlInit, MYSQL_SET_CHARSET_NAME, "utf8");

        int port = 0;
        char const* unixSocket = NULL;
        std::string host = connectionInfo._host;

#ifdef _WIN32
        if (host == ".")
        {
            unsigned int protocol = MYSQL_PROTOCOL_PIPE;
            mysql_options(mysqlInit, MYSQL_OPT_PROTOCOL, reinterpret_cast<char const*>(&protocol));
        }
        else
            port = atoi(connectionInfo._port_or_socket.c_str());
#else
        if (host == ".")
        {
            unsigned int protocol = MYSQL_PROTOCOL_SOCKET;
            mysql_options(mysqlInit, MYSQL_OPT_PROTOCOL, reinterpret_cast<char const*>(&protocol));
            host = "localhost";
            unixSocket = connectionInfo._port_or_socket.c_str();
        }
        else
            port = atoi(connectionInfo._port_or_socket.c_str());
#endif

        handle = mysql_real_connect(mysqlInit, host.c_str(), connectionInfo._user.c_str(),
            connectionInfo._password.c_str(), databaseName, port, unixSocket, 0);

        if (!handle)
        {
            SF_LOG_ERROR(context.LogFilter, "Could not connect to MySQL server for setup: %s",
                mysql_error(mysqlInit));
            mysql_close(mysqlInit);
            return false;
        }

        return true;
    }

    bool EnsureDatabaseExists(MySQLConnectionInfo const& connectionInfo, SetupOptions const& options,
        SetupRuntimeContext const& context)
    {
        if (!options.AutoSetup || !options.AutoCreate)
            return true;

        if (connectionInfo._database.empty())
        {
            SF_LOG_ERROR(context.LogFilter, "%s.AutoCreate requires %s database name.",
                context.ConfigPrefix, context.DatabaseNameWithArticle);
            return false;
        }

        MYSQL* setupConnection = NULL;
        if (!ConnectToMySQLServer(connectionInfo, NULL, setupConnection, context))
            return false;

        if (mysql_query(setupConnection, BuildCreateDatabaseSql(connectionInfo._database).c_str()))
        {
            SF_LOG_ERROR(context.LogFilter, "Could not create %s database `%s`: %s",
                context.DatabaseName, connectionInfo._database.c_str(), mysql_error(setupConnection));
            mysql_close(setupConnection);
            return false;
        }

        SF_LOG_INFO(context.LogFilter, "%s database `%s` exists or was created.",
            context.DatabaseNameTitle, connectionInfo._database.c_str());
        mysql_close(setupConnection);
        return true;
    }

    bool LoadDatabaseSetupState(MYSQL* setupConnection, SetupOptions const& options, SetupState& state,
        SetupRuntimeContext const& context)
    {
        state.DatabaseExists = true;

        std::string tableCountContext = "Could not inspect " + std::string(context.DatabaseName) +
            " database table count";
        if (!QuerySetupUInt32(setupConnection,
            "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = DATABASE()",
            state.SchemaTableCount, tableCountContext.c_str(), context))
            return false;

        std::string trackingTableContext = "Could not inspect " + std::string(context.DatabaseName) +
            " database update tracking table";
        uint32 updateTrackingTableCount = 0;
        if (!QuerySetupUInt32(setupConnection,
            "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = DATABASE() "
            "AND table_name = 'skyfire_db_updates'",
            updateTrackingTableCount, trackingTableContext.c_str(), context))
            return false;

        state.UpdateTrackingExists = updateTrackingTableCount != 0;
        if (!state.UpdateTrackingExists)
            return true;

        std::string appliedQuery = "SELECT `filename`, `hash` FROM `skyfire_db_updates` WHERE `domain` = '" +
            EscapeSqlString(options.Domain) + "'";
        if (mysql_query(setupConnection, appliedQuery.c_str()))
        {
            SF_LOG_ERROR(context.LogFilter, "Could not read %s database applied updates: %s",
                context.DatabaseName, mysql_error(setupConnection));
            return false;
        }

        MYSQL_RES* result = mysql_store_result(setupConnection);
        if (!result)
        {
            SF_LOG_ERROR(context.LogFilter, "Could not read %s database applied updates: %s",
                context.DatabaseName, mysql_error(setupConnection));
            return false;
        }

        std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)> resultGuard(result, mysql_free_result);
        while (MYSQL_ROW row = mysql_fetch_row(result))
        {
            if (row[0])
            {
                state.AppliedUpdates.insert(row[0]);
                if (row[1])
                    state.AppliedUpdateHashes[row[0]] = row[1];
            }
        }

        return true;
    }

    bool ExecuteSqlFile(MYSQL* setupConnection, std::filesystem::path const& path, std::string& contents,
        SetupRuntimeContext const& context)
    {
        contents.clear();

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file)
        {
            SF_LOG_ERROR(context.LogFilter, "Could not read SQL file %s.", path.string().c_str());
            return false;
        }

        std::uintmax_t totalBytes = 0;
        std::error_code fileSizeError;
        totalBytes = std::filesystem::file_size(path, fileSizeError);
        if (fileSizeError)
            totalBytes = 0;

        std::string fileName = path.filename().string();
        std::string currentTable;
        uint32 lastLoggedPercent = 0;

        SF_LOG_INFO(context.LogFilter, "Executing SQL file %s (%llu bytes).",
            path.string().c_str(), static_cast<unsigned long long>(totalBytes));

        bool executed = ExecuteSqlStream(file, totalBytes,
            [setupConnection, &context, &currentTable, &fileName, &lastLoggedPercent]
            (std::string const& statement, SqlStatementContext const& sqlContext)
        {
            std::string detectedTable = ExtractSqlTableName(statement);
            if (!detectedTable.empty() && detectedTable != currentTable)
            {
                currentTable = detectedTable;
                SF_LOG_INFO(context.LogFilter, "Importing %s database table `%s` from %s.",
                    context.DatabaseName, currentTable.c_str(), fileName.c_str());
            }

            uint32 percent = CalculateSqlProgressPercent(sqlContext);
            if (sqlContext.StatementCount == 1 || sqlContext.StatementCount % 500 == 0 ||
                percent >= lastLoggedPercent + 5 || percent == 100)
            {
                lastLoggedPercent = percent;
                SF_LOG_INFO(context.LogFilter,
                    "Import progress: %u%% - %s database table `%s` - %u statements.",
                    percent, context.DatabaseName, currentTable.empty() ? "unknown" : currentTable.c_str(),
                    uint32(sqlContext.StatementCount));
            }

            std::ostringstream queryContext;
            queryContext << context.SqlExecutionContext << " statement " << sqlContext.StatementCount
                << " near byte " << static_cast<unsigned long long>(sqlContext.BytesRead);
            if (!currentTable.empty())
                queryContext << " while importing table `" << currentTable << "`";

            return ExecuteSetupQuery(setupConnection, statement, queryContext.str().c_str(), context);
        });

        if (!executed)
        {
            SF_LOG_ERROR(context.LogFilter, "Failed while executing SQL file %s.", path.string().c_str());
            return false;
        }

        SF_LOG_INFO(context.LogFilter, "Finished executing SQL file %s.", path.string().c_str());

        return true;
    }

    void LogSetupPlan(SetupPlan const& plan, std::size_t discoveredUpdateCount, bool appliesRequiredSql,
        SetupRuntimeContext const& context)
    {
        std::string summary = BuildSetupPlanSummary(context.DatabaseNameTitle, plan, discoveredUpdateCount,
            appliesRequiredSql);
        SF_LOG_INFO(context.LogFilter, "%s", summary.c_str());

        for (SqlUpdateFile const& update : plan.HashMismatchedUpdates)
        {
            SF_LOG_WARN(context.LogFilter,
                "%s database update %s was already applied with a different hash; %s.AllowUpdateHashMismatch is enabled, skipping reapply.",
                context.DatabaseNameTitle, update.Name.c_str(), context.ConfigPrefix);
        }
    }

    bool EnsureSetupTrackingTables(MYSQL* setupConnection, SetupRuntimeContext const& context)
    {
        std::string trackingContext = "Could not create " + std::string(context.DatabaseName) +
            " database update tracking table";
        if (!ExecuteSetupQuery(setupConnection, BuildUpdateTrackingTableSql(), trackingContext.c_str(), context))
        {
            SF_LOG_ERROR(context.LogFilter, "%s.", trackingContext.c_str());
            return false;
        }

        std::string auditContext = "Could not create " + std::string(context.DatabaseName) +
            " database update audit table";
        if (!ExecuteSetupQuery(setupConnection, BuildDbUpdateAuditTableSql(), auditContext.c_str(), context))
        {
            SF_LOG_ERROR(context.LogFilter, "%s.", auditContext.c_str());
            return false;
        }

        return true;
    }

    bool BaselineSetupUpdates(MYSQL* setupConnection, SetupOptions const& options, SetupPlan const& plan,
        SetupRuntimeContext const& context)
    {
        if (!plan.ShouldBaselineUpdates)
            return true;

        SF_LOG_WARN(context.LogFilter,
            "%s.AutoBaseline is enabled. Recording %u %s updates as already applied without executing them.",
            context.ConfigPrefix, uint32(plan.BaselineUpdates.size()), context.DatabaseName);
        SF_LOG_WARN(context.LogFilter,
            "Disable %s.AutoBaseline after this startup to keep future update checks strict.",
            context.ConfigPrefix);

        for (SqlUpdateFile const& update : plan.BaselineUpdates)
        {
            if (!RecordUpdateMetadata(setupConnection, options, update, update.Hash, context))
            {
                SF_LOG_ERROR(context.LogFilter, "Could not baseline %s database update %s.",
                    context.DatabaseName, update.Name.c_str());
                return false;
            }
        }

        return true;
    }

    bool ApplyPendingSetupUpdates(MYSQL* setupConnection, SetupOptions const& options, SetupPlan const& plan,
        SetupRuntimeContext const& context)
    {
        for (SqlUpdateFile const& update : plan.PendingUpdates)
        {
            std::string updateSql;
            SF_LOG_INFO(context.LogFilter, "Applying %s database update %s.",
                context.DatabaseName, update.Name.c_str());
            if (!ExecuteSqlFile(setupConnection, update.Path, updateSql, context))
                return false;

            if (!RecordAppliedUpdate(setupConnection, options, update, update.Hash, context))
            {
                SF_LOG_ERROR(context.LogFilter, "Could not record %s database update %s.",
                    context.DatabaseName, update.Name.c_str());
                return false;
            }
        }

        return true;
    }
}
}
