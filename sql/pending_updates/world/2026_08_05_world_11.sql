-- Burn Constriction Totem
DELETE FROM `spell_script_names` WHERE `spell_id`=77314;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(77314, 'spell_gen_burn_constriction_totem');

-- Pushing Forward
UPDATE `quest_template` SET `PrevQuestId`=313 WHERE `Id`=25792;

-- Constriction Totem
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41202;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41202;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41202, 0, 0, 0, 8, 0, 100, 0, 77314, 0, 0, 0, 0, 80, 4120200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Constriction Totem - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4120200;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4120200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 41237, 10, 0, 0, 0, 0, 0, 'Constriction Totem - On Script - Set Data 1 1 to Kharanos Mountaineer'),
(4120200, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 41202, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Constriction Totem - On Script - Give Credit'),
(4120200, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Constriction Totem - On Script - Despawn');

-- Kharanos Mountaineer
DELETE FROM `creature_text` WHERE `entry`=41237;
INSERT INTO `creature_text` (`entry`, `text`, `type`, `probability`, `comment`) VALUES
(41237, 'Thank you for freeing me!', 12, 100, 'Kharanos Mountaineer - SAY_0');

UPDATE `creature_template_addon` SET `auras`='77311' WHERE `entry`=41237;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41237;

DELETE FROM `smart_scripts` WHERE `entryorguid`=41237;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41237, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Spawn - Set Data 1 0'),
(41237, 0, 1, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 77311, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Spawn - Cast Spell'),
(41237, 0, 2, 0, 38, 0, 100, 0, 1, 1, 1000, 1000, 0, 80, 4123700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captured Demolitionist - Data Set - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4123700;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4123700, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 28, 77311, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Script - Remove Aura'),
(4123700, 9, 1, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Script - Talk 0'),
(4123700, 9, 2, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 46, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Script - Move Forward'),
(4123700, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kharanos Mountaineer - On Script - Despawn');
