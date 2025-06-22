#ifndef _AUTHDEFINES_H
#define _AUTHDEFINES_H

#include "Define.h"
#include <array>

constexpr size_t SESSION_KEY_LENGTH = 40;
using SessionKey = std::array<uint8, SESSION_KEY_LENGTH>;

#endif
