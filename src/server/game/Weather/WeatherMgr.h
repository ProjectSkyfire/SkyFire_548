/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup world
/// @{
/// \file

#ifndef SF_WEATHERMGR_H
#define SF_WEATHERMGR_H

#include "Define.h"

class Weather;
class Player;

namespace WeatherMgr
{
    void LoadWeatherData();

    Weather* FindWeather(uint32 id);
    Weather* AddWeather(uint32 zone_id);
    void RemoveWeather(uint32 zone_id);

    void SendFineWeatherUpdateToPlayer(Player* player);

    void Update(uint32 diff);
}

#endif
