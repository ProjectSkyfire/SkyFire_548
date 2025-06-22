/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_TYPEDEFS_H
#define SKYFIRESERVER_TYPEDEFS_H

#include "Common.h"

namespace G3D
{
    class Vector2;
    class Vector3;
    class Vector4;
}

namespace Movement
{
    using G3D::Vector2;
    using G3D::Vector3;
    using G3D::Vector4;

    inline uint32 SecToMS(float sec)
    {
        return static_cast<uint32>(sec * 1000.f);
    }

    inline float MSToSec(uint32 ms)
    {
        return ms / 1000.f;
    }

    float computeFallTime(float path_length, bool isSafeFall);
    float computeFallElevation(float t_passed, bool isSafeFall, float start_velocity = 0.0f);
    /*
    #ifndef static_assert
        #define CONCAT(x, y) CONCAT1 (x, y)
        #define CONCAT1(x, y) x##y
        #define static_assert(expr, msg) typedef char CONCAT(static_assert_failed_at_line_, __LINE__) [(expr) ? 1 : -1]
    #endif
    */
    template<class T, T limit>
    class counter
    {
    public:
        counter() { init(); }

        void Increase()
        {
            if (m_counter == limit)
                init();
            else
                ++m_counter;
        }

        T NewId() { Increase(); return m_counter; }
        T getCurrent() const { return m_counter; }

    private:
        void init() { m_counter = 0; }
        T m_counter;
    };

    typedef counter<uint32, 0xFFFFFFFF> UInt32Counter;

    extern double gravity;
    extern UInt32Counter splineIdGen;
    extern std::string MovementFlags_ToString(uint32 flags);
    extern std::string MovementFlagsExtra_ToString(uint32 flags);
}

#endif // SKYFIRESERVER_TYPEDEFS_H
