# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
set(character_files characterserver.py database.py character_state.py wire.py metrics.py statements.json characterserver.toml.dist requirements.txt)
set(character_sources)
foreach(asset IN LISTS character_files)
  list(APPEND character_sources "${CMAKE_CURRENT_SOURCE_DIR}/${asset}")
endforeach()
list(APPEND character_sources "${CMAKE_SOURCE_DIR}/src/server/shared/Platform/hub_service.py")
add_custom_target(characterserver_assets
  COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:worldserver>/characterserver"
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${character_sources} "$<TARGET_FILE_DIR:worldserver>/characterserver"
  COMMENT "Updating character server assets" VERBATIM)
add_dependencies(worldserver characterserver_assets)
add_dependencies(hubserver characterserver_assets)
if(WIN32)
  install(FILES ${character_sources} DESTINATION characterserver)
else()
  install(FILES ${character_sources} DESTINATION bin/characterserver)
endif()
