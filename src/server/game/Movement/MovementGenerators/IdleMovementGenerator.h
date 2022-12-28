/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_IDLEMOVEMENTGENERATOR_H
#define SKYFIRE_IDLEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"

class IdleMovementGenerator : public MovementGenerator
{
    public:

        void Initialize(Unit*);
        void Finalize(Unit*) {  }
        void Reset(Unit*);
        bool Update(Unit*, uint32) { return true; }
        MovementGeneratorType GetMovementGeneratorType() { return IDLE_MOTION_TYPE; }
};

extern IdleMovementGenerator si_idleMovement;

class RotateMovementGenerator : public MovementGenerator
{
    public:
        explicit RotateMovementGenerator(uint32 time, RotateDirection direction) : m_duration(time), m_maxDuration(time), m_direction(direction) { }

        void Initialize(Unit*);
        void Finalize(Unit*);
        void Reset(Unit* owner) { Initialize(owner); }
        bool Update(Unit*, uint32);
        MovementGeneratorType GetMovementGeneratorType() { return ROTATE_MOTION_TYPE; }

    private:
        uint32 m_duration, m_maxDuration;
        RotateDirection m_direction;
};

class DistractMovementGenerator : public MovementGenerator
{
    public:
        explicit DistractMovementGenerator(uint32 timer) : m_timer(timer) { }

        void Initialize(Unit*);
        void Finalize(Unit*);
        void Reset(Unit* owner) { Initialize(owner); }
        bool Update(Unit*, uint32);
        MovementGeneratorType GetMovementGeneratorType() { return DISTRACT_MOTION_TYPE; }

    private:
        uint32 m_timer;
};

class AssistanceDistractMovementGenerator : public DistractMovementGenerator
{
    public:
        AssistanceDistractMovementGenerator(uint32 timer) :
            DistractMovementGenerator(timer) { }

        MovementGeneratorType GetMovementGeneratorType() { return ASSISTANCE_DISTRACT_MOTION_TYPE; }
        void Finalize(Unit*);
};

#endif
