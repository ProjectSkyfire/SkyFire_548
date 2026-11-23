-- The Battle for Andorhal (28749, Alliance): War-Mage Erallier in Booty Bay is to prepare a portal to Andorhal for the
-- player, and nothing happened when the gossip option was chosen. Erallier had no script, the Portal to Andorhal
-- (49638) could not be clicked and the portal spell (92581) had no destination. Choosing the option now closes the
-- gossip and summons the portal for 30 seconds; using it teleports to the Alliance camp at Thassarian, the
-- destination of the source.
-- Rows are as in Legends of Azeroth 5.4.8; its phase switch and timer on the summon change nothing (no row has a phase
-- mask) and are left out. The Horde counterpart, Arcanist Arman (28750), has no script, gossip menu or destination in
-- that source and is not covered here.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 49635 AND `ScriptName` = '';
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` = 49638;

DELETE FROM `smart_scripts` WHERE `entryorguid` = 49635 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(49635, 0, 0, 1, 62, 0, 100, 0, 12511, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'War-Mage Erallier - On Gossip Select - Close Gossip'),
(49635, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 49638, 3, 30000, 0, 0, 0, 8, 0, 0, 0, -14470.9, 469.327, 30.736, 0, 'War-Mage Erallier - Linked To Id 0 - Summon Portal to Andorhal');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 49638;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(49638, 92581, 1, 0);

DELETE FROM `spell_target_position` WHERE `id` = 92581 AND `effIndex` = 0;
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(92581, 0, 0, 1350.84, -1339.26, 50.2147, 4.46185);
