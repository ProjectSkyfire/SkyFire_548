-- Saving Yenniku (26305) and Nighttime in the Jungle (26304, 26780): Yenniku (2530) is bound to Ana'thek the Cruel. When
-- Ana'thek dies, Yenniku is told to wake up, says his line and attacks the player; the player's Soul Gem (item 3912, spell
-- 3607 "Yenniku's Release") then stuns him and makes the Filled Soul Gem (3913) the quest asks for.
-- Yenniku had only the old C++ script (npc_yenniku, written for the removed quest 592), which does nothing for 26305, and
-- no AI rows, and Ana'thek never woke him, so no Filled Soul Gem could be made. The template also had the flag "immune to
-- players", which would have blocked the Soul Gem. The rows, the speech and the template values (unit flags 32768, civilian
-- extra flag, so he does not attack on sight) are as in Legends of Azeroth 5.4.8; its "Combat Stop" action is number 203 in
-- that source and 27 here. The source lets the Soul Gem hit Yenniku only in his attack phase through a phase condition
-- this core does not have; the SmartAI rows already ignore the spell in the other phases.
-- The Zul'Mamwe Brazier (204247) of Nighttime in the Jungle had no script: as in the source it now despawns nine
-- seconds after it is lit (the source marks the row as not repeatable, which would stop it working after the first
-- respawn, so the flag is cleared). Ana'thek keeps his Head Crack row, the new rows follow it.

UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '', `unit_flags` = 32768, `flags_extra` = 2 WHERE `entry` = 2530 AND `ScriptName` = 'npc_yenniku';
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 1059 AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 204247 AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 1059 AND `id` IN (1, 2, 3, 4);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2530 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204247 AND `id` IN (0);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(1059, 0, 1, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ana''thek the Cruel - On Aggro - Store Target List'),
(1059, 0, 2, 3, 6, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ana''thek the Cruel - On Death - Say Line 0'),
(1059, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 2530, 30, 0, 0, 0, 0, 0, 'Ana''thek the Cruel - Linked - Set Data 0 1'),
(1059, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 100, 1, 0, 0, 0, 0, 0, 19, 2530, 30, 0, 0, 0, 0, 0, 'Ana''thek the Cruel - Linked - Send Target To Target'),
(2530, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - On Respawn - Set Phase 0'),
(2530, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 0 - Set Faction Default'),
(2530, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 431, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 1 - Set Emote State 431'),
(2530, 0, 3, 4, 38, 0, 100, 0, 0, 1, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - On Data Set 0 1 - Set Phase 1'),
(2530, 0, 4, 5, 61, 1, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 3 - Set Emote State 0'),
(2530, 0, 5, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 4 - Say Line 0'),
(2530, 0, 6, 0, 52, 1, 100, 0, 0, 2530, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Yenniku - On Text Over 0 - Attack Start'),
(2530, 0, 7, 8, 8, 1, 100, 0, 3607, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - On SpellHit Yenniku''s Release - Combat Stop'),
(2530, 0, 8, 9, 61, 1, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 7 - Set Phase 2'),
(2530, 0, 9, 10, 61, 2, 100, 0, 0, 0, 0, 0, 0, 2, 83, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 8 - Set Faction 83'),
(2530, 0, 10, 11, 61, 2, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 9 - Say Line 1'),
(2530, 0, 11, 12, 61, 2, 100, 0, 0, 0, 0, 0, 0, 56, 3913, 1, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 10 - Add Item Filled Soul Gem'),
(2530, 0, 12, 13, 61, 2, 100, 0, 0, 0, 0, 0, 0, 75, 78320, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 11 - Add Aura Stunned'),
(2530, 0, 13, 0, 61, 2, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Yenniku - Linked To Id 12 - Despawn after 10 sec'),
(204247, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 41, 9000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Zul''Mamwe Brazier - On GO Change State - Despawn After 9 Seconds');

DELETE FROM `creature_text` WHERE `entry` IN (1059, 2530);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(1059, 0, 0, 'Skullsplitter... is... forever... mon...', 12, 0, 100, 0, 0, 0, 'Ana''thek the Cruel'),
(2530, 0, 0, 'Zanzil is creating an army that will wash over all of Stranglethorn!  Bloodscalps, Skullsplitter, Gurubashi and Horde... all will serve Zanzil!', 12, 0, 100, 0, 0, 0, 'Yenniku'),
(2530, 1, 0, '%s is struck dumb by the Soul Gem!', 16, 0, 100, 0, 0, 0, 'Yenniku');
