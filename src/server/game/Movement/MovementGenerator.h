/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MOVEMENTGENERATOR_H
#define SKYFIRE_MOVEMENTGENERATOR_H

#include "Common.h"
#include "Define.h"
#include "FactoryHolder.h"
#include "MotionMaster.h"
#include "ObjectRegistry.h"
#include <ace/Singleton.h>

class Unit;

class MovementGenerator
{
public:
    virtual ~MovementGenerator();

    virtual void Initialize(Unit*) = 0;
    virtual void Finalize(Unit*) = 0;

    virtual void Reset(Unit*) = 0;

    virtual bool Update(Unit*, uint32 time_diff) = 0;

    virtual MovementGeneratorType GetMovementGeneratorType() = 0;

    virtual void unitSpeedChanged() { }

    // used by Evade code for select point to evade with expected restart default movement
    virtual bool GetResetPosition(Unit*, float& /*x*/, float& /*y*/, float& /*z*/) { return false; }
};

template<class T, class D>
class MovementGeneratorMedium : public MovementGenerator
{
public:
    void Initialize(Unit* u)
    {
        //u->AssertIsType<T>();
        (static_cast<D*>(this))->DoInitialize(static_cast<T*>(u));
    }

    void Finalize(Unit* u)
    {
        //u->AssertIsType<T>();
        (static_cast<D*>(this))->DoFinalize(static_cast<T*>(u));
    }

    void Reset(Unit* u)
    {
        //u->AssertIsType<T>();
        (static_cast<D*>(this))->DoReset(static_cast<T*>(u));
    }

    bool Update(Unit* u, uint32 time_diff)
    {
        //u->AssertIsType<T>();
        return (static_cast<D*>(this))->DoUpdate(static_cast<T*>(u), time_diff);
    }
};

struct SelectableMovement : public FactoryHolder<MovementGenerator, MovementGeneratorType>
{
    SelectableMovement(MovementGeneratorType mgt) : FactoryHolder<MovementGenerator, MovementGeneratorType>(mgt) { }
};

template<class REAL_MOVEMENT>
struct MovementGeneratorFactory : public SelectableMovement
{
    MovementGeneratorFactory(MovementGeneratorType mgt) : SelectableMovement(mgt) { }

    MovementGenerator* Create(void*) const;
};

typedef FactoryHolder<MovementGenerator, MovementGeneratorType> MovementGeneratorCreator;
typedef FactoryHolder<MovementGenerator, MovementGeneratorType>::FactoryHolderRegistry MovementGeneratorRegistry;
typedef FactoryHolder<MovementGenerator, MovementGeneratorType>::FactoryHolderRepository MovementGeneratorRepository;
#endif
