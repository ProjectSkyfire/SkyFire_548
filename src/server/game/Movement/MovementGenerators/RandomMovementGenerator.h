/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_RANDOMMOTIONGENERATOR_H
#define SKYFIRE_RANDOMMOTIONGENERATOR_H

#include "MovementGenerator.h"

template<class T>
class RandomMovementGenerator : public MovementGeneratorMedium< T, RandomMovementGenerator<T> >
{
    public:
        RandomMovementGenerator(float spawn_dist = 0.0f) : i_nextMoveTime(0), wander_distance(spawn_dist) { }

        void _setRandomLocation(T*);
        void DoInitialize(T*);
        void DoFinalize(T*);
        void DoReset(T*);
        bool DoUpdate(T*, const uint32);
        bool GetResetPos(T*, float& x, float& y, float& z);
        MovementGeneratorType GetMovementGeneratorType() { return RANDOM_MOTION_TYPE; }
    private:
        TimeTrackerSmall i_nextMoveTime;

        uint32 i_nextMove;
        float wander_distance;
};
#endif
