/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _ADV_STD_H_
#define _ADV_STD_H_

#include <cstddef>
#include <type_traits>

// this namespace holds implementations of upcoming stdlib features that our c++ version doesn't have yet
namespace advstd
{
    // C++20 advstd::remove_cvref_t
    template <class T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    // C++20 std::type_identity
    template <typename T>
    struct type_identity
    {
        using type = T;
    };

    // C++20 std::type_identity_t
    template <typename T>
    using type_identity_t = typename type_identity<T>::type;
}

#endif // _ADV_STD_H_
