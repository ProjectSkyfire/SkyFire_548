-- Mulgore Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44848 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44848, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44848, 0, 1, 0, 8, 0, 100, 0, 35395, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Crusader Strike - Give Quest Credit'),
(44848, 0, 2, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44848, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44848, 0, 4, 0, 8, 0, 100, 0, 73899, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Primal Strike - Give Quest Credit'),
(44848, 0, 5, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit'),
(44848, 0, 6, 0, 8, 0, 100, 0, 8921, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Moonfire - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44848;

-- Gnomeregan Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44171 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44171, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44171, 0, 1, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44171, 0, 2, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44171, 0, 3, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44171, 0, 4, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44171, 0, 5, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44171;

-- Northshire Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44548 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44548, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44548, 0, 1, 0, 8, 0, 100, 0, 35395, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Crusader Strike - Give Quest Credit'),
(44548, 0, 2, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44548, 0, 3, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44548, 0, 4, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44548, 0, 5, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44548, 0, 6, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44548, 0, 7, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44548;

-- Aldrassil Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44614 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44614, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44614, 0, 1, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44614, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44614, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44614, 0, 4, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44614, 0, 5, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit'),
(44614, 0, 6, 0, 8, 0, 100, 0, 8921, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Moonfire - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44614;

-- Anvilmar Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44389 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44389, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44389, 0, 1, 0, 8, 0, 100, 0, 35395, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Crusader Strike - Give Quest Credit'),
(44389, 0, 2, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44389, 0, 3, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44389, 0, 4, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44389, 0, 5, 0, 8, 0, 100, 0, 73899, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Primal Strike - Give Quest Credit'),
(44389, 0, 6, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44389, 0, 7, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44389, 0, 8, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44389;

-- Valley of Trials Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44820 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44820, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44820, 0, 1, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44820, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44820, 0, 3, 0, 8, 0, 100, 0, 73899, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Primal Strike - Give Quest Credit'),
(44820, 0, 4, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44820, 0, 5, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44820, 0, 6, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44820;

-- Darkspear Training Grounds Tiki Target 
DELETE FROM `smart_scripts` WHERE `entryorguid`=38038 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38038, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Charge - Give Quest Credit'),
(38038, 0, 1, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Steady Shot - Give Quest Credit'),
(38038, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Eviscerate - Give Quest Credit'),
(38038, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(38038, 0, 4, 0, 8, 0, 100, 0, 73899, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Primal Strike - Give Quest Credit'),
(38038, 0, 5, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Frost Nova - Give Quest Credit'),
(38038, 0, 6, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Corruption - Give Quest Credit'),
(38038, 0, 7, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Tiger Palm - Give Quest Credit'),
(38038, 0, 8, 0, 8, 0, 100, 0, 8921, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tiki Target - On Spellhit - Moonfire - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=38038;

-- Deathknell Training Dummy
DELETE FROM `smart_scripts` WHERE `entryorguid`=44794 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44794, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Charge - Give Quest Credit'),
(44794, 0, 1, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Steady Shot - Give Quest Credit'),
(44794, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Eviscerate - Give Quest Credit'),
(44794, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(44794, 0, 4, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Frost Nova - Give Quest Credit'),
(44794, 0, 5, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Corruption - Give Quest Credit'),
(44794, 0, 6, 0, 8, 0, 100, 0, 100787, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Tiger Palm - Give Quest Credit');
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=44794;
