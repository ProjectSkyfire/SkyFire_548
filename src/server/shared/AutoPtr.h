/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _SKYFIRE_AUTO_PTR_H
#define _SKYFIRE_AUTO_PTR_H

#include <ace/Bound_Ptr.h>

namespace Skyfire
{

template <class Pointer, class Lock>
class AutoPtr : public ACE_Strong_Bound_Ptr<Pointer, Lock>
{
    typedef ACE_Strong_Bound_Ptr<Pointer, Lock> Base;

public:
    AutoPtr()
        : Base()
    { }

    AutoPtr(Pointer* x)
        : Base(x)
    { }

    operator bool() const
    {
        return !Base::null();
    }

    bool operator !() const
    {
        return Base::null();
    }
};

} // namespace Skyfire

#endif
