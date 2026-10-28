/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
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

/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
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

#ifndef SCARLET_HALLS_H
#define SCARLET_HALLS_H

#include "ScriptedCreature.h"

enum ScarletHallsData
{
    BOSS_HOUNDMASTER_BRAUN = 0,
    BOSS_ARMSMASTER_HARLAN = 1,
    BOSS_FLAMEWEAVER_KOEGLER = 2,
    ENCOUNTER_COUNT = 3,
    DATA_COMANDER_LINDON = 4
};

enum ScarletHallsCreatures
{
    NPC_HOUNDMASTER_BRAUN = 59303,
    NPC_ARMSMASTER_HARLAN = 58632,
    NPC_FLAMEWEAVER_KOEGLER = 59150,
    NPC_OBEDIEND_HOUND = 59309,
    NPC_COMANDER_LINDON = 59191,
    NPC_SERGEANT_VERDONE = 59302,
    NPC_SCARLET_GUARDIAN = 59299
};

enum ScarletHallsObjects
{
    GO_COMANDER_LINDON_EXIT = 212160,
    GO_HOUNDMASTER_BRAUN_EXIT = 210097,
    GO_ARMSMASTER_HARLAN_EXIT = 210480
};

enum ScarletHallsWorldStates
{
    WORLDSTATE_HUMANE_SOCIETY = 12645
};

const Position EndScenario = { 1038.13f, 509.705f, 13.47f, 0.10f };

#endif
