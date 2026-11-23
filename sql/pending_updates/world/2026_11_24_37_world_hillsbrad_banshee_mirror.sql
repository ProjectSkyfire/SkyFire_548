-- Cry of the Banshee (28538): the Banshee Mirror (spell 90976) is used on a wounded SI:7 agent and ten of them count as
-- captured. The agents (SI:7 Operative 48741, SI:7 Assassin 48742) gave no credit and the summoned Banshee (48752) had no
-- script. Added as in Legends of Azeroth 5.4.8: the agents credit the Banshee entry to whoever hits them with the mirror
-- (their combat rows keep ids 0 to 2, the new row follows), and the Banshee runs to the player, speaks and despawns.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 48752;

DELETE FROM `creature_text` WHERE `entry` IN (48752);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48752, 0, 0, 'Begone infidels, you are not welcome here!', 12, 0, 100, 0, 0, 19735, ''),
(48752, 0, 1, 'Death is only the beginning!', 12, 0, 100, 0, 0, 19733, ''),
(48752, 0, 2, 'May peace find you now.', 12, 0, 100, 0, 0, 19738, ''),
(48752, 0, 3, 'The heavens take you!', 12, 0, 100, 0, 0, 19736, '');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48741 AND `source_type` = 0 AND `id` IN (3);
DELETE FROM `smart_scripts` WHERE `entryorguid` = 48742 AND `source_type` = 0 AND `id` IN (3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48741, 0, 3, 0, 8, 0, 100, 0, 90976, 0, 0, 0, 0, 33, 48752, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'SI:7 Operative - On Spellhit - Killcredit'),
(48742, 0, 3, 0, 8, 0, 100, 0, 90976, 0, 0, 0, 0, 33, 48752, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'SI:7 Assassin - On Spellhit - Killcredit');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48752 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3, 4);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48752, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Banshee - On Summon - Set Immune'),
(48752, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Banshee - On Summon - Set Run'),
(48752, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Banshee - On Summon - Move to Pos'),
(48752, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Banshee - On Summon - Say Line 0'),
(48752, 0, 4, 0, 52, 0, 100, 0, 0, 48752, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Banshee - On Text Over 0 - Despawn');
