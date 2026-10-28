/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _DURATION_H_
#define _DURATION_H_

#include <chrono>

 /// Microseconds shorthand typedef.
typedef std::chrono::microseconds Microseconds;

/// Milliseconds shorthand typedef.
typedef std::chrono::milliseconds Milliseconds;

/// Seconds shorthand typedef.
typedef std::chrono::seconds Seconds;

/// Minutes shorthand typedef.
typedef std::chrono::minutes Minutes;

/// time_point shorthand typedefs
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::system_clock::time_point SystemTimePoint;

/// Hours shorthand typedef.
typedef std::chrono::hours Hours;

/// Makes std::chrono_literals globally available.
using namespace std::chrono_literals;

constexpr std::chrono::hours operator""_days(unsigned long long days)
{
    return std::chrono::hours(days * 24h);
}


#endif // _DURATION_H_
