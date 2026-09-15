#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

set(g3dlib_STAT_SRCS
  source/AABox.cpp
  source/Any.cpp
  source/AnyTableReader.cpp
  source/BinaryFormat.cpp
  source/BinaryInput.cpp
  source/BinaryOutput.cpp
  source/Box.cpp
  source/Capsule.cpp
  source/CollisionDetection.cpp
  source/CoordinateFrame.cpp
  source/Crypto.cpp
  source/Cylinder.cpp
  source/debugAssert.cpp
  source/FileSystem.cpp
  source/fileutils.cpp
  source/format.cpp
  source/g3dfnmatch.cpp
  source/g3dmath.cpp
  source/GThread.cpp
  source/Line.cpp
  source/LineSegment.cpp
  source/Log.cpp
  source/Matrix3.cpp
  source/Matrix4.cpp
  source/MemoryManager.cpp
  source/PhysicsFrame.cpp
  source/Plane.cpp
  source/prompt.cpp
  source/Quat.cpp
  source/Random.cpp
  source/Ray.cpp
  source/RegistryUtil.cpp
  source/Sphere.cpp
  source/stringutils.cpp
  source/System.cpp
  source/TextInput.cpp
  source/TextOutput.cpp
  source/Triangle.cpp
  source/uint128.cpp
  source/UprightFrame.cpp
  source/Vector2.cpp
  source/Vector3.cpp
  source/Vector4.cpp
)

add_library(g3dlib STATIC ${g3dlib_STAT_SRCS})

target_include_directories(g3dlib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(WIN32)
  target_include_directories(g3dlib PRIVATE ${CMAKE_SOURCE_DIR}/dep/zlib)
else()
  string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" G3DLITE_SYSTEM_PROCESSOR)
  if(G3DLITE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|arm)")
    target_include_directories(g3dlib PRIVATE ${SSE2NEON_INCLUDE_DIRS})
  endif()
endif()

target_link_libraries(g3dlib
  ${ZLIB_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)
