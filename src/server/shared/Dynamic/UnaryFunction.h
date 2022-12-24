/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_UNARY_FUNCTION_H
#define SKYFIRE_UNARY_FUNCTION_H

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1910 && _HAS_CXX17
template<class ArgumentType, class ResultType>
struct unary_function
{
    typedef ArgumentType argument_type;
    typedef ResultType result_type;
};

#define SF_UNARY_FUNCTION unary_function
#else 
#define SF_UNARY_FUNCTION std::unary_function
#endif 
#endif