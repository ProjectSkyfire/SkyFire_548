-- Thousand Needles quests that are finished by clicking a creature.
-- Carcass Collection (25672) asks for 10 carcasses. Clicking a dead Saltstone Crystalhide (41099), Scorpid Reaver
-- (41100), Scorpid Terror (41101), Sparkleshell Snapper (41103, 48148), Sparkleshell Tortoise (41104, 48147), Saltstone
-- Gazer (41109) or Salt Flats Vulture (41111) casts spell 93455 (Carcass Collection: Create Carcass) on the player, which
-- creates the carcass (item 55226). The click is allowed while the quest is taken. Legends of Azeroth has no condition on
-- 48148; it gets the one of its twin 48147, the same quest.
-- Free the Pridelings (28086) asks for 12 Highperch Pridelings (47481, 41 spawns). The click (spell 88554) does nothing
-- by itself: the script gives the credit, removes the net aura the prideling gets on respawn (spell 84531) and lets it
-- go. Legends of Azeroth also summons a prideling that runs off after the click, which is left out here.
-- As in Legends of Azeroth 5.4.8 otherwise.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (41099, 41100, 41101, 41103, 41104, 41109, 41111, 48147, 48148, 47481);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (47481) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (41099, 41100, 41101, 41103, 41104, 41109, 41111, 47481, 48147, 48148);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(41099, 93455, 2, 0),
(41100, 93455, 2, 0),
(41101, 93455, 2, 0),
(41103, 93455, 2, 0),
(41104, 93455, 2, 0),
(41109, 93455, 2, 0),
(41111, 93455, 2, 0),
(48147, 93455, 2, 0),
(48148, 93455, 2, 0),
(47481, 88554, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41099 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41100 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41101 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41103 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41104 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41109 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 41111 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 48147 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 48148 AND `SourceEntry` = 93455;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 47481 AND `SourceEntry` = 88554;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 41099, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Saltstone Crystalhide - only with Carcass Collection'),
(18, 41100, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Scorpid Reaver - only with Carcass Collection'),
(18, 41101, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Scorpid Terror - only with Carcass Collection'),
(18, 41103, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Sparkleshell Snapper - only with Carcass Collection'),
(18, 41104, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Sparkleshell Tortoise - only with Carcass Collection'),
(18, 41109, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Saltstone Gazer - only with Carcass Collection'),
(18, 41111, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Salt Flats Vulture - only with Carcass Collection'),
(18, 48147, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Sparkleshell Tortoise - only with Carcass Collection'),
(18, 48148, 93455, 0, 0, 9, 0, 25672, 0, 0, 0, 0, 0, '', 'Sparkleshell Snapper - only with Carcass Collection');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 47481 AND `id` IN (0, 1, 2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47481, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 84531, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Highperch Prideling - On Respawn - Cast Cosmetic Net'),
(47481, 0, 1, 2, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47481, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Highperch Prideling - On Spellclick - Quest Credit'),
(47481, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 84531, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Highperch Prideling - Linked To Id 1 - Remove Aura Cosmetic Net'),
(47481, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Highperch Prideling - Linked To Id 2 - Despawn after 1 sec');
