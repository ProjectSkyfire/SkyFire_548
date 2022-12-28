#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

option(SERVERS            "Build worldserver and authserver"                            1)
option(SCRIPTS            "Build core with scripts included"                            1)
option(TOOLS              "Build map/vmap/mmap extraction/assembler tools"              0)
option(USE_SCRIPTPCH      "Use precompiled headers when compiling scripts"              1)
option(USE_COREPCH        "Use precompiled headers when compiling servers"              1)
option(WITH_WARNINGS      "Show all warnings during compile"                            0)
option(WITH_COREDEBUG     "Include additional debug-code in core"                       0)
option(WITHOUT_GIT        "Disable the GIT testing routines"                            0)
option(WITH_CXX_20_STD    "Use c++20 standard"                                          1)
option(WITH_CXX_DRAFT_STD "Use c++ draft standard"                                      0)

