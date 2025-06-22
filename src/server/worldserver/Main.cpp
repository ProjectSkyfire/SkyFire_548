/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Skyfired Skyfire Daemon
/// @{
/// \file
#pragma comment (lib, "Crypt32")
#include <ace/Version.h>
#include <openssl/crypto.h>
#include <openssl/opensslv.h>
#include <openssl/provider.h>

#include "Common.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"

#include "Log.h"
#include "Master.h"
#include "World.h"

#ifndef _SKYFIRE_CORE_CONFIG
# define _SKYFIRE_CORE_CONFIG  "worldserver.conf"
#endif

#ifdef _WIN32
#include "ServiceWin32.h"
char serviceName[] = "worldserver";
char serviceLongName[] = "SkyFire world service";
char serviceDescription[] = "SkyFire World of Warcraft emulator world service";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

WorldDatabaseWorkerPool WorldDatabase;                      ///< Accessor to the world database
CharacterDatabaseWorkerPool CharacterDatabase;              ///< Accessor to the character database
LoginDatabaseWorkerPool LoginDatabase;                      ///< Accessor to the realm/login database

#ifndef _SKYFIRE_WORLD_DATABASE_HOST
# define _SKYFIRE_WORLD_DATABASE_HOST  ""
#endif

#ifndef _SKYFIRE_WORLD_DATABASE_PORT
# define _SKYFIRE_WORLD_DATABASE_PORT  ""
#endif

#ifndef _SKYFIRE_WORLD_DATABASE_USER
# define _SKYFIRE_WORLD_DATABASE_USER  ""
#endif

#ifndef _SKYFIRE_WORLD_DATABASE_PASS
# define _SKYFIRE_WORLD_DATABASE_PASS  ""
#endif

#ifndef _SKYFIRE_DATABASE_AUTH
# define _SKYFIRE_DATABASE_AUTH  ""
#endif

#ifndef _SKYFIRE_DATABASE_CHARACTERS
# define _SKYFIRE_DATABASE_CHARACTERS  ""
#endif

#ifndef _SKYFIRE_DATABASE_WORLD
# define _SKYFIRE_DATABASE_WORLD  ""
#endif

RealmNameMap realmNameStore;
uint32 realmID = -1;                                        ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    printf("Usage:\n");
    printf(" %s [<options>]\n");
    printf("    -c config_file                   use config_file as configuration file\n");
    printf("    --no_use_config_database_info    dont use database login info from config file\n");
    printf("    --db_host                        sets the database host, requires: --no_use_config_database_info\n");
    printf("    --db_port                        sets the database port, requires: --no_use_config_database_info\n");
    printf("    --db_user                        sets the database user, requires: --no_use_config_database_info\n");
    printf("    --db_password                    sets the database password, requires: --no_use_config_database_info\n");
    printf("    --db_auth                        sets the auth database, requires: --no_use_config_database_info\n");
    printf("    --db_chars                       sets the characters database, requires: --no_use_config_database_info\n");
    printf("    --db_world                       sets the world database, requires: --no_use_config_database_info\n");
#ifdef _WIN32
    printf("    Running as service functions:\n");
    printf("    --service                run as service\n");
    printf("    -s install               install service\n");
    printf("    -s uninstall             uninstall service\n");
#endif
}

/// Launch the Skyfire server
extern int main(int argc, char** argv)
{

    bool noUseConfigDatabaseInfo = 0;
    const char* db_Host = _SKYFIRE_WORLD_DATABASE_HOST;
    const char* db_Port = _SKYFIRE_WORLD_DATABASE_PORT;
    const char* db_User = _SKYFIRE_WORLD_DATABASE_USER;
    const char* db_Password = _SKYFIRE_WORLD_DATABASE_PASS;
    const char* authDB = _SKYFIRE_DATABASE_AUTH;
    const char* charactersDB = _SKYFIRE_DATABASE_CHARACTERS;
    const char* worldDB = _SKYFIRE_DATABASE_WORLD;

    ///- Command line parsing to get the configuration file name
    char const* cfg_file = _SKYFIRE_CORE_CONFIG;
    int c = 1;
    while (c < argc)
    {
        if (strcmp(argv[c], "--help") == 0)
        {
            usage(argv[0]);
            return 1;
        }

        if (strcmp(argv[c], "--no_use_config_database_info") == 0)
        {
            noUseConfigDatabaseInfo = argv[c];
        }

        if (noUseConfigDatabaseInfo == 1)
        {
            if (strcmp(argv[c], "--db_host") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_host option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                {

                    db_Host = argv[c];
                }
            }

            if (strcmp(argv[c], "--db_port") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_port option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    db_Port = argv[c];
            }

            if (strcmp(argv[c], "--db_user") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_user option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    db_User = argv[c];
            }

            if (strcmp(argv[c], "--db_password") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_password option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    db_Password = argv[c];
            }

            if (strcmp(argv[c], "--db_auth") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_auth option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    authDB = argv[c];
            }

            if (strcmp(argv[c], "--db_chars") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_chars option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    charactersDB = argv[c];
            }

            if (strcmp(argv[c], "--db_world") == 0)
            {
                if (++c >= argc)
                {
                    printf("Runtime-Error: --db_world option requires an input argument\n");
                    usage(argv[0]);
                    return 1;
                }
                else
                    worldDB = argv[c];
            }
        }

        if (!strcmp(argv[c], "-c"))
        {
            if (++c >= argc)
            {
                printf("Runtime-Error: -c option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            else
                cfg_file = argv[c];
        }

#ifdef _WIN32
        if (strcmp(argv[c], "-s") == 0) // Services
        {
            if (++c >= argc)
            {
                printf("Runtime-Error: -s option requires an input argument");
                usage(argv[0]);
                return 1;
            }

            if (strcmp(argv[c], "install") == 0)
            {
                if (WinServiceInstall())
                    printf("Installing service\n");
                return 1;
            }
            else if (strcmp(argv[c], "uninstall") == 0)
            {
                if (WinServiceUninstall())
                    printf("Uninstalling service\n");
                return 1;
            }
            else
            {
                printf("Runtime-Error: unsupported option %s", argv[c]);
                usage(argv[0]);
                return 1;
            }
        }

        if (strcmp(argv[c], "--service") == 0)
            WinServiceRun();
#endif
        ++c;
    }

    if (!sConfigMgr->LoadInitial(cfg_file))
    {
        printf("Invalid or missing configuration file : %s\n", cfg_file);
        printf("Verify that the file exists and has \'[worldserver]' written in the top of the file!\n");
        return 1;
    }

    SF_LOG_INFO("server.worldserver", "Using configuration file %s.", cfg_file);

    SF_LOG_INFO("server.worldserver", "Using SSL version: %s (library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));

    OSSL_PROVIDER *default_provider = OSSL_PROVIDER_try_load(NULL, "default", 1); 
    OSSL_PROVIDER* legacy_provider = OSSL_PROVIDER_try_load(NULL, "legacy", 1);
    
    if (legacy_provider == NULL)
    {
        SF_LOG_INFO("server.worldserver", "Failed loading legacy provider, Try to load legacy provider again.");
        legacy_provider = OSSL_PROVIDER_try_load(NULL, "legacy", 1);
    }

    SF_LOG_INFO("server.worldserver", "Loading default provider: (%s)", (default_provider == NULL || !OSSL_PROVIDER_available(NULL, "default")) ? "failed" : "succeeded");
    SF_LOG_INFO("server.worldserver", "Loading legacy provider: (%s)", (legacy_provider == NULL || !OSSL_PROVIDER_available(NULL, "legacy")) ? "failed" : "succeeded");

    // recheck 
    if (legacy_provider == NULL)
        return 1;

    OSSL_PROVIDER_unload(legacy_provider);

    SF_LOG_INFO("server.worldserver", "Using ACE version: %s", ACE_VERSION);

    if (noUseConfigDatabaseInfo == true)
    {
        sMaster->SetNoUseConfigDatabaseInfo(noUseConfigDatabaseInfo);
        sMaster->SetDatabaseHost(db_Host);
        sMaster->SetDatabasePort(db_Port);
        sMaster->SetDatabaseUser(db_User);
        sMaster->SetDatabasePassword(db_Password);
        sMaster->SetDatabaseAuth(authDB);
        sMaster->SetDatabaseChars(charactersDB);
        sMaster->SetDatabaseWorld(worldDB);
    }

    ///- and run the 'Master'
    /// @todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    int ret = sMaster->Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Skyfired

    return ret;
}

/// @}
