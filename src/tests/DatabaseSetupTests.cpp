/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DatabaseSetup.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool ExpectEqual(std::string const& actual, std::string const& expected, char const* message)
    {
        if (actual == expected)
            return true;

        std::cerr << message << '\n'
                  << "  actual:   " << actual << '\n'
                  << "  expected: " << expected << '\n';
        return false;
    }

    bool TestAuthDefaultsAreConservative()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(false, false, "");

        bool passed = true;
        passed &= Expect(options.Domain == "auth", "Auth setup domain should be auth");
        passed &= Expect(!options.AutoSetup, "Auth auto setup should default off");
        passed &= Expect(!options.AutoCreate, "Auth auto create should default off");
        passed &= Expect(!options.AutoBaseline, "Auth auto baseline should default off");
        passed &= Expect(!options.AllowUpdateHashMismatch, "Auth update hash mismatch bypass should default off");
        passed &= Expect(!options.ImportPendingUpdates, "Auth pending updates import should default off");
        passed &= Expect(options.SqlPath.empty(), "Auth setup SQL path should default empty");
        passed &= Expect(options.BaseFileName == "auth_database.sql", "Auth setup should use the auth base SQL file");
        passed &= Expect(options.UpdatesDirectory == "updates/auth", "Auth setup should use the auth updates directory");
        passed &= Expect(options.PendingUpdatesDirectory == "pending_updates/auth",
            "Auth setup should use the auth pending updates directory");

        return passed;
    }

    bool TestCharacterDefaultsAreConservative()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeCharacterDatabaseSetupOptions(false, false, "");

        bool passed = true;
        passed &= Expect(options.Domain == "characters", "Character setup domain should be characters");
        passed &= Expect(!options.AutoSetup, "Character auto setup should default off");
        passed &= Expect(!options.AutoCreate, "Character auto create should default off");
        passed &= Expect(!options.AutoBaseline, "Character auto baseline should default off");
        passed &= Expect(!options.AllowUpdateHashMismatch, "Character update hash mismatch bypass should default off");
        passed &= Expect(!options.ImportPendingUpdates, "Character pending updates import should default off");
        passed &= Expect(options.SqlPath.empty(), "Character setup SQL path should default empty");
        passed &= Expect(options.BaseFileName == "characters_database.sql", "Character setup should use the characters base SQL file");
        passed &= Expect(options.UpdatesDirectory == "updates/characters", "Character setup should use the characters updates directory");
        passed &= Expect(options.PendingUpdatesDirectory == "pending_updates/characters",
            "Character setup should use the characters pending updates directory");

        return passed;
    }

    bool TestWorldDefaultsAreConservative()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeWorldDatabaseSetupOptions(false, false, "", "");

        bool passed = true;
        passed &= Expect(options.Domain == "world", "World setup domain should be world");
        passed &= Expect(!options.AutoSetup, "World auto setup should default off");
        passed &= Expect(!options.AutoCreate, "World auto create should default off");
        passed &= Expect(!options.AutoBaseline, "World auto baseline should default off");
        passed &= Expect(!options.AllowUpdateHashMismatch, "World update hash mismatch bypass should default off");
        passed &= Expect(!options.ImportPendingUpdates, "World pending updates import should default off");
        passed &= Expect(options.SqlPath.empty(), "World setup SQL path should default empty");
        passed &= Expect(options.BaseFileName.empty(), "World setup should not assume an in-tree base SQL file");
        passed &= Expect(options.ExternalBaseFile.empty(), "World external base SQL file should default empty");
        passed &= Expect(options.RequiredBaseFileNames.size() == 1, "World setup should require stored procedures after base install");
        passed &= Expect(options.RequiredBaseFileNames[0] == "stored_procs.sql",
            "World setup should use the in-tree stored procedures file");
        passed &= Expect(options.UpdatesDirectory == "updates/world", "World setup should use the world updates directory");
        passed &= Expect(options.PendingUpdatesDirectory == "pending_updates/world",
            "World setup should use the world pending updates directory");

        return passed;
    }

    bool TestExistingAuthDatabaseCanBaselineUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-02-03_auth_00.sql", "sql/updates/auth/2026-02-03_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Existing auth schema should allow explicit baseline mode");
        passed &= Expect(!plan.ShouldInstallBase, "Baseline mode should not reinstall base SQL");
        passed &= Expect(plan.ShouldBaselineUpdates, "Baseline mode should record update metadata");
        passed &= Expect(plan.PendingUpdates.empty(), "Baseline mode should not execute historical updates");
        passed &= Expect(plan.BaselineUpdates.size() == 2, "Baseline mode should record all discovered updates");

        return passed;
    }

    bool TestExistingAuthDatabaseBaselineRequiresHashes()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Baseline mode should reject updates without content hashes");
        passed &= Expect(!plan.Error.empty(), "Baseline mode should explain missing content hashes");
        passed &= Expect(!plan.ShouldBaselineUpdates, "Baseline mode should not continue without hashes");

        return passed;
    }

    bool TestSqlUpdatesAreFilteredAndSorted()
    {
        std::vector<std::string> names =
        {
            "updates go here.txt",
            "2026-01-23_auth_00.sql",
            "2025-12-01_auth_00.sql",
            "2026-02-03_auth_00.SQL"
        };

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
            Skyfire::Database::BuildSortedSqlUpdateList(names, "sql/updates/auth");

        bool passed = true;
        passed &= Expect(updates.size() == 3, "Only SQL update files should be kept");
        passed &= Expect(updates[0].Name == "2025-12-01_auth_00.sql", "Auth updates should be sorted by filename");
        passed &= Expect(updates[1].Name == "2026-01-23_auth_00.sql", "Auth updates should preserve middle sorted filename");
        passed &= Expect(updates[2].Name == "2026-02-03_auth_00.SQL", "Auth update filtering should be case insensitive");
        passed &= Expect(updates[0].Path == "sql/updates/auth/2025-12-01_auth_00.sql", "Auth update path should include the directory");

        return passed;
    }

    bool TestEmptyAuthDatabaseInstallsBaseAndPendingUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth setup plan should be valid when base SQL exists");
        passed &= Expect(!plan.ShouldCreateDatabase, "Existing auth database should not be created");
        passed &= Expect(plan.ShouldInstallBase, "Empty auth database should install base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Empty auth database should apply pending auth updates");

        return passed;
    }

    bool TestExistingAuthDatabaseChecksUpdatesWhenAutoSetupIsDisabled()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(false, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "hash-a";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-06-26_auth_00.sql", "sql/updates/auth/2026-06-26_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth update check should run even when AutoSetup is disabled");
        passed &= Expect(!plan.ShouldCreateDatabase, "Auth update check should not create databases when AutoSetup is disabled");
        passed &= Expect(!plan.ShouldInstallBase, "Auth update check should not install base SQL when AutoSetup is disabled");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Auth update check should queue unapplied updates when AutoSetup is disabled");
        passed &= Expect(plan.PendingUpdates[0].Name == "2026-06-26_auth_00.sql",
            "Auth update check should keep the unapplied update when AutoSetup is disabled");

        return passed;
    }

    bool TestEmptyAuthDatabaseRequiresAutoSetupForBaseInstall()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(false, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, {});

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Empty auth database should not be installed when AutoSetup is disabled");
        passed &= Expect(!plan.Error.empty(), "Empty auth database should explain that AutoSetup is required");
        passed &= Expect(!plan.ShouldInstallBase, "Disabled AutoSetup should not install auth base SQL");

        return passed;
    }

    bool TestExistingAuthDatabaseCanBaselineUpdatesWhenAutoSetupIsDisabled()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(false, false, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-06-26_auth_00.sql", "sql/updates/auth/2026-06-26_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth baseline should be available when AutoSetup is disabled");
        passed &= Expect(!plan.ShouldInstallBase, "Auth baseline should not install base SQL when AutoSetup is disabled");
        passed &= Expect(plan.ShouldBaselineUpdates, "Auth baseline should record discovered updates when AutoSetup is disabled");
        passed &= Expect(plan.BaselineUpdates.size() == 2, "Auth baseline should include all discovered updates when AutoSetup is disabled");

        return passed;
    }

    bool TestEmptyCharacterDatabaseInstallsBase()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeCharacterDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildCharacterDatabaseSetupPlan(options, state, true, {});

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Character setup plan should be valid when base SQL exists");
        passed &= Expect(!plan.ShouldCreateDatabase, "Existing character database should not be created");
        passed &= Expect(plan.ShouldInstallBase, "Empty character database should install base SQL");
        passed &= Expect(plan.PendingUpdates.empty(), "Empty character database with no updates should not queue updates");

        return passed;
    }

    bool TestEmptyWorldDatabaseRequiresExternalBase()
    {
        Skyfire::Database::SetupOptions options =
            Skyfire::Database::MakeWorldDatabaseSetupOptions(true, false, "sql", "");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        Skyfire::Database::SetupPlan missingPlan =
            Skyfire::Database::BuildWorldDatabaseSetupPlan(options, state, false, true, {});

        options.ExternalBaseFile = "D:/SkyFire/world_database.sql";
        Skyfire::Database::SetupPlan validPlan =
            Skyfire::Database::BuildWorldDatabaseSetupPlan(options, state, true, true, {});

        bool passed = true;
        passed &= Expect(!missingPlan.IsValid(), "Empty world database should require the external base dump");
        passed &= Expect(!missingPlan.Error.empty(), "Missing world base dump should explain why setup stopped");
        passed &= Expect(validPlan.IsValid(), "Empty world database should be valid when base and stored procedures exist");
        passed &= Expect(validPlan.ShouldInstallBase, "Empty world database should install base SQL");

        return passed;
    }

    bool TestExistingCharacterDatabaseChecksUpdatesWhenAutoSetupIsDisabled()
    {
        Skyfire::Database::SetupOptions options =
            Skyfire::Database::MakeCharacterDatabaseSetupOptions(false, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 42;
        state.UpdateTrackingExists = true;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-06-26_characters_00.sql", "sql/updates/characters/2026-06-26_characters_00.sql", "hash-a" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildCharacterDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Character update check should run even when AutoSetup is disabled");
        passed &= Expect(!plan.ShouldInstallBase, "Character update check should not install base SQL when AutoSetup is disabled");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Character update check should queue unapplied updates when AutoSetup is disabled");

        return passed;
    }

    bool TestExistingWorldDatabaseChecksUpdatesWhenAutoSetupIsDisabled()
    {
        Skyfire::Database::SetupOptions options =
            Skyfire::Database::MakeWorldDatabaseSetupOptions(false, false, "sql", "");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 1200;
        state.UpdateTrackingExists = true;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-06-22_world_00.sql", "sql/updates/world/2026-06-22_world_00.sql", "hash-a" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildWorldDatabaseSetupPlan(options, state, false, false, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "World update check should run even when AutoSetup is disabled");
        passed &= Expect(!plan.ShouldInstallBase, "World update check should not install base SQL when AutoSetup is disabled");
        passed &= Expect(plan.PendingUpdates.size() == 1, "World update check should queue unapplied updates when AutoSetup is disabled");

        return passed;
    }

    bool TestEmptyWorldDatabaseRequiresStoredProcedures()
    {
        Skyfire::Database::SetupOptions options =
            Skyfire::Database::MakeWorldDatabaseSetupOptions(true, false, "sql", "D:/SkyFire/world_database.sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 0;

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildWorldDatabaseSetupPlan(options, state, true, false, {});

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Empty world database should require stored procedures");
        passed &= Expect(!plan.Error.empty(), "Missing stored procedures should explain why setup stopped");

        return passed;
    }

    bool TestExistingWorldDatabaseKeepsAutoSetupEnabled()
    {
        Skyfire::Database::SetupOptions options =
            Skyfire::Database::MakeWorldDatabaseSetupOptions(true, true, "sql", "");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 1200;
        state.UpdateTrackingExists = true;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-06-22_world_00.sql", "sql/updates/world/2026-06-22_world_00.sql", "hash-a" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildWorldDatabaseSetupPlan(options, state, false, false, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Existing world database should keep running when AutoSetup remains enabled");
        passed &= Expect(!plan.ShouldCreateDatabase, "Existing world database should not be created when AutoCreate remains enabled");
        passed &= Expect(!plan.ShouldInstallBase, "Existing world database should not reinstall base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Existing world database should still queue unapplied updates");

        return passed;
    }

    bool TestExistingAuthDatabaseKeepsAutoCreateEnabled()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, true, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "hash-a";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-06-26_auth_00.sql", "sql/updates/auth/2026-06-26_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Existing auth database should keep running when AutoSetup and AutoCreate remain enabled");
        passed &= Expect(!plan.ShouldCreateDatabase, "Existing auth database should not be created when AutoCreate remains enabled");
        passed &= Expect(!plan.ShouldInstallBase, "Existing auth database should not reinstall base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Existing auth database should still queue unapplied updates");

        return passed;
    }

    bool TestExistingAuthDatabaseSkipsAppliedUpdates()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "hash-a";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "hash-a" },
            { "2026-02-03_auth_00.sql", "sql/updates/auth/2026-02-03_auth_00.sql", "hash-b" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Auth setup plan should be valid for existing auth schema");
        passed &= Expect(!plan.ShouldInstallBase, "Existing auth schema should not reinstall base SQL");
        passed &= Expect(plan.PendingUpdates.size() == 1, "Existing auth schema should skip applied updates");
        passed &= Expect(plan.PendingUpdates[0].Name == "2026-02-03_auth_00.sql", "Existing auth schema should keep unapplied updates");

        return passed;
    }

    bool TestExistingAuthDatabaseRejectsChangedAppliedUpdate()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "old-hash";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "new-hash" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Changed applied auth update should fail version control validation");
        passed &= Expect(!plan.Error.empty(), "Changed applied auth update should explain the hash mismatch");
        passed &= Expect(plan.PendingUpdates.empty(), "Changed applied auth update should not be queued");

        return passed;
    }

    bool TestExistingAuthDatabaseCanBypassChangedAppliedUpdate()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");
        options.AllowUpdateHashMismatch = true;

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = true;
        state.AppliedUpdates.insert("2026-01-23_auth_00.sql");
        state.AppliedUpdateHashes["2026-01-23_auth_00.sql"] = "old-hash";

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql", "new-hash" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(plan.IsValid(), "Changed applied auth update should be bypassed when explicitly allowed");
        passed &= Expect(plan.PendingUpdates.empty(), "Bypassed changed applied update should not be queued again");
        passed &= Expect(plan.HashMismatchedUpdates.size() == 1, "Bypassed changed applied update should be reported");
        passed &= Expect(plan.HashMismatchedUpdates[0].Name == "2026-01-23_auth_00.sql",
            "Bypassed changed applied update should retain the update name");

        return passed;
    }

    bool TestExistingAuthDatabaseWithoutTrackingFailsSafe()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = true;
        state.SchemaTableCount = 12;
        state.UpdateTrackingExists = false;

        std::vector<Skyfire::Database::SqlUpdateFile> updates =
        {
            { "2026-01-23_auth_00.sql", "sql/updates/auth/2026-01-23_auth_00.sql" }
        };

        Skyfire::Database::SetupPlan plan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, updates);

        bool passed = true;
        passed &= Expect(!plan.IsValid(), "Existing auth schema without tracking should fail safe");
        passed &= Expect(!plan.Error.empty(), "Missing tracking table plan should explain why setup stopped");
        passed &= Expect(plan.PendingUpdates.empty(), "Missing tracking table plan should not queue updates");

        return passed;
    }

    bool TestMissingDatabaseRequiresAutoCreate()
    {
        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, "sql");

        Skyfire::Database::SetupState state;
        state.DatabaseExists = false;

        Skyfire::Database::SetupPlan blockedPlan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, {});

        options.AutoCreate = true;
        Skyfire::Database::SetupPlan createPlan =
            Skyfire::Database::BuildAuthDatabaseSetupPlan(options, state, true, {});

        bool passed = true;
        passed &= Expect(!blockedPlan.IsValid(), "Missing auth database should fail when auto create is disabled");
        passed &= Expect(!blockedPlan.Error.empty(), "Blocked auth setup plan should explain the missing database");
        passed &= Expect(createPlan.IsValid(), "Missing auth database should be valid when auto create is enabled");
        passed &= Expect(createPlan.ShouldCreateDatabase, "Missing auth database should be created when auto create is enabled");
        passed &= Expect(createPlan.ShouldInstallBase, "New auth database should install base SQL");

        return passed;
    }

    bool TestSqlUpdatesAreDiscoveredFromConfiguredRoot()
    {
        std::filesystem::path root = std::filesystem::temp_directory_path() / "skyfire_database_setup_tests";
        std::filesystem::path updatesDir = root / "updates" / "auth";
        std::filesystem::path pendingDir = root / "pending_updates" / "auth";

        std::filesystem::remove_all(root);
        std::filesystem::create_directories(updatesDir);
        std::filesystem::create_directories(pendingDir);

        std::ofstream(updatesDir / "2026-03-01_auth_00.sql") << "SELECT 1;";
        std::ofstream(updatesDir / "readme.txt") << "notes";
        std::ofstream(updatesDir / "2026-01-23_auth_00.sql") << "SELECT 2;";
        std::ofstream(pendingDir / "pending_auth_fix.sql") << "SELECT 3;";
        std::ofstream(pendingDir / "2026-03-01_auth_00.sql") << "SELECT 4;";

        Skyfire::Database::SetupOptions options = Skyfire::Database::MakeAuthDatabaseSetupOptions(true, false, root.string());
        std::vector<Skyfire::Database::SqlUpdateFile> updates =
            Skyfire::Database::DiscoverSqlUpdates(options);

        bool passed = true;
        passed &= Expect(updates.size() == 2, "Auth discovery should keep SQL files only");
        passed &= Expect(updates[0].Name == "2026-01-23_auth_00.sql", "Auth discovery should sort update filenames");
        passed &= Expect(updates[1].Name == "2026-03-01_auth_00.sql", "Auth discovery should preserve later sorted update");
        passed &= ExpectEqual(updates[0].Path, (updatesDir / "2026-01-23_auth_00.sql").string(),
            "Auth discovery should return full configured update paths");
        passed &= Expect(updates[0].Hash == Skyfire::Database::CalculateStableSqlHash("SELECT 2;"),
            "Auth discovery should hash update file contents for version control");

        options.ImportPendingUpdates = true;
        updates = Skyfire::Database::DiscoverSqlUpdates(options);

        passed &= Expect(updates.size() == 3, "Auth discovery should include pending updates when enabled");
        passed &= Expect(updates[0].Name == "2026-01-23_auth_00.sql", "Pending import should keep sorted update filenames");
        passed &= Expect(updates[1].Name == "2026-03-01_auth_00.sql", "Pending import should prefer updates over pending duplicates");
        passed &= ExpectEqual(updates[1].Path, (updatesDir / "2026-03-01_auth_00.sql").string(),
            "Pending import should keep the updates-directory path for duplicate names");
        passed &= Expect(updates[2].Name == "pending_auth_fix.sql", "Pending import should discover pending-only SQL files");
        passed &= ExpectEqual(updates[2].Path, (pendingDir / "pending_auth_fix.sql").string(),
            "Pending import should return full pending update paths");
        passed &= Expect(updates[2].Hash == Skyfire::Database::CalculateStableSqlHash("SELECT 3;"),
            "Pending import should hash pending update file contents");

        std::filesystem::remove_all(root);

        return passed;
    }

    bool TestSqlScriptSplitsStatementsSafely()
    {
        std::string sql =
            "-- setup auth database\n"
            "CREATE TABLE `account` (`name` varchar(32));\n"
            "INSERT INTO `account` (`name`) VALUES ('semi;colon'), (\"double;semi\");\n"
            "/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;\n";

        std::vector<std::string> statements = Skyfire::Database::SplitSqlStatements(sql);

        bool passed = true;
        passed &= Expect(statements.size() == 3, "SQL splitter should return three executable statements");
        passed &= Expect(statements[0].find("CREATE TABLE") != std::string::npos,
            "SQL splitter should keep the first CREATE statement");
        passed &= Expect(statements[1].find("'semi;colon'") != std::string::npos,
            "SQL splitter should not split semicolons inside single quotes");
        passed &= Expect(statements[1].find("\"double;semi\"") != std::string::npos,
            "SQL splitter should not split semicolons inside double quotes");
        passed &= Expect(statements[2].find("/*!40101 SET SQL_MODE=@OLD_SQL_MODE */") != std::string::npos,
            "SQL splitter should keep MySQL conditional comments as statements");

        return passed;
    }

    bool TestSqlScriptIgnoresQuotesInsideComments()
    {
        std::string sql =
            "-- Deferred IDs include Kor'kron and Y'Shaarj entries\n"
            "DELETE FROM `creature_loot_template`\n"
            "WHERE `entry` IN (71454);\n"
            "-- 71454 - Malkorok\n"
            "INSERT INTO `creature_loot_template` (`entry`, `item`) VALUES (71454, 102303);\n";

        std::vector<std::string> statements = Skyfire::Database::SplitSqlStatements(sql);

        bool passed = true;
        passed &= Expect(statements.size() == 2, "SQL splitter should ignore apostrophes inside line comments");
        passed &= Expect(statements[0].find("DELETE FROM `creature_loot_template`") != std::string::npos,
            "SQL splitter should keep the DELETE statement after an apostrophe comment");
        passed &= Expect(statements[1].find("INSERT INTO `creature_loot_template`") != std::string::npos,
            "SQL splitter should split the INSERT after an apostrophe comment");

        return passed;
    }

    bool TestSqlScriptSplitsDelimiterStatements()
    {
        std::string sql =
            "CREATE TABLE `world_test` (`id` int);\n"
            "DELIMITER ;;\n"
            "CREATE PROCEDURE `sp_world_test`()\n"
            "BEGIN\n"
            "  SELECT 'semi;colon';\n"
            "END ;;\n"
            "DELIMITER ;\n"
            "CALL `sp_world_test`();\n";

        std::vector<std::string> statements = Skyfire::Database::SplitSqlStatements(sql);

        bool passed = true;
        passed &= Expect(statements.size() == 3, "SQL splitter should honor custom DELIMITER commands");
        passed &= Expect(statements[0].find("CREATE TABLE") != std::string::npos,
            "Delimiter-aware splitter should keep the first table statement");
        passed &= Expect(statements[1].find("CREATE PROCEDURE") != std::string::npos,
            "Delimiter-aware splitter should keep the procedure body as one statement");
        passed &= Expect(statements[1].find("'semi;colon'") != std::string::npos,
            "Delimiter-aware splitter should ignore semicolons inside procedure strings");
        passed &= Expect(statements[2].find("CALL `sp_world_test`()") != std::string::npos,
            "Delimiter-aware splitter should return to semicolon splitting after DELIMITER reset");

        return passed;
    }

    bool TestSqlScriptExecutorStopsOnFailure()
    {
        std::vector<std::string> executed;
        bool result = Skyfire::Database::ExecuteSqlScript("SELECT 1; SELECT 2; SELECT 3;",
            [&executed](std::string const& statement)
            {
                executed.push_back(statement);
                return statement != "SELECT 2";
            });

        bool passed = true;
        passed &= Expect(!result, "SQL script execution should fail when a statement fails");
        passed &= Expect(executed.size() == 2, "SQL script execution should stop after the failed statement");
        passed &= Expect(executed[0] == "SELECT 1", "SQL script execution should run statements in order");
        passed &= Expect(executed[1] == "SELECT 2", "SQL script execution should include the failed statement");

        return passed;
    }

    bool TestStableSqlHash()
    {
        bool passed = true;
        passed &= Expect(Skyfire::Database::CalculateStableSqlHash("") == "cbf29ce484222325",
            "Stable SQL hash should use FNV-1a offset for empty content");
        passed &= Expect(Skyfire::Database::CalculateStableSqlHash("hello") == "a430d84680aabd0b",
            "Stable SQL hash should use deterministic FNV-1a");

        return passed;
    }

    bool TestSqlStringEscaping()
    {
        bool passed = true;
        passed &= Expect(Skyfire::Database::EscapeSqlString("plain") == "plain",
            "SQL string escaping should leave plain text unchanged");
        passed &= Expect(Skyfire::Database::EscapeSqlString("can't\\stop") == "can\\'t\\\\stop",
            "SQL string escaping should escape quotes and backslashes");

        return passed;
    }

    bool TestDbUpdateAuditSql()
    {
        bool passed = true;
        std::string createSql = Skyfire::Database::BuildDbUpdateAuditTableSql();
        std::string insertSql = Skyfire::Database::BuildDbUpdateAuditInsertSql("2026-06-21_world.sql");

        passed &= Expect(createSql.find("CREATE TABLE IF NOT EXISTS `db_update`") != std::string::npos,
            "DB update audit table SQL should create db_update");
        passed &= Expect(createSql.find("`date` date NOT NULL") != std::string::npos,
            "DB update audit table SQL should include a date column");
        passed &= Expect(createSql.find("`time` time NOT NULL") != std::string::npos,
            "DB update audit table SQL should include a time column");
        passed &= Expect(createSql.find("`filename` varchar(255) NOT NULL") != std::string::npos,
            "DB update audit table SQL should include a filename column");
        passed &= Expect(insertSql.find("INSERT INTO `db_update`") != std::string::npos,
            "DB update audit insert should target db_update");
        passed &= Expect(insertSql.find("CURDATE()") != std::string::npos,
            "DB update audit insert should record the current date");
        passed &= Expect(insertSql.find("CURTIME()") != std::string::npos,
            "DB update audit insert should record the current time");
        passed &= Expect(insertSql.find("'2026-06-21_world.sql'") != std::string::npos,
            "DB update audit insert should record the update filename");

        return passed;
    }

    bool TestDatabaseSetupRuntimeSqlHelpers()
    {
        bool passed = true;
        std::string createDatabaseSql = Skyfire::Database::BuildCreateDatabaseSql("sky`fire_auth");
        std::string trackingTableSql = Skyfire::Database::BuildUpdateTrackingTableSql();
        std::string insertTrackingSql = Skyfire::Database::BuildUpdateTrackingInsertSql(
            "world", "2026-06-21_world's.sql", "hash\\value");

        passed &= Expect(createDatabaseSql == "CREATE DATABASE IF NOT EXISTS `sky``fire_auth` DEFAULT CHARACTER SET utf8",
            "Database creation SQL should escape MySQL identifiers");
        passed &= Expect(trackingTableSql.find("CREATE TABLE IF NOT EXISTS `skyfire_db_updates`") != std::string::npos,
            "Update tracking table SQL should create skyfire_db_updates");
        passed &= Expect(trackingTableSql.find("PRIMARY KEY (`domain`,`filename`)") != std::string::npos,
            "Update tracking table SQL should key by domain and filename");
        passed &= Expect(insertTrackingSql.find("'world'") != std::string::npos,
            "Update tracking insert should record the setup domain");
        passed &= Expect(insertTrackingSql.find("'2026-06-21_world\\'s.sql'") != std::string::npos,
            "Update tracking insert should escape filenames");
        passed &= Expect(insertTrackingSql.find("'hash\\\\value'") != std::string::npos,
            "Update tracking insert should escape hashes");

        return passed;
    }

    bool TestSetupPlanSummary()
    {
        Skyfire::Database::SetupPlan updatePlan;
        updatePlan.PendingUpdates.push_back({ "2026-06-21_auth.sql", "sql/updates/auth/2026-06-21_auth.sql", "hash-a" });

        Skyfire::Database::SetupPlan worldInstallPlan;
        worldInstallPlan.ShouldInstallBase = true;
        worldInstallPlan.PendingUpdates.push_back({ "2026-06-21_world.sql", "sql/updates/world/2026-06-21_world.sql", "hash-b" });

        Skyfire::Database::SetupPlan baselinePlan;
        baselinePlan.ShouldBaselineUpdates = true;
        baselinePlan.BaselineUpdates.push_back({ "2026-06-21_characters.sql", "sql/updates/characters/2026-06-21_characters.sql", "hash-c" });

        bool passed = true;
        passed &= ExpectEqual(
            Skyfire::Database::BuildSetupPlanSummary("Auth", updatePlan, 2, false),
            "Auth database setup plan: mode=apply-updates, discovered updates=2, pending updates=1, baseline updates=0, hash mismatch bypasses=0, install base=no, required SQL=no.",
            "Setup plan summary should describe pending update mode");
        passed &= ExpectEqual(
            Skyfire::Database::BuildSetupPlanSummary("World", worldInstallPlan, 3, true),
            "World database setup plan: mode=install-base, discovered updates=3, pending updates=1, baseline updates=0, hash mismatch bypasses=0, install base=yes, required SQL=yes.",
            "Setup plan summary should describe base install mode and required SQL");
        passed &= ExpectEqual(
            Skyfire::Database::BuildSetupPlanSummary("Character", baselinePlan, 1, false),
            "Character database setup plan: mode=baseline, discovered updates=1, pending updates=0, baseline updates=1, hash mismatch bypasses=0, install base=no, required SQL=no.",
            "Setup plan summary should describe baseline mode");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestAuthDefaultsAreConservative();
    passed &= TestCharacterDefaultsAreConservative();
    passed &= TestWorldDefaultsAreConservative();
    passed &= TestSqlUpdatesAreFilteredAndSorted();
    passed &= TestEmptyAuthDatabaseInstallsBaseAndPendingUpdates();
    passed &= TestExistingAuthDatabaseChecksUpdatesWhenAutoSetupIsDisabled();
    passed &= TestEmptyAuthDatabaseRequiresAutoSetupForBaseInstall();
    passed &= TestExistingAuthDatabaseCanBaselineUpdatesWhenAutoSetupIsDisabled();
    passed &= TestEmptyCharacterDatabaseInstallsBase();
    passed &= TestExistingCharacterDatabaseChecksUpdatesWhenAutoSetupIsDisabled();
    passed &= TestEmptyWorldDatabaseRequiresExternalBase();
    passed &= TestExistingWorldDatabaseChecksUpdatesWhenAutoSetupIsDisabled();
    passed &= TestEmptyWorldDatabaseRequiresStoredProcedures();
    passed &= TestExistingWorldDatabaseKeepsAutoSetupEnabled();
    passed &= TestExistingAuthDatabaseKeepsAutoCreateEnabled();
    passed &= TestExistingAuthDatabaseSkipsAppliedUpdates();
    passed &= TestExistingAuthDatabaseRejectsChangedAppliedUpdate();
    passed &= TestExistingAuthDatabaseCanBypassChangedAppliedUpdate();
    passed &= TestExistingAuthDatabaseWithoutTrackingFailsSafe();
    passed &= TestExistingAuthDatabaseCanBaselineUpdates();
    passed &= TestExistingAuthDatabaseBaselineRequiresHashes();
    passed &= TestMissingDatabaseRequiresAutoCreate();
    passed &= TestSqlUpdatesAreDiscoveredFromConfiguredRoot();
    passed &= TestSqlScriptSplitsStatementsSafely();
    passed &= TestSqlScriptIgnoresQuotesInsideComments();
    passed &= TestSqlScriptSplitsDelimiterStatements();
    passed &= TestSqlScriptExecutorStopsOnFailure();
    passed &= TestStableSqlHash();
    passed &= TestSqlStringEscaping();
    passed &= TestDbUpdateAuditSql();
    passed &= TestDatabaseSetupRuntimeSqlHelpers();
    passed &= TestSetupPlanSummary();

    return passed ? 0 : 1;
}
