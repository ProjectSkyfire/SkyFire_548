-- Ulduar: Thorim Cache of Storms spawns, one per entry (10 and 25 player, normal and hard mode).
-- The script unlocks the one that matches the raid size and the hard mode state.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 910800 AND 910803;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910800, 194312, 603, 8, 0, 0, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.71325, 0.70091, -2592000, 255, 1),
(910801, 194313, 603, 8, 0, 0, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.71325, 0.70091, -2592000, 255, 1),
(910802, 194314, 603, 16, 0, 0, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.71325, 0.70091, -2592000, 255, 1),
(910803, 194315, 603, 16, 0, 0, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.71325, 0.70091, -2592000, 255, 1);

-- The script now summons the pre event creatures, the Runic Colossus add group and the Ancient Rune Giant add group
-- itself, so the static copies would only double them.
-- Pre event: Jormungar Behemoth (137587), Captured Mercenary Soldier (136471, 136472, 136473),
-- Dark Rune Acolyte (136490), Captured Mercenary Captain (136555).
-- Runic Colossus: Iron Ring Guard (136433, 136434, 137583, 137584), Dark Rune Acolyte (137585, 137586).
-- Ancient Rune Giant: Iron Honor Guard (136435, 136436), Dark Rune Acolyte (136274).
DELETE FROM `creature_addon` WHERE `guid` IN (136274, 136490, 136555);
DELETE FROM `creature` WHERE `map` = 603 AND `guid` IN (137587, 136471, 136472, 136473, 136490, 136555, 136433, 136434, 137583, 137584, 137585, 137586, 136435, 136436, 136274);

-- Six extra Thorim Controllers, only present in 10 player mode. The script uses the single controller at the arena
-- center (136446), as the 3.3.5 data has it.
DELETE FROM `creature` WHERE `map` = 603 AND `id` = 32879 AND `guid` IN (136447, 136448, 136449, 136450, 136451, 136452);
