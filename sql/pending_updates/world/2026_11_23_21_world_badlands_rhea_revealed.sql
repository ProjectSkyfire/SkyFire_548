-- Rhea Revealed (27769) cannot be finished. The credit is Rhea (46654) herself: after the quest is accepted she tells the
-- player about the red dragons' plan and, seven seconds later, the credit is given. Rhea had no SmartAI and no speech,
-- so nothing happened after accepting and the quest, which the rest of the Rheastrasza chain waits for, stayed open.
-- Rows and speech are as in Legends of Azeroth 5.4.8. The source gives the credit to the invoker of a timer, which this
-- core does not pass on, so the player is stored when the quest is accepted and credited from the store.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 46654 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` = 46654 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(46654, 0, 0, 1, 19, 0, 100, 0, 27769, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rhea - On Quest Accepted - Say Line 0'),
(46654, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rhea - On Quest Accepted - Store Target'),
(46654, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1, 7000, 7000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rhea - On Quest Accepted - Create Timed Event 1'),
(46654, 0, 3, 0, 59, 0, 100, 0, 1, 0, 0, 0, 0, 33, 46654, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Rhea - On Timed Event 1 - Quest Credit');

DELETE FROM `creature_text` WHERE `entry` IN (46654);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46654, 0, 0, 'Deathwing''s madness overcomes him, and we, the red dragons, must take this opportunity to steal his black dragons from beneath him.  We will remake the black dragons as they were intended to be: the warders of the earth.', 12, 0, 100, 5, 0, 0, '');
