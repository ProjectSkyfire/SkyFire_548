-- The Universal Key (26400): the player clicks a Venture Co. Sawblade (43003) while carrying the Mosh'Ogg Bounty, which
-- casts spell 80277 and makes the Opened Mosh'Ogg Bounty (item 58490) the quest asks for. The sawblades had no spell
-- click flag, no spell click row and no script, so they could not be clicked. The click is allowed only while the quest
-- is taken, and the sawblade says its line when clicked. The rows and the line (an emote) are as in Legends of Azeroth
-- 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI' WHERE `entry` = 43003 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 43003;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(43003, 80277, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 43003 AND `SourceEntry` = 80277;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 43003, 80277, 0, 0, 9, 0, 26400, 0, 0, 0, 0, 0, '', 'Venture Co. Sawblade - only with The Universal Key');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43003 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43003, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Venture Co. Sawblade - On Spellclick - Say Line 0');

DELETE FROM `creature_text` WHERE `entry` = 43003;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43003, 0, 0, 'The blade cuts into the Mosh''Ogg Bounty, forcing it open!', 41, 0, 100, 0, 0, 0, 'Venture Co. Sawblade');
