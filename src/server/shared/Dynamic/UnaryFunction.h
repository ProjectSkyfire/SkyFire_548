/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_UNARY_FUNCTION_H
#define SKYFIRE_UNARY_FUNCTION_H

template<class ArgumentType, class ResultType>
struct unary_function
{
    typedef ArgumentType argument_type;
    typedef ResultType result_type;
};

#define SF_UNARY_FUNCTION unary_function

#endif 
