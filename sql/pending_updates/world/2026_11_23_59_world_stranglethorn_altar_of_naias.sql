-- The Altar of Naias (26731) and An Unusual Patron (9457): the Gift of Naias (spell 30015) sends event 10554, which summons
-- Naias for the player to fight. The event summoned her 41 yards from the Altar of Naias, at ground level in the lake, as a
-- level 37 enemy, kept her for fifty minutes, did not stop a second summon while the first Naias stood, and gave her no
-- spells or speech. The summon position, the delay and the duration are as in Legends of Azeroth 5.4.8; the level (26)
-- and faction (91) are those of both Legends of Azeroth and the Cataclysm Preservation Project 4.3.4, which also have
-- the cast block while Naias is within 100 yards, and her Water Bolt and opening line. The Cataclysm Preservation Project
-- also lets her cast Water Shield on her victim, which is left out.

UPDATE `creature_template` SET `minlevel` = 26, `maxlevel` = 26, `faction_A` = 91, `faction_H` = 91, `AIName` = 'SmartAI' WHERE `entry` = 17207 AND `ScriptName` = '';

DELETE FROM `event_scripts` WHERE `id` = 10554 AND `command` = 10 AND `datalong` = 17207;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(10554, 5, 10, 17207, 300000, 0, -12128.9, 966, 5.48, 5.19);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 30015;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 30015, 0, 0, 29, 0, 17207, 100, 0, 1, 0, 0, '', 'Summon Naias - not while Naias is near');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 17207 AND `id` IN (0, 1);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17207, 0, 0, 0, 0, 0, 100, 0, 5000, 5000, 12000, 13000, 0, 11, 37054, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Naias - In Combat - Cast Water Bolt'),
(17207, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Naias - On Summoned - Say Line 0');

DELETE FROM `creature_text` WHERE `entry` = 17207;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(17207, 0, 0, 'Who challenges Naias? Puny $r, you are little better than those mindless trolls I have played against each other like so many pieces on a game board!', 12, 0, 100, 0, 0, 0, 'Naias');
