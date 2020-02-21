SET @WORGEN := 35118;
SET @KC := 44175;

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE  `entry`=35118;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@WORGEN;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@WORGEN, 0, 0, 0, 8, 0, 100, 0, 100, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Charge - Give Quest Credit'),
(@WORGEN, 0, 1, 0, 8, 0, 100, 0, 56641, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Steady Shot - Give Quest Credit'),
(@WORGEN, 0, 2, 0, 8, 0, 100, 0, 2098, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Eviscerate - Give Quest Credit'),
(@WORGEN, 0, 3, 0, 8, 0, 100, 0, 589, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Shadow Word: Pain - Give Quest Credit'),
(@WORGEN, 0, 4, 0, 8, 0, 100, 0, 122, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Frost Nova - Give Quest Credit'),
(@WORGEN, 0, 5, 0, 8, 0, 100, 0, 172, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Corruption - Give Quest Credit'),
(@WORGEN, 0, 6, 0, 8, 0, 100, 0, 8921, 0, 0, 0, 33, @KC, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodfang Worgen - On Spellhit - Moonfire - Give Quest Credit');
