-- Out of the Ziggurat (27481) and Into the Flames (27482) cannot be finished. Vex'tul, the undead who walks with the
-- player after the first quest, was never summoned (no area aura row), could not give or take the quests, and had no
-- script, and the bunny at the bonfire on top of Zul'Mashar did not react to Warlord Thresh'jin's body (spell 85327,
-- usable on any target). Now the Summon Vex'tul aura is applied at the ziggurat while the first quest is taken and the
-- second not rewarded, Vex'tul ends 27481 and gives and ends 27482, the body can be burnt only at the bonfire bunny,
-- and the bunny sends Vex'tul to the fire with his farewell speech, which gives the credit.
-- Rows, texts, the area aura and the condition are as in Legends of Azeroth 5.4.8. Its first Vex'tul row, which kills him
-- one second after he is summoned, is left out.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45738, 45741) AND `ScriptName` = '';
UPDATE `creature_template` SET `npcflag` = `npcflag` | 2 WHERE `entry` = 45741;

DELETE FROM `smart_scripts` WHERE (`entryorguid` IN (45738, 45741) AND `source_type` = 0) OR (`entryorguid` = 4574100 AND `source_type` = 9);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45738, 0, 0, 0, 8, 0, 100, 0, 85327, 0, 10000, 10000, 0, 45, 1, 0, 0, 0, 0, 0, 19, 45741, 20, 0, 0, 0, 0, 0, 'Into the Flames Credit - On Spellhit Destroy Thresh''jin''s Body - Set Data 1 On Vex''tul'),
(45741, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - On Death - Despawn'),
(45741, 0, 1, 2, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - On Summon - Store Target'),
(45741, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - Linked To Id 1 - Set Defensive'),
(45741, 0, 3, 0, 20, 0, 100, 0, 27482, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - On Quest Reward - Despawn'),
(45741, 0, 4, 0, 38, 0, 100, 0, 1, 0, 0, 0, 0, 80, 4574100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - On Data Set 1 - Run Script'),
(4574100, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - Set Immune Flags'),
(4574100, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - Set Run'),
(4574100, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 3456.71, -4998.24, 198.802, 0, 'Vex''tul - Script - Move to Pos'),
(4574100, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - Say Line 0'),
(4574100, 9, 4, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - Say Line 1'),
(4574100, 9, 5, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 97, 10, 10, 0, 0, 0, 0, 1, 0, 0, 0, 3462.09, -5004.08, 202.471, 0, 'Vex''tul - Script - Jump to Pos'),
(4574100, 9, 6, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 90, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - State Death'),
(4574100, 9, 7, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 33, 45738, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Vex''tul - Script - Killcredit');;

DELETE FROM `creature_text` WHERE `entry` = 45741;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45741, 0, 0, 'I will be goin'' now.  I have no reason to remain here.  I may be undead, but my will is my own.', 12, 0, 100, 0, 0, 0, ''),
(45741, 1, 0, 'I will see you in green fields on the other side, $n.', 12, 0, 100, 0, 0, 0, '');

DELETE FROM `creature_queststarter` WHERE `id` = 45741 AND `quest` = 27482;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(45741, 27482);

DELETE FROM `creature_questender` WHERE `id` = 45741 AND `quest` IN (27481, 27482);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(45741, 27481),
(45741, 27482);

DELETE FROM `spell_area` WHERE `spell` = 85334 AND `area` = 2273;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(85334, 2273, 27481, 27482, 0, 0, 2, 1, 74, 9);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 85327;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 85327, 0, 0, 31, 0, 3, 45738, 0, 0, 0, 0, '', 'Destroy Thresh''jin''s Body can hit only the Into the Flames Credit bunny');
