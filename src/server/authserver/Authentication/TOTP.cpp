/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "TOTP.h"

#include "openssl/evp.h"
#include "openssl/hmac.h"

#include <cstring>

int base32_decode(std::string& encoded, char* result, int bufSize)
{
    // Base32 implementation
    // Copyright 2010 Google Inc.
    // Author: Markus Gutschke
    // Licensed under the Apache License, Version 2.0
    int buffer = 0;
    int bitsLeft = 0;
    int count = 0;
    for (const char* ptr = encoded.c_str(); count < bufSize && *ptr; ++ptr)
    {
        char ch = *ptr;
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-')
            continue;
        buffer <<= 5;

        // Deal with commonly mistyped characters
        if (ch == '0')
            ch = 'O';
        else if (ch == '1')
            ch = 'L';
        else if (ch == '8')
            ch = 'B';

        // Look up one base32 digit
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            ch = (ch & 0x1F) - 1;
        else if (ch >= '2' && ch <= '7')
            ch -= '2' - 26;
        else
            return -1;

        buffer |= ch;
        bitsLeft += 5;
        if (bitsLeft >= 8)
        {
            result[count++] = buffer >> (bitsLeft - 8);
            bitsLeft -= 8;
        }
    }

    if (count < bufSize)
        result[count] = '\000';
    return count;
}

#define HMAC_RES_SIZE 20

namespace TOTP
{
    unsigned int GenerateToken(std::string& b32key)
    {
        size_t keySize = b32key.length();
        int bufsize = (keySize + 7) / 8 * 5;
        char* encoded = new char[bufsize];
        memset(encoded, 0, bufsize);
        unsigned int hmacResSize = HMAC_RES_SIZE;
        unsigned char hmacRes[HMAC_RES_SIZE];
        unsigned long timestamp = time(NULL) / 30;
        unsigned char challenge[8];

        for (int i = 8; i--; timestamp >>= 8)
            challenge[i] = timestamp;

        base32_decode(b32key, encoded, bufsize);
        HMAC(EVP_sha1(), encoded, bufsize, challenge, 8, hmacRes, &hmacResSize);
        unsigned int offset = hmacRes[19] & 0xF;
        unsigned int truncHash = (hmacRes[offset] << 24) | (hmacRes[offset + 1] << 16) | (hmacRes[offset + 2] << 8) | (hmacRes[offset + 3]);
        truncHash &= 0x7FFFFFFF;

        delete[] encoded;

        return truncHash % 1000000;
    }
}
