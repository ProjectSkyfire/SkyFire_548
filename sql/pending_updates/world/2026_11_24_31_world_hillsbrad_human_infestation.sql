-- Human Infestation (28138): the Titanium Shackles (88797) can be used on a captured Hillsbrad Miner, Sentry or Foreman
-- who is below 35 percent health; the human is freed, credited and follows the player to Captain Keyton. None of that was
-- scripted here: the spell could target anything and the three kinds of human had no credit, no freed human and no
-- paralytic poison, so the quest could not be finished. Added as in Legends of Azeroth 5.4.8: the target conditions of
-- the shackles (unit entry and health), the random poison cast on respawn, the spellhit credit that summons a freed
-- human (spells 88799, 88803 and 88804 in the client data summon 47689, 47693 and 47694), and the AI of the freed humans,
-- who follow the player and vanish near Captain Keyton. The existing flee rows of the Sentry and the Foreman are kept
-- and the new rows follow them. The poison variants 87972 and 87973 are stuns, 87969 is a visual.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (47689, 47693, 47694);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2269 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3, 4, 5, 6);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2269, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 87, 226900, 226901, 226902, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Respawn - Random Cast Paralytic Poison'),
(2269, 0, 1, 2, 8, 0, 100, 0, 88797, 0, 0, 0, 0, 33, 47697, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Spellhit - Give Kill Credit'),
(2269, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88799, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Spellhit - Cross Cast Captured Hillsbrad Human'),
(2269, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Spellhit - Force Despawn'),
(2269, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Spellhit - Set Home Pos'),
(2269, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - On Spellhit - Evade'),
(2269, 0, 6, 0, 2, 0, 100, 1, 0, 10, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Miner - Between 0-10% Health - Flee For Assist');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2270 AND `source_type` = 0 AND `id` IN (1, 2, 3, 4, 5, 6, 7);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2270, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 87, 226900, 226901, 226902, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Respawn - Random Cast Paralytic Poison'),
(2270, 0, 2, 3, 8, 0, 100, 0, 88797, 0, 0, 0, 0, 33, 47697, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Spellhit - Give Kill Credit'),
(2270, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88804, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Spellhit - Cross Cast Captured Hillsbrad Human'),
(2270, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Spellhit - Force Despawn'),
(2270, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Spellhit - Set Home Pos'),
(2270, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - On Spellhit - Evade'),
(2270, 0, 7, 0, 0, 0, 100, 0, 1000, 1000, 180000, 180000, 0, 11, 7164, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Sentry - In Combat - Cast ''Defensive Stance''');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 2503 AND `source_type` = 0 AND `id` IN (2, 3, 4, 5, 6, 7);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2503, 0, 2, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 87, 226900, 226901, 226902, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Respawn - Random Cast Paralytic Poison'),
(2503, 0, 3, 4, 8, 0, 100, 0, 88797, 0, 0, 0, 0, 33, 47697, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Spellhit - Give Kill Credit'),
(2503, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88803, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Spellhit - Cross Cast Captured Hillsbrad Human'),
(2503, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Spellhit - Force Despawn'),
(2503, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Spellhit - Set Home Pos'),
(2503, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hillsbrad Foreman - On Spellhit - Evade');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 47689 AND `source_type` = 0 AND `id` IN (0, 1, 2, 4);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47693 AND `source_type` = 0 AND `id` IN (0, 1, 2, 4);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 47694 AND `source_type` = 0 AND `id` IN (0, 1, 2, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47689, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Immune'),
(47689, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Run'),
(47689, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Follow'),
(47689, 0, 4, 0, 75, 0, 100, 1, 0, 47554, 10, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Distance Npc - Force Despawn'),
(47693, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Immune'),
(47693, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Run'),
(47693, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Follow'),
(47693, 0, 4, 0, 75, 0, 100, 1, 0, 47554, 10, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Distance Npc - Force Despawn'),
(47694, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Immune'),
(47694, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Set Run'),
(47694, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Summon - Follow'),
(47694, 0, 4, 0, 75, 0, 100, 1, 0, 47554, 10, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Hillsbrad Human - On Distance Npc - Force Despawn');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 226900 AND `source_type` = 9 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 226901 AND `source_type` = 9 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 226902 AND `source_type` = 9 AND `id` IN (0);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(226900, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 87969, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Human Proxy - Script - Cast Paralytic Poison'),
(226901, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 87972, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Human Proxy - Script - Cast Paralytic Poison'),
(226902, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 11, 87973, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Human Proxy - Script - Cast Paralytic Poison');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 88797;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 88797, 0, 0, 31, 1, 3, 2269, 0, 0, 0, 0, '', 'Titanium Shackles can bind a Hillsbrad Miner'),
(17, 0, 88797, 0, 0, 38, 1, 35, 4, 0, 0, 0, 0, '', 'Titanium Shackles need the target at 35 percent health or less'),
(17, 0, 88797, 0, 1, 31, 1, 3, 2270, 0, 0, 0, 0, '', 'Titanium Shackles can bind a Hillsbrad Sentry'),
(17, 0, 88797, 0, 1, 38, 1, 35, 4, 0, 0, 0, 0, '', 'Titanium Shackles need the target at 35 percent health or less'),
(17, 0, 88797, 0, 2, 31, 1, 3, 2503, 0, 0, 0, 0, '', 'Titanium Shackles can bind a Hillsbrad Foreman'),
(17, 0, 88797, 0, 2, 38, 1, 35, 4, 0, 0, 0, 0, '', 'Titanium Shackles need the target at 35 percent health or less');
