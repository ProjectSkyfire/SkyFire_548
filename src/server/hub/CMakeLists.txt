#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See COPYRIGHT file for Copyright information
#

########### hubserver ###############

file(GLOB sources_Localdir *.cpp *.h)
file(GLOB_RECURSE web_assets web/*)
list(APPEND sources_Localdir HubAuthProxy.cpp HubAuthProxy.h HubClusterServer.cpp HubClusterServer.h HubProcessSupervisor.cpp HubProcessSupervisor.h HubWebServer.cpp HubWebServer.h)
list(REMOVE_DUPLICATES sources_Localdir)

source_group(CMake FILES CMakeLists.txt)
source_group(localdir FILES ${sources_Localdir})
source_group("Web Files" FILES ${web_assets})
set_source_files_properties(${web_assets} PROPERTIES HEADER_FILE_ONLY TRUE)

set(hubserver_SRCS
  ${hubserver_SRCS}
  ${sources_Localdir}
  ${web_assets}
)

if(WIN32)
  set(hubserver_SRCS
    ${hubserver_SRCS}
    ${sources_windows_Debugging}
  )
  if(MSVC)
    set(hubserver_SRCS
      ${hubserver_SRCS}
      hubserver.rc
    )
  endif()
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_executable(hubserver
  ${hubserver_SRCS}
)

add_dependencies(hubserver revision.h)

if(NOT WIN32)
  set_target_properties(hubserver PROPERTIES
    COMPILE_DEFINITIONS _SKYFIRE_HUB_CONFIG="${CONF_DIR}/hubserver.conf"
  )
endif()

target_link_libraries(hubserver
  shared
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

if(TARGET Boost::headers)
  target_link_libraries(hubserver Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(hubserver Boost::boost)
endif()

# Refresh assets even when only web files changed and hubserver needs no relink.
add_custom_target(hubserver_web
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/web"
    "$<TARGET_FILE_DIR:hubserver>/web"
  COMMENT "Updating hubserver web assets"
  VERBATIM
)
add_dependencies(hubserver hubserver_web)

if(WIN32)
  if(MSVC)
    skyfire_copy_windows_runtime_dependencies(hubserver)

    add_custom_command(TARGET hubserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hubserver.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
    )
  elseif(MINGW)
    add_custom_command(TARGET hubserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hubserver.conf.dist ${CMAKE_BINARY_DIR}/bin/
    )
  elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    skyfire_copy_windows_runtime_dependencies(hubserver)

    add_custom_command(TARGET hubserver
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/hubserver.conf.dist ${CMAKE_BINARY_DIR}/bin/
    )
  endif()
endif()

if(UNIX)
  install(TARGETS hubserver DESTINATION bin)
  install(FILES hubserver.conf.dist DESTINATION ${CONF_DIR})
  install(DIRECTORY web DESTINATION bin)
elseif(WIN32)
  install(TARGETS hubserver RUNTIME DESTINATION ".")
  install(FILES hubserver.conf.dist DESTINATION ".")
  install(DIRECTORY web DESTINATION ".")
endif()
