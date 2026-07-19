/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DatabaseSetup.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <utility>

namespace Skyfire
{
namespace Database
{
    namespace
    {
        bool EndsWithSqlExtension(std::string const& name)
        {
            if (name.length() < 4)
                return false;

            std::string extension = name.substr(name.length() - 4);
            std::transform(extension.begin(), extension.end(), extension.begin(),
                [](unsigned char c) { return char(std::tolower(c)); });

            return extension == ".sql";
        }

        std::string BuildSqlPath(std::string const& directory, std::string const& name)
        {
            if (directory.empty())
                return name;

            char last = directory[directory.length() - 1];
            if (last == '/' || last == '\\')
                return directory + name;

            char separator = directory.find('\\') != std::string::npos && directory.find('/') == std::string::npos ? '\\' : '/';
            return directory + separator + name;
        }

        std::string Trim(std::string const& text)
        {
            std::string::size_type begin = 0;
            while (begin < text.length() && std::isspace(static_cast<unsigned char>(text[begin])))
                ++begin;

            std::string::size_type end = text.length();
            while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
                --end;

            return text.substr(begin, end - begin);
        }

        bool ReadTextFile(std::filesystem::path const& path, std::string& contents)
        {
            std::ifstream file(path, std::ios::in | std::ios::binary);
            if (!file)
                return false;

            std::ostringstream stream;
            stream << file.rdbuf();
            contents = stream.str();
            return true;
        }

        bool StartsWithCaseInsensitive(std::string const& text, std::string const& prefix)
        {
            if (text.length() < prefix.length())
                return false;

            for (std::string::size_type i = 0; i < prefix.length(); ++i)
            {
                if (std::tolower(static_cast<unsigned char>(text[i])) !=
                    std::tolower(static_cast<unsigned char>(prefix[i])))
                    return false;
            }

            return true;
        }

        bool TryReadDelimiterCommand(std::string const& line, std::string& delimiter)
        {
            std::string trimmed = Trim(line);
            if (!StartsWithCaseInsensitive(trimmed, "delimiter"))
                return false;

            if (trimmed.length() != 9 && !std::isspace(static_cast<unsigned char>(trimmed[9])))
                return false;

            delimiter = Trim(trimmed.substr(9));
            return !delimiter.empty();
        }

        std::string::size_type FindDelimiterOutsideQuotedText(std::string const& sql, std::string const& delimiter)
        {
            bool inSingleQuote = false;
            bool inDoubleQuote = false;
            bool inBacktick = false;
            bool escaped = false;

            for (std::string::size_type i = 0; i < sql.length(); ++i)
            {
                char c = sql[i];

                if (inSingleQuote)
                {
                    if (escaped)
                        escaped = false;
                    else if (c == '\\')
                        escaped = true;
                    else if (c == '\'')
                        inSingleQuote = false;

                    continue;
                }

                if (inDoubleQuote)
                {
                    if (escaped)
                        escaped = false;
                    else if (c == '\\')
                        escaped = true;
                    else if (c == '"')
                        inDoubleQuote = false;

                    continue;
                }

                if (inBacktick)
                {
                    if (c == '`')
                        inBacktick = false;

                    continue;
                }

                if (c == '-' && i + 1 < sql.length() && sql[i + 1] == '-' &&
                    (i + 2 == sql.length() || std::isspace(static_cast<unsigned char>(sql[i + 2]))))
                {
                    i += 2;
                    while (i < sql.length() && sql[i] != '\n')
                        ++i;

                    continue;
                }

                if (c == '#')
                {
                    while (i < sql.length() && sql[i] != '\n')
                        ++i;

                    continue;
                }

                if (c == '/' && i + 1 < sql.length() && sql[i + 1] == '*')
                {
                    i += 2;
                    while (i + 1 < sql.length() && !(sql[i] == '*' && sql[i + 1] == '/'))
                        ++i;

                    if (i + 1 < sql.length())
                        ++i;

                    continue;
                }

                if (!delimiter.empty() && sql.compare(i, delimiter.length(), delimiter) == 0)
                    return i;

                if (c == '\'')
                    inSingleQuote = true;
                else if (c == '"')
                    inDoubleQuote = true;
                else if (c == '`')
                    inBacktick = true;
            }

            return std::string::npos;
        }
    }

    bool SetupPlan::IsValid() const
    {
        return Error.empty();
    }

    SetupOptions MakeAuthDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath)
    {
        return MakeAuthDatabaseSetupOptions(autoSetup, autoCreate, false, std::move(sqlPath));
    }

    SetupOptions MakeAuthDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath)
    {
        SetupOptions options;
        options.AutoSetup = autoSetup;
        options.AutoCreate = autoCreate;
        options.AutoBaseline = autoBaseline;
        options.Domain = "auth";
        options.SqlPath = std::move(sqlPath);
        options.BaseFileName = "auth_database.sql";
        options.UpdatesDirectory = "updates/auth";

        return options;
    }

    SetupOptions MakeCharacterDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath)
    {
        return MakeCharacterDatabaseSetupOptions(autoSetup, autoCreate, false, std::move(sqlPath));
    }

    SetupOptions MakeCharacterDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath)
    {
        SetupOptions options;
        options.AutoSetup = autoSetup;
        options.AutoCreate = autoCreate;
        options.AutoBaseline = autoBaseline;
        options.Domain = "characters";
        options.SqlPath = std::move(sqlPath);
        options.BaseFileName = "characters_database.sql";
        options.UpdatesDirectory = "updates/characters";

        return options;
    }

    SetupOptions MakeWorldDatabaseSetupOptions(bool autoSetup, bool autoCreate, std::string sqlPath,
        std::string externalBaseFile)
    {
        return MakeWorldDatabaseSetupOptions(autoSetup, autoCreate, false, std::move(sqlPath),
            std::move(externalBaseFile));
    }

    SetupOptions MakeWorldDatabaseSetupOptions(bool autoSetup, bool autoCreate, bool autoBaseline, std::string sqlPath,
        std::string externalBaseFile)
    {
        SetupOptions options;
        options.AutoSetup = autoSetup;
        options.AutoCreate = autoCreate;
        options.AutoBaseline = autoBaseline;
        options.Domain = "world";
        options.SqlPath = std::move(sqlPath);
        options.ExternalBaseFile = std::move(externalBaseFile);
        options.RequiredBaseFileNames.push_back("stored_procs.sql");
        options.UpdatesDirectory = "updates/world";

        return options;
    }

    std::vector<SqlUpdateFile> BuildSortedSqlUpdateList(std::vector<std::string> const& names, std::string const& directory)
    {
        std::vector<SqlUpdateFile> updates;
        updates.reserve(names.size());

        for (std::string const& name : names)
        {
            if (!EndsWithSqlExtension(name))
                continue;

            updates.push_back({ name, BuildSqlPath(directory, name) });
        }

        std::sort(updates.begin(), updates.end(), [](SqlUpdateFile const& left, SqlUpdateFile const& right)
        {
            return left.Name < right.Name;
        });

        return updates;
    }

    std::vector<SqlUpdateFile> DiscoverSqlUpdates(SetupOptions const& options)
    {
        std::vector<std::string> names;

        if (options.SqlPath.empty())
            return {};

        std::filesystem::path updatesDirectory = std::filesystem::path(options.SqlPath) / options.UpdatesDirectory;
        updatesDirectory.make_preferred();
        if (!std::filesystem::exists(updatesDirectory) || !std::filesystem::is_directory(updatesDirectory))
            return {};

        for (std::filesystem::directory_entry const& entry : std::filesystem::directory_iterator(updatesDirectory))
        {
            if (entry.is_regular_file())
                names.push_back(entry.path().filename().string());
        }

        std::vector<SqlUpdateFile> updates = BuildSortedSqlUpdateList(names, updatesDirectory.string());
        for (SqlUpdateFile& update : updates)
        {
            std::string contents;
            if (ReadTextFile(update.Path, contents))
                update.Hash = CalculateStableSqlHash(contents);
        }

        return updates;
    }

    std::vector<std::string> SplitSqlStatements(std::string const& sql)
    {
        std::vector<std::string> statements;
        std::string current;
        std::string delimiter = ";";
        std::istringstream input(sql);
        std::string line;

        while (std::getline(input, line))
        {
            std::string newDelimiter;
            if (Trim(current).empty() && TryReadDelimiterCommand(line, newDelimiter))
            {
                current.clear();
                delimiter = newDelimiter;
                continue;
            }

            current += line;
            current.push_back('\n');
            while (true)
            {
                std::string::size_type delimiterPosition = FindDelimiterOutsideQuotedText(current, delimiter);
                if (delimiterPosition == std::string::npos)
                    break;

                std::string statement = Trim(current.substr(0, delimiterPosition));
                if (!statement.empty())
                    statements.push_back(statement);

                current.erase(0, delimiterPosition + delimiter.length());
            }
        }

        std::string statement = Trim(current);
        if (!statement.empty())
            statements.push_back(statement);

        return statements;
    }

    bool ExecuteSqlScript(std::string const& sql, std::function<bool(std::string const&)> const& executor)
    {
        std::string current;
        std::string delimiter = ";";
        std::istringstream input(sql);
        std::string line;

        while (std::getline(input, line))
        {
            std::string newDelimiter;
            if (Trim(current).empty() && TryReadDelimiterCommand(line, newDelimiter))
            {
                current.clear();
                delimiter = newDelimiter;
                continue;
            }

            current += line;
            current.push_back('\n');
            while (true)
            {
                std::string::size_type delimiterPosition = FindDelimiterOutsideQuotedText(current, delimiter);
                if (delimiterPosition == std::string::npos)
                    break;

                std::string statement = Trim(current.substr(0, delimiterPosition));
                if (!statement.empty() && !executor(statement))
                    return false;

                current.erase(0, delimiterPosition + delimiter.length());
            }
        }

        std::string statement = Trim(current);
        if (!statement.empty() && !executor(statement))
            return false;

        return true;
    }

    std::string CalculateStableSqlHash(std::string const& sql)
    {
        std::uint64_t hash = 14695981039346656037ull;

        for (unsigned char c : sql)
        {
            hash ^= c;
            hash *= 1099511628211ull;
        }

        std::ostringstream stream;
        stream << std::hex << std::setfill('0') << std::setw(16) << hash;
        return stream.str();
    }

    std::string EscapeSqlString(std::string const& value)
    {
        std::string escaped;
        escaped.reserve(value.length());

        for (char c : value)
        {
            if (c == '\'' || c == '\\')
                escaped.push_back('\\');

            escaped.push_back(c);
        }

        return escaped;
    }

    std::string EscapeSqlIdentifier(std::string const& identifier)
    {
        std::string escaped;
        escaped.reserve(identifier.length());

        for (char c : identifier)
        {
            if (c == '`')
                escaped.push_back('`');

            escaped.push_back(c);
        }

        return escaped;
    }

    std::string BuildCreateDatabaseSql(std::string const& databaseName)
    {
        return "CREATE DATABASE IF NOT EXISTS `" + EscapeSqlIdentifier(databaseName) +
            "` DEFAULT CHARACTER SET utf8";
    }

    std::string BuildUpdateTrackingTableSql()
    {
        return "CREATE TABLE IF NOT EXISTS `skyfire_db_updates` ("
            "`domain` varchar(32) NOT NULL,"
            "`filename` varchar(255) NOT NULL,"
            "`hash` varchar(64) NOT NULL,"
            "`applied_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "PRIMARY KEY (`domain`,`filename`)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb3";
    }

    std::string BuildUpdateTrackingInsertSql(std::string const& domain, std::string const& filename,
        std::string const& hash)
    {
        return "INSERT INTO `skyfire_db_updates` (`domain`, `filename`, `hash`, `applied_at`) VALUES ('" +
            EscapeSqlString(domain) + "', '" + EscapeSqlString(filename) + "', '" + EscapeSqlString(hash) +
            "', NOW())";
    }

    std::string BuildDbUpdateAuditTableSql()
    {
        return "CREATE TABLE IF NOT EXISTS `db_update` ("
            "`date` date NOT NULL,"
            "`time` time NOT NULL,"
            "`filename` varchar(255) NOT NULL"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb3";
    }

    std::string BuildDbUpdateAuditInsertSql(std::string const& filename)
    {
        return "INSERT INTO `db_update` (`date`, `time`, `filename`) VALUES (CURDATE(), CURTIME(), '" +
            EscapeSqlString(filename) + "')";
    }

    std::string BuildSetupPlanSummary(std::string const& databaseName, SetupPlan const& plan,
        std::size_t discoveredUpdateCount, bool appliesRequiredSql)
    {
        char const* mode = "check-only";
        if (plan.ShouldInstallBase)
            mode = "install-base";
        else if (plan.ShouldBaselineUpdates)
            mode = "baseline";
        else if (!plan.PendingUpdates.empty())
            mode = "apply-updates";

        std::ostringstream stream;
        stream << databaseName << " database setup plan: mode=" << mode
            << ", discovered updates=" << discoveredUpdateCount
            << ", pending updates=" << plan.PendingUpdates.size()
            << ", baseline updates=" << plan.BaselineUpdates.size()
            << ", hash mismatch bypasses=" << plan.HashMismatchedUpdates.size()
            << ", install base=" << (plan.ShouldInstallBase ? "yes" : "no")
            << ", required SQL=" << (appliesRequiredSql ? "yes" : "no")
            << ".";

        return stream.str();
    }

    SetupPlan BuildDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates)
    {
        SetupPlan plan;

        if (!state.DatabaseExists)
        {
            if (!options.AutoSetup)
            {
                plan.Error = options.Domain + " database does not exist and auto setup is disabled.";
                return plan;
            }

            if (!options.AutoCreate)
            {
                plan.Error = options.Domain + " database does not exist and auto create is disabled.";
                return plan;
            }

            plan.ShouldCreateDatabase = true;
            plan.ShouldInstallBase = true;
        }
        else if (state.SchemaTableCount == 0)
        {
            if (!options.AutoSetup)
            {
                plan.Error = options.Domain + " database is empty and auto setup is disabled.";
                return plan;
            }

            plan.ShouldInstallBase = true;
        }

        if (plan.ShouldInstallBase && !baseSqlExists)
        {
            plan.Error = options.Domain + " database base SQL file was not found.";
            return plan;
        }

        if (!plan.ShouldInstallBase && !state.UpdateTrackingExists && !updates.empty())
        {
            if (!options.AutoBaseline)
            {
                plan.Error = options.Domain + " database update tracking table is missing on a non-empty schema.";
                return plan;
            }

            plan.ShouldBaselineUpdates = true;
            plan.BaselineUpdates = updates;
            for (SqlUpdateFile const& update : plan.BaselineUpdates)
            {
                if (update.Hash.empty())
                {
                    plan.ShouldBaselineUpdates = false;
                    plan.BaselineUpdates.clear();
                    plan.Error = options.Domain + " database update `" + update.Name + "` has no content hash for baseline.";
                    return plan;
                }
            }

            return plan;
        }

        for (SqlUpdateFile const& update : updates)
        {
            if (state.AppliedUpdates.find(update.Name) == state.AppliedUpdates.end())
            {
                plan.PendingUpdates.push_back(update);
                continue;
            }

            std::map<std::string, std::string>::const_iterator appliedHash = state.AppliedUpdateHashes.find(update.Name);
            if (appliedHash != state.AppliedUpdateHashes.end() && !appliedHash->second.empty() &&
                !update.Hash.empty() && appliedHash->second != update.Hash)
            {
                if (options.AllowUpdateHashMismatch)
                {
                    plan.HashMismatchedUpdates.push_back(update);
                    continue;
                }

                plan.Error = options.Domain + " database update `" + update.Name + "` was already applied with a different hash.";
                plan.PendingUpdates.clear();
                return plan;
            }
        }

        return plan;
    }

    SetupPlan BuildAuthDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates)
    {
        return BuildDatabaseSetupPlan(options, state, baseSqlExists, updates);
    }

    SetupPlan BuildCharacterDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool baseSqlExists,
        std::vector<SqlUpdateFile> const& updates)
    {
        return BuildDatabaseSetupPlan(options, state, baseSqlExists, updates);
    }

    SetupPlan BuildWorldDatabaseSetupPlan(SetupOptions const& options, SetupState const& state, bool externalBaseSqlExists,
        bool requiredBaseSqlExists, std::vector<SqlUpdateFile> const& updates)
    {
        SetupPlan plan = BuildDatabaseSetupPlan(options, state, true, updates);
        if (!plan.IsValid())
            return plan;

        if (plan.ShouldInstallBase && (options.ExternalBaseFile.empty() || !externalBaseSqlExists))
        {
            plan.Error = "world database external base SQL file was not found.";
            return plan;
        }

        if (plan.ShouldInstallBase && !requiredBaseSqlExists)
        {
            plan.Error = "world database required base SQL file was not found.";
            return plan;
        }

        return plan;
    }
}
}
