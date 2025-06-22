/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_MOVESPLINEINIT_ARGS_H
#define SKYFIRESERVER_MOVESPLINEINIT_ARGS_H

#include "MoveSplineFlag.h"
#include <G3D/Vector3.h>

class Unit;

namespace Movement
{
    typedef std::vector<Vector3> PointsArray;

    union FacingInfo
    {
        struct {
            float x, y, z;
        } f;
        uint64  target;
        float   angle;

        FacingInfo() : angle(0.0f) { f.x = 0.0f; f.y = 0.0f; f.z = 0.0f; }
    };

    struct MoveSplineInitArgs
    {
        MoveSplineInitArgs(size_t path_capacity = 16) : path_Idx_offset(0), velocity(0.f),
            parabolic_amplitude(0.f), time_perc(0.f), splineId(0), initialOrientation(0.f),
            HasVelocity(false), TransformForTransport(true)
        {
            path.reserve(path_capacity);
        }

        PointsArray path;
        FacingInfo facing;
        MoveSplineFlag flags;
        int32 path_Idx_offset;
        float velocity;
        float parabolic_amplitude;
        float time_perc;
        uint32 splineId;
        float initialOrientation;
        bool HasVelocity;
        bool TransformForTransport;

        /** Returns true to show that the arguments were configured correctly and MoveSpline initialization will succeed. */
        bool Validate(Unit* unit) const;

    private:
        bool _checkPathBounds() const;
    };
}

#endif // SKYFIRESERVER_MOVESPLINEINIT_ARGS_H
