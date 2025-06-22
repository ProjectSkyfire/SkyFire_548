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
    void Initialize(Unit*) OVERRIDE;
    void Finalize(Unit*) OVERRIDE {  }
    void Reset(Unit*) OVERRIDE;
    bool Update(Unit*, uint32) OVERRIDE { return true; }
    MovementGeneratorType GetMovementGeneratorType() OVERRIDE { return IDLE_MOTION_TYPE; }
};

extern IdleMovementGenerator si_idleMovement;

class RotateMovementGenerator : public MovementGenerator
{
public:
    explicit RotateMovementGenerator(uint32 time, RotateDirection direction) : m_duration(time), m_maxDuration(time), m_direction(direction) { }

    void Initialize(Unit*) OVERRIDE;
    void Finalize(Unit*) OVERRIDE;
    void Reset(Unit* owner) OVERRIDE { Initialize(owner); }
    bool Update(Unit*, uint32) OVERRIDE;
    MovementGeneratorType GetMovementGeneratorType() OVERRIDE { return ROTATE_MOTION_TYPE; }

private:
    uint32 m_duration, m_maxDuration;
    RotateDirection m_direction;
};

class DistractMovementGenerator : public MovementGenerator
{
public:
    explicit DistractMovementGenerator(uint32 timer) : m_timer(timer) { }

    void Initialize(Unit*) OVERRIDE;
    void Finalize(Unit*) OVERRIDE;
    void Reset(Unit* owner) OVERRIDE { Initialize(owner); }
    bool Update(Unit*, uint32) OVERRIDE;
    MovementGeneratorType GetMovementGeneratorType() OVERRIDE { return DISTRACT_MOTION_TYPE; }

private:
    uint32 m_timer;
};

class AssistanceDistractMovementGenerator : public DistractMovementGenerator
{
public:
    AssistanceDistractMovementGenerator(uint32 timer) :
        DistractMovementGenerator(timer) { }

    MovementGeneratorType GetMovementGeneratorType() OVERRIDE { return ASSISTANCE_DISTRACT_MOTION_TYPE; }
    void Finalize(Unit*) OVERRIDE;
};

#endif
