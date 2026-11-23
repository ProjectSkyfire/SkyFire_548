#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Events/childrens_week.cpp
  Events/pilgrims_bounty.cpp
)

include(Events/darkmoonfaire/CMakeLists.txt)

message("  -> Prepared: Events")
