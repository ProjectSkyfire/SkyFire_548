/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Trinityd Skyfire Daemon
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

RealmNameMap realmNameStore;
uint32 realmID = -1;                                        ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    printf("Usage:\n");
    printf(" %s [<options>]\n", prog);
    printf("    -c config_file           use config_file as configuration file\n");
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
    ///- Command line parsing to get the configuration file name
    char const* cfg_file = _SKYFIRE_CORE_CONFIG;
    int c = 1;
    while (c < argc)
    {
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

    OSSL_PROVIDER* legacy_provider = OSSL_PROVIDER_load(NULL, "legacy");
    SF_LOG_INFO("server.authserver", "Loading legacy provider: (%s)", (legacy_provider == NULL || !OSSL_PROVIDER_available(NULL, "legacy")) ? "failed" : "succeeded");

    if (legacy_provider == NULL)
        return 1;
        
    OSSL_PROVIDER_unload(legacy_provider);

    SF_LOG_INFO("server.worldserver", "Using ACE version: %s", ACE_VERSION);

    ///- and run the 'Master'
    /// @todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    int ret = sMaster->Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Trinityd

    return ret;
}

/// @}
