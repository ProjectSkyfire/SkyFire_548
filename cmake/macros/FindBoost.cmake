 #
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

if( WIN32 )
  SET(Boost_INCLUDE_DIRS
    "C:/local/boost_1_91_0/"
    )
endif()

if(BOOST_ROOT)
  file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT_HINT)
elseif(DEFINED ENV{BOOST_ROOT})
  file(TO_CMAKE_PATH "$ENV{BOOST_ROOT}" BOOST_ROOT_HINT)
endif()

if(BOOST_ROOT_HINT)
  list(APPEND CMAKE_PREFIX_PATH "${BOOST_ROOT_HINT}")

  if(NOT Boost_DIR)
    file(GLOB BOOST_CONFIG_DIRS LIST_DIRECTORIES true
      "${BOOST_ROOT_HINT}/lib*/cmake/Boost-${BOOST_EXPECTED_VERSION}"
    )

    if(BOOST_CONFIG_DIRS)
      list(SORT BOOST_CONFIG_DIRS ORDER DESCENDING)
      list(GET BOOST_CONFIG_DIRS 0 Boost_DIR)
    endif()
  endif()
endif()
