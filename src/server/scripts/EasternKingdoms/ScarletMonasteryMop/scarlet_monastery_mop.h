/*
 * Ported from Legends of Azeroth 5.4.8.
 * Authors: Jason Dove, Kulyadev, lee, leelf00
 * See LICENSE.md for copyright information.
 */
/*
 * This file is part of the Pandaria 5.4.8 Project. See THANKS file for copyright information.
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

#ifndef SCARLET_MONASTERY_MOP_H
#define SCARLET_MONASTERY_MOP_H

enum ScarletMonasteryData
{
    BOSS_THALNOS_THE_SOULRENDER = 0,
    BOSS_BROTHER_KORLOFF = 1,
    BOSS_WHITEMANE = 2,
    BOSS_DURAND = 3,
    ENCOUNTER_COUNT = 4
};

enum ScarletMonasteryCreatures
{
    NPC_THALNOS_THE_SOULRENDER = 59789,
    NPC_BROTHER_KORLOFF = 59223,
    NPC_COMMANDER_DURAND = 60040,
    NPC_HIGH_INQUISITOR_WHITEMANE = 3977,
    NPC_FALLEN_CRUSADER = 59884,
    NPC_HOODED_CRUSADER_OUTRO = 64842
};

enum ScarletMonasteryObjects
{
    GO_THALNOS_EXIT = 211844,
    GO_KORLOFF_EXIT = 210564,
    GO_WHITEMANE_ENTRANCE = 210563
};
#endif
