-- The Goblin Heavy Gun (41895) in Krom'gar Fortress is a placeholder: level 1, no vehicle, no spell and no spawn, so
-- "In Defense of Krom'gar Fortress" (26058) cannot be done. Make it the gun the quest asks the player to take control of.
-- Vehicle 844 has one seat that controls and casts, and Rocket Blast (78295) is the gun's spell. Based on Legends of
-- Azeroth 5.4.8, except for the points below.
--
-- unit_flags is 0 and not the 768 of that data: the immune-to-NPC bit stops the gun's own spells from hitting
-- creatures, and stops a player without the PvP flag from boarding it. Health_mod is raised because the Machine Gun
-- (78298) of the bombers picks the gun as a target, and the immunity that would protect it cannot be used.
-- The click casts 46598 (Ride Vehicle Hardcoded) and not 78286 (Control Goblin Heavy Gun): 78286 moves the rider
-- and the gun to phase 170, where the bombers, which are in the default phase, cannot be seen or hit.
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 29, `faction_H` = 29, `npcflag` = 16777216, `baseattacktime` = 2000, `rangeattacktime` = 2000, `unit_class` = 4, `unit_flags` = 0, `spell1` = 78295, `VehicleId` = 844, `Health_mod` = 5 WHERE `entry` = 41895;

DELETE FROM `creature_template_addon` WHERE `entry` = 41895;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(41895, 0, 0, 65536, 1, 0, NULL);

-- The four guns of the fortress. The invisible, feigning 41894 guns stand on the same spots and stay as they are.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255210 AND 9255213;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9255210, 41895, 1, 1, 0, 0, 0, 0, 923.851, 5.76042, 93.2641, 0.663225, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9255211, 41895, 1, 1, 0, 0, 0, 0, 928.064, -0.815972, 93.2641, 0.750492, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9255212, 41895, 1, 1, 0, 0, 0, 0, 921.507, 8.72396, 93.3419, 0.575959, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9255213, 41895, 1, 1, 0, 0, 0, 0, 925.766, 2.49653, 93.2641, 0.663225, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- Only a player who has taken the quest can board.
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 41895 AND `spell_id` = 46598;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(41895, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41895 AND `SourceEntry` = 46598;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 41895, 46598, 0, 0, 9, 0, 26058, 0, 0, 0, 0, 0, '', 'Goblin Heavy Gun requires quest 26058 taken for spellclick 46598');
