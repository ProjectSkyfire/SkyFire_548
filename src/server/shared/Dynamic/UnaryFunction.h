/*
* Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
* Copyright (C) 2008-2021 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2021 MaNGOS <https://www.getmangos.eu/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
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