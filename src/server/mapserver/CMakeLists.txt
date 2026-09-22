# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
set(mapserver_files mapserver.py map_common.py fetch_maps.py mapserver.toml.dist requirements.txt)
set(mapserver_sources)
foreach(asset IN LISTS mapserver_files)
  list(APPEND mapserver_sources "${CMAKE_CURRENT_SOURCE_DIR}/${asset}")
endforeach()
list(APPEND mapserver_sources "${CMAKE_SOURCE_DIR}/src/server/shared/Platform/hub_service.py")
list(APPEND mapserver_sources "${CMAKE_SOURCE_DIR}/src/server/shared/Platform/cluster_certificates.py")
add_custom_target(mapserver_assets
  COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:worldserver>/mapserver"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${mapserver_sources} "$<TARGET_FILE_DIR:worldserver>/mapserver"
  COMMENT "Updating mapserver assets" VERBATIM)
add_dependencies(worldserver mapserver_assets)
add_dependencies(hubserver mapserver_assets)
if(WIN32)
  install(FILES ${mapserver_sources} DESTINATION mapserver)
else()
  install(FILES ${mapserver_sources} DESTINATION bin/mapserver)
endif()
