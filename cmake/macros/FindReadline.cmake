# Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/
# Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# find Readline (terminal input library) includes and library
#
# READLINE_INCLUDE_DIR - where the directory containing the READLINE headers can be found
# READLINE_LIBRARY     - full path to the READLINE library
# READLINE_FOUND       - TRUE if READLINE was found

FIND_PATH(READLINE_INCLUDE_DIR readline/readline.h)
FIND_LIBRARY(READLINE_LIBRARY NAMES readline) 

IF (READLINE_LIBRARY)
    SET(READLINE_LIB_FOUND TRUE)
    MESSAGE(STATUS "Found Readline library: ${READLINE_LIBRARY}")
ELSE (READLINE_LIBRARY)
    SET(READLINE_LIB_FOUND FALSE)
    MESSAGE(FATAL_ERROR "** Readline library not found!\n**")
ENDIF (READLINE_LIBRARY)

IF (READLINE_INCLUDE_DIR)
    SET(READLINE_DIR_FOUND TRUE)
    MESSAGE(STATUS "Readline include dir is: ${READLINE_INCLUDE_DIR}")
ELSE (READLINE_INCLUDE_DIR)
    SET(READLINE_DIR_FOUND FALSE)
    MESSAGE(FATAL_ERROR "** Readline include dir not found!\n** ")
ENDIF (READLINE_INCLUDE_DIR)
    
IF (READLINE_DIR_FOUND AND READLINE_LIB_FOUND)
    SET(READLINE_FOUND TRUE)
    MESSAGE(STATUS "Found Readline library: ${READLINE_LIBRARY}")
    MESSAGE(STATUS "Include dir is: ${READLINE_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${READLINE_INCLUDE_DIR})
ELSE (READLINE_DIR_FOUND AND READLINE_LIB_FOUND)
    SET(READLINE_FOUND FALSE)
ENDIF (READLINE_DIR_FOUND AND READLINE_LIB_FOUND)
