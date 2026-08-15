-- Burning Bomber Credit
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42046;
UPDATE `creature_template` SET `modelid1`=11686 WHERE `entry`=42046;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=42046;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42046;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42046, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 78357, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Burning Bomber Credit - On Respawn - Cast Spell'),
(42046, 0, 1, 0, 8, 0, 100, 0, 78369, 0, 0, 0, 0, 80, 4204600, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Burning Bomber Credit - On Spellhit - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4204600;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4204600, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42046, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Burning Bomber Credit - On Script - Give Credit'),
(4204600, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Burning Bomber Credit - On Script - Despawn');

-- Extinguish the Fires
UPDATE `quest_template` SET `OfferRewardText`='Well done! My men are working hard on getting our flying machines and steam tanks back into the fight.' WHERE `Id`=26078;
UPDATE `quest_template` SET `RequestItemsText`='We need to hurry and get those bombers airborne!' WHERE `Id`=26078;
