-- An Ancient Enemy (24814): "I am ready, Vol'jin." starts the fight with Zar'jira at Spitescale Cove. Below half health
-- she draws power from three Fires of the Seas, which players stamp out to make her vulnerable again.
UPDATE `creature_template` SET `ScriptName` = 'npc_voljin_ancient_enemy' WHERE `entry` = 38225;
UPDATE `creature_template` SET `ScriptName` = 'npc_zarjira' WHERE `entry` = 38306;
UPDATE `creature_template` SET `ScriptName` = 'npc_fire_of_the_seas', `npcflag` = 16777216 WHERE `entry` = 38542;

-- Zar'jira was a level 85 elite with about 7.7 million health; she is a level 5 elite.
UPDATE `creature_template` SET `minlevel` = 5, `maxlevel` = 5, `exp` = 0, `mindmg` = 9, `maxdmg` = 17, `attackpower` = 19 WHERE `entry` = 38306;

-- Manifestation of the Sea Witch, her add: friendly, level 1 and without damage before.
UPDATE `creature_template` SET `minlevel` = 4, `maxlevel` = 4, `faction_A` = 2224, `faction_H` = 2224, `unit_flags` = 32768,
    `mindmg` = 5, `maxdmg` = 13, `attackpower` = 18, `baseattacktime` = 2000, `AIName` = 'SmartAI' WHERE `entry` = 38302;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 38302;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38302, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Manifestation of the Sea Witch - On summoned - Attack the closest player'),
(38302, 0, 1, 0, 0, 0, 100, 0, 7000, 7000, 10000, 10000, 0, 11, 73432, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Manifestation of the Sea Witch - In combat - Cast Soul Scar'),
(38302, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Manifestation of the Sea Witch - On death - Despawn');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 38542;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38542, 73296, 1, 0);

DELETE FROM `creature` WHERE `guid` IN (8311000, 8311001, 8311002);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311000, 38542, 1, 1, 0, 0, 0, 0, -711.141, -5574.35, 26.053, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8311001, 38542, 1, 1, 0, 0, 0, 0, -706.938, -5578.52, 26.0191, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8311002, 38542, 1, 1, 0, 0, 0, 0, -700.205, -5579.72, 26.0177, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 73294) OR (`SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11020 AND `SourceEntry` = 0);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 73294, 0, 0, 31, 0, 3, 38306, 0, 0, 0, 0, '', 'Fire of the Seas Energy Beam targets Zar''jira'),
(15, 11020, 0, 0, 0, 9, 0, 24814, 0, 0, 0, 0, 0, '', 'Vol''jin: "I am ready" only while An Ancient Enemy is in progress');

DELETE FROM `creature_text` WHERE `entry` IN (38225, 38306, 38437);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38225, 0, 0, 'Ya were foolish to come ''ere, Sea Witch. Ya escaped our vengeance once, but the Darkspear Tribe will not abide ya trespassin'' again.', 14, 0, 100, 1, 0, 20119, 'Vol''jin - Intro'),
(38225, 1, 0, 'She''s drawing power from the fires! Stamp out the braziers, quickly!', 41, 0, 100, 0, 0, 0, 'Vol''jin - Fires of the Seas lit'),
(38306, 0, 0, 'You are weak Vol''jin, like your father was weak. Today I will finish what I started long ago - the Darkspear shall be wiped from existence!', 14, 0, 100, 1, 0, 21810, 'Zar''jira - Intro'),
(38306, 1, 0, 'Leave the fire alone, pesky trolls! Just give up and die.', 14, 0, 100, 0, 0, 21812, 'Zar''jira - A fire is stamped out'),
(38306, 2, 0, 'No! I will make your death slow and painful, little trolls.', 14, 0, 100, 0, 0, 21813, 'Zar''jira - All fires are out'),
(38437, 0, 0, 'Take care of her spirits! We be handlin'' Zar''jira.', 14, 0, 100, 5, 0, 21944, 'Vanira - Zar''jira engages');
