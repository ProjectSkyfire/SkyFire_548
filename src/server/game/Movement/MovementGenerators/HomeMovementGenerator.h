/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HOMEMOVEMENTGENERATOR_H
#define SKYFIRE_HOMEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"

class Creature;

template < class T >
class HomeMovementGenerator;

template <>
class HomeMovementGenerator<Creature> : public MovementGeneratorMedium< Creature, HomeMovementGenerator<Creature> >
{
    public:

        HomeMovementGenerator() : arrived(false) { }
        ~HomeMovementGenerator() { }

        void DoInitialize(Creature*);
        void DoFinalize(Creature*);
        void DoReset(Creature*);
        bool DoUpdate(Creature*, const uint32);
        MovementGeneratorType GetMovementGeneratorType() { return HOME_MOTION_TYPE; }

    private:
        void _setTargetLocation(Creature*);
        bool arrived;
};
#endif
