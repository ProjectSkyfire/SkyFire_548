file(GLOB_RECURSE sources *.c)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  list(REMOVE_ITEM sources
    ${CMAKE_CURRENT_SOURCE_DIR}/argon2/opt.c)
else()
  list(REMOVE_ITEM sources
    ${CMAKE_CURRENT_SOURCE_DIR}/argon2/ref.c)
endif()

add_library(argon2 STATIC
  ${sources})

target_compile_definitions(argon2
  PRIVATE
    -DARGON2_NO_THREADS)

set_target_properties(argon2 PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(argon2
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(argon2
  PROPERTIES
    FOLDER
      "dep")
