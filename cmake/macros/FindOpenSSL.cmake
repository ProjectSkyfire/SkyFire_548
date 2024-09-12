#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

# - Try to find the OpenSSL encryption library
# Once done this will define
#
#  OPENSSL_ROOT_DIR - Set this variable to the root installation of OpenSSL
#
# Read-Only variables:
#  OPENSSL_FOUND - system has the OpenSSL library
#  OPENSSL_INCLUDE_DIR - the OpenSSL include directory
#  OPENSSL_LIBRARIES - The libraries needed to use OpenSSL

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2006 Alexander Neundorf <neundorf@kde.org>
# Copyright 2009-2010 Mathieu Malaterre <mathieu.malaterre@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

# http://www.slproweb.com/products/Win32OpenSSL.html

IF(PLATFORM EQUAL 64)
  SET(_OPENSSL_ROOT_PATHS
    "C:/OpenSSL-Win64/"
	"C:/OpenSSL-Win64/OpenSSL-Win64/"
  )
ENDIF()

FIND_PATH(OPENSSL_ROOT_DIR
  NAMES
    include/openssl/ssl.h
  PATHS
    ${_OPENSSL_ROOT_PATHS}
)
MARK_AS_ADVANCED(OPENSSL_ROOT_DIR)

# Re-use the previous path:
FIND_PATH(OPENSSL_INCLUDE_DIR openssl/ssl.h
  ${OPENSSL_ROOT_DIR}/include
)

IF(WIN32 AND NOT CYGWIN)
  # MINGW should go here too

  IF(MSVC)
    # /MD and /MDd are the standard values - if someone wants to use
    # others, the libnames have to change here too
    # use also ssl and ssleay32 in debug as fallback for openssl < 0.9.8b
    # TODO: handle /MT and static lib
    # In Visual C++ naming convention each of these four kinds of Windows libraries has it's standard suffix:
    #   * MD for dynamic-release
    #   * MDd for dynamic-debug
    #   * MT for static-release
    #   * MTd for static-debug

    # Implementation details:
    # We are using the libraries located in the VC subdir instead of the parent directory eventhough :
    # libeay32MD.lib is identical to ../libeay32.lib, and
    # ssleay32MD.lib is identical to ../ssleay32.lib

    

    FIND_LIBRARY(OPENSSL_LIB_CRYPTO_DEBUG
      NAMES
        libcrypto libcrypto32MDd libcrypto32 libcrypto64MDd libcrypto64
      PATHS
        ${OPENSSL_ROOT_DIR}/lib
    )

    FIND_LIBRARY(OPENSSL_LIB_CRYPTO_RELEASE
      NAMES
        libcrypto libcrypto32MD libcrypto32 libcrypto64MD libcrypto64
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/
    )

    FIND_LIBRARY(OPENSSL_LIB_SSL_DEBUG
      NAMES
        libssl libssl32MDd libssl32 ssl libssl64MDd libssl64
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/
    )

    FIND_LIBRARY(OPENSSL_LIB_SSL_RELEASE
      NAMES
        libssl libssl32MD libssl32 ssl libssl libssl64MD libssl64
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/
    )

    if( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
      set( OPENSSL_LIBRARIES
        optimized ${OPENSSL_LIB_SSL_RELEASE} ${OPENSSL_LIB_CRYPTO_RELEASE}
        debug ${OPENSSL_LIB_SSL_DEBUG} ${OPENSSL_LIB_CRYPTO_DEBUG}
      )
    else()
      set( OPENSSL_LIBRARIES
        ${OPENSSL_LIB_SSL_RELEASE}
        ${OPENSSL_LIB_CRYPTO_RELEASE}
      )
    endif()

    MARK_AS_ADVANCED(OPENSSL_LIB_SSL_DEBUG OPENSSL_LIB_SSL_RELEASE OPENSSL_LIB_CRYPTO_DEBUG OPENSSL_LIB_CRYPTO_RELEASE)
  ELSEIF(MINGW)

    # same player, for MingW
    FIND_LIBRARY(OPENSSL_LIB_CRYPTO
      NAMES
        libcrypto
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/MinGW
    )

    FIND_LIBRARY(OPENSSL_LIB_SSL NAMES
      NAMES
        ssleay32
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/MinGW
    )

    MARK_AS_ADVANCED(OPENSSL_LIB_SSL OPENSSL_LIB_CRYPTO)

    set( OPENSSL_LIBRARIES
      ${OPENSSL_LIB_SSL}
      ${OPENSSL_LIB_CRYPTO}
    )
  ELSE(MSVC)
    # Not sure what to pick for -say- intel, let's use the toplevel ones and hope someone report issues:
    FIND_LIBRARY(OPENSSL_LIB_CRYPTO
      NAMES
        libcrypto
      PATHS
        ${OPENSSL_ROOT_DIR}/lib
        ${OPENSSL_ROOT_DIR}/lib/VC
    )

    FIND_LIBRARY(OPENSSL_LIB_SSL
      NAMES
        libssl
      PATHS
        ${OPENSSL_ROOT_DIR}/lib
        ${OPENSSL_ROOT_DIR}/lib/VC
    )
    MARK_AS_ADVANCED(OPENSSL_LIB_SSL OPENSSL_LIB_CRYPTO)

    SET( OPENSSL_LIBRARIES ${OPENSSL_LIB_SSL} ${OPENSSL_LIB_CRYPTO} )
  ENDIF(MSVC)
ELSE(WIN32 AND NOT CYGWIN)
  FIND_LIBRARY(OPENSSL_SSL_LIBRARIES NAMES ssl ssleay32 ssleay32MD)
  FIND_LIBRARY(OPENSSL_CRYPTO_LIBRARIES NAMES crypto)
  MARK_AS_ADVANCED(OPENSSL_CRYPTO_LIBRARIES OPENSSL_SSL_LIBRARIES)

  SET(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES})

ENDIF(WIN32 AND NOT CYGWIN)

IF (WIN32)
  FIND_FILE(OPENSSL_LIB_LEGACY
      NAMES
        legacy.dll
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/ossl-modules/
    )
ELSE(UNIX)
  FIND_FILE(OPENSSL_LIB_LEGACY
      NAMES
        legacy.so
      PATHS
        ${OPENSSL_ROOT_DIR}/lib/ossl-modules/
        ${OPENSSL_ROOT_DIR}/lib64/ossl-modules/
        ${OPENSSL_ROOT_DIR}/lib/x86_64-linux-gnu/ossl-modules/
    )
ENDIF()

IF (OPENSSL_LIB_LEGACY)
  message( STATUS "Found OpenSSL legacy library: ${OPENSSL_LIB_LEGACY}")
ELSE()
  message( FATAL_ERROR "Found OpenSSL legacy library: ${OPENSSL_LIB_LEGACY}")
ENDIF()
  
if (NOT OPENSSL_INCLUDE_DIR)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(OpenSSL DEFAULT_MSG
    OPENSSL_LIB_LEGACY
    OPENSSL_LIBRARIES
    OPENSSL_INCLUDE_DIR
  )
endif()

if (OPENSSL_INCLUDE_DIR)
  message( STATUS "Found OpenSSL library: ${OPENSSL_LIBRARIES}")
  message( STATUS "Found OpenSSL headers: ${OPENSSL_INCLUDE_DIR}")
  if (_OPENSSL_VERSION)
    set(OPENSSL_VERSION "${_OPENSSL_VERSION}")
  else (_OPENSSL_VERSION)
    file(STRINGS "${OPENSSL_INCLUDE_DIR}/openssl/opensslv.h" openssl_version_str
         REGEX "^.*define[\t ]+OPENSSL_VERSION_NUMBER[\t ]+0x[0-9][0-9][0-9][0-9][0-9][0-9].*")

    # The version number is encoded as 0xMNNFFPPS: major minor fix patch status
    # The status gives if this is a developer or prerelease and is ignored here.
    # Major, minor, and fix directly translate into the version numbers shown in
    # the string. The patch field translates to the single character suffix that
    # indicates the bug fix state, which 00 -> nothing, 01 -> a, 02 -> b and so
    # on.

    #string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-f])([0-9a-f][0-9a-f])([0-9a-f][0-9a-f])([0-9a-f][0-9a-f])([0-9a-f]).*$"
    #       "\\1;\\2;\\3;\\4;\\5" OPENSSL_VERSION_LIST "${openssl_version_str}")
    #list(GET OPENSSL_VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
    #list(GET OPENSSL_VERSION_LIST 1 OPENSSL_VERSION_MINOR)
    #list(GET OPENSSL_VERSION_LIST 2 OPENSSL_VERSION_FIX)
    #list(GET OPENSSL_VERSION_LIST 3 OPENSSL_VERSION_PATCH)

    #string(REGEX REPLACE "^0(.)" "\\1" OPENSSL_VERSION_MINOR "${OPENSSL_VERSION_MINOR}")
    #string(REGEX REPLACE "^0(.)" "\\1" OPENSSL_VERSION_FIX "${OPENSSL_VERSION_FIX}")

    #set(OPENSSL_VERSION "${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MINOR}.${OPENSSL_VERSION_FIX}${OPENSSL_VERSION_PATCH_STRING}")
  endif (_OPENSSL_VERSION)

  #include(EnsureVersion)
  #ENSURE_VERSION( "${OPENSSL_EXPECTED_VERSION}" "${OPENSSL_VERSION}" OPENSSL_VERSION_OK)
  #if (NOT OPENSSL_VERSION_OK)
      #message(FATAL_ERROR "SkyFire needs OpenSSL version ${OPENSSL_EXPECTED_VERSION} but found version ${OPENSSL_VERSION}")
  #endif()
endif (OPENSSL_INCLUDE_DIR)

MARK_AS_ADVANCED(OPENSSL_INCLUDE_DIR OPENSSL_LIBRARIES)
