#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

file(GLOB_RECURSE sources_Commands Commands/*.cpp Commands/*.h)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ${sources_Commands}
)

message("  -> Prepared: Commands")
