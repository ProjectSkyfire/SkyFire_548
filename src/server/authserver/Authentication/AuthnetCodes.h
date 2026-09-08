/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETCODES_H
#define SF_AUTHNETCODES_H

#include "Define.h"

// Values below are read directly out of client build 18414's own tables,
// not inferred - see the authnet client login probe research note.

enum class AuthnetLoginState : uint32
{
    INITIALIZED = 0x00,
    CONNECTING = 0x01,
    HANDSHAKING = 0x02,
    AUTHENTICATING = 0x03,
    AUTHENTICATED = 0x04,
    FAILED = 0x05,
    DOWNLOADFILE = 0x06,
    FIRST_SECURITY = 0x07,
    PIN = 0x08,
    PIN_WAIT = 0x09,
    MATRIX = 0x0A,
    MATRIX_WAIT = 0x0B,
    TOKEN = 0x0C,
    TOKEN_WAIT = 0x0D,
    CHECKINGVERSIONS = 0x0E,
    RESPONSE_CONNECTED = 0x0F,
    DISCONNECTED = 0x10,
    SURVEY = 0x11,
    OUTOFBAND_CHALLENGE = 0x12,
    OUTOFBAND_CHALLENGE_WAIT = 0x13
};

// Client-side event queue message types, drained by the login object's
// main-thread pump. Only entries whose handler has an unambiguous
// meaning are named; the rest are deliberately left out rather than
// guessed at.
enum class AuthnetClientEvent : uint32
{
    CONNECTED = 0x07,
    SURVEY = 0x0E,
    GAME_ACCOUNT_SELECT = 0x0C,
    OUTOFBAND_CHALLENGE = 0x10,
    RECONNECT = 0x11
};

#endif
