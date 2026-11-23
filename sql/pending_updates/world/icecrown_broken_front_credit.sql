-- The Broken Front (13228, 13231) and Finish Me! (13232): the dying soldiers and berserkers gave no quest credit.
-- Dying Soldier had SmartAI without a single script row and Dying Berserker had no AI at all, and the Berserker's
-- gossip option was shown to everyone. Gossip option 3 of menu 10040 is the third option of the Soldier script.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (31273, 31304);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (31273, 31304);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(31273, 0, 0, 0, 62, 0, 100, 0, 10036, 0, 0, 0, 0, 11, 58922, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dying Berserker - On Gossip Option 0 Selected - Cast Horde Quest Aura'),
(31304, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 81, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Respawn - Set Npc Flag Gossip'),
(31304, 0, 1, 2, 62, 0, 100, 0, 10040, 0, 0, 0, 0, 11, 58955, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Gossip Option 0 Selected - Cast Alliance Quest Aura'),
(31304, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Link - Set Npc Flag Gossip and Quest Giver'),
(31304, 0, 3, 0, 19, 0, 100, 0, 13232, 0, 0, 0, 0, 83, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Quest 13232 Accepted - Remove Npc Flag Quest Giver'),
(31304, 0, 4, 5, 62, 0, 100, 0, 10040, 1, 0, 0, 0, 11, 58955, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Gossip Option 1 Selected - Cast Alliance Quest Aura'),
(31304, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 3240, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Link - Cast Bloody Explosion'),
(31304, 0, 7, 8, 62, 0, 100, 0, 10040, 3, 0, 0, 0, 81, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Gossip Option 3 Selected - Set Npc Flag Gossip and Quest Giver'),
(31304, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Dying Soldier - On Link - Close Gossip');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10036 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10036, 0, 0, 0, 9, 0, 13228, 0, 0, 0, 0, 0, '', 'Dying Berserker - Show gossip option 0 only if the player has quest 13228');
