# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
set(chatserver_sources Main.cpp ChatServer.cpp ChatServer.h)
if(WIN32)
  list(APPEND chatserver_sources ${sources_windows_Debugging} chatserver.rc chatserver.ico)
endif()
add_executable(chatserver ${chatserver_sources})
add_dependencies(chatserver revision.h)
target_include_directories(chatserver PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(chatserver shared ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
if(TARGET Boost::headers)
  target_link_libraries(chatserver Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(chatserver Boost::boost)
endif()
if(WIN32)
  skyfire_copy_windows_runtime_dependencies(chatserver)
  install(TARGETS chatserver RUNTIME DESTINATION ".")
  install(FILES chatserver.conf.dist DESTINATION ".")
else()
  target_compile_definitions(chatserver PRIVATE _SKYFIRE_CHAT_CONFIG="${CONF_DIR}/chatserver.conf")
  install(TARGETS chatserver RUNTIME DESTINATION bin)
  install(FILES chatserver.conf.dist DESTINATION ${CONF_DIR})
endif()
add_custom_command(TARGET chatserver POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/chatserver.conf.dist" "$<TARGET_FILE_DIR:chatserver>/chatserver.conf.dist"
  VERBATIM)
