/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DATABASE_SETUP_RUNTIME_H
#define SKYFIRE_DATABASE_SETUP_RUNTIME_H

#include "DatabaseSetup.h"

#ifdef _WIN32
#include <winsock2.h>
#endif
#include <mysql.h>

#include <cstddef>
#include <filesystem>
#include <string>

struct MySQLConnectionInfo;

namespace Skyfire
{
namespace Database
{
    struct SetupRuntimeContext
    {
        char const* LogFilter = "";
        char const* ConfigPrefix = "";
        char const* DatabaseName = "";
        char const* DatabaseNameWithArticle = "";
        char const* DatabaseNameTitle = "";
        char const* SqlExecutionContext = "";
    };

    std::filesystem::path GetDatabaseBaseSqlPath(SetupOptions const& options);
    std::filesystem::path GetDatabaseBaseSqlPath(SetupOptions const& options, std::string const& baseFileName);
    bool ConnectToMySQLServer(MySQLConnectionInfo const& connectionInfo, char const* databaseName, MYSQL*& handle,
        SetupRuntimeContext const& context);
    bool EnsureDatabaseExists(MySQLConnectionInfo const& connectionInfo, SetupOptions const& options,
        SetupRuntimeContext const& context);
    bool LoadDatabaseSetupState(MYSQL* setupConnection, SetupOptions const& options, SetupState& state,
        SetupRuntimeContext const& context);
    bool ExecuteSqlFile(MYSQL* setupConnection, std::filesystem::path const& path, std::string& contents,
        SetupRuntimeContext const& context);
    void LogSetupPlan(SetupPlan const& plan, std::size_t discoveredUpdateCount, bool appliesRequiredSql,
        SetupRuntimeContext const& context);
    bool EnsureSetupTrackingTables(MYSQL* setupConnection, SetupRuntimeContext const& context);
    bool BaselineSetupUpdates(MYSQL* setupConnection, SetupOptions const& options, SetupPlan const& plan,
        SetupRuntimeContext const& context);
    bool ApplyPendingSetupUpdates(MYSQL* setupConnection, SetupOptions const& options, SetupPlan const& plan,
        SetupRuntimeContext const& context);
}
}

#endif
