-- Naxxramas: Thaddius' Tesla Coils (creatures, their beam auras, the coil objects), the coil script and its emotes.
DELETE FROM `creature` WHERE `guid` IN (8430030, 8430031);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`, `curhealth`) VALUES
(8430030, 16218, 533, 24, 3527.81, -2952.38, 319.326, 3.90954, 300, 0, 0, 1),
(8430031, 16218, 533, 24, 3487.76, -2911.2, 319.406, 3.90954, 300, 0, 0, 1);
DELETE FROM `creature_addon` WHERE `guid` IN (8430030, 8430031);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(8430030, 0, 0, 0, 0, 0, '28109'),
(8430031, 0, 0, 0, 0, 0, '28097');
DELETE FROM `gameobject` WHERE `guid` IN (910700, 910701);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(910700, 181477, 533, 24, 3527.94, -2952.26, 318.898, 3.14159, 0, 0, 1, 0.00000126759, 0, 0, 0),
(910701, 181478, 533, 24, 3487.32, -2911.38, 318.898, 3.14159, 0, 0, 1, 0.00000126759, 0, 0, 0);
UPDATE `creature_template` SET `ScriptName` = 'npc_tesla' WHERE `entry` = 16218 AND `ScriptName` = '' AND `AIName` = '';
DELETE FROM `creature_text` WHERE `entry` = 16218 AND `groupid` IN (0, 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(16218, 0, 0, '%s loses its link!', 41, 0, 100, 0, 0, 0, 'Tesla Coil - link broken'),
(16218, 1, 0, '%s overloads!', 41, 0, 100, 0, 0, 0, 'Tesla Coil - overload');

-- The encounter and both platform adds must also exist in 25-player mode.
-- Match each existing spawn explicitly and preserve other spawn-mask bits.
UPDATE `creature` SET `spawnMask` = `spawnMask` | 16
WHERE `map` = 533 AND (`spawnMask` & 8) <> 0
  AND ((`guid` = 130957 AND `id` = 15928)
    OR (`guid` = 130958 AND `id` = 15929)
    OR (`guid` = 130959 AND `id` = 15930));
