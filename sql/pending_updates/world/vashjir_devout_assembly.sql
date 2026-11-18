-- Vashj'ir, "Devout Assembly" (25896): gather Fathom-Caller Azrajar and six Sira'kess Tide Priestesses. The credit
-- spells 78267 and 78266 credit the targeted player, and nothing cast them. Azrajar has the gossip option of menu
-- 11572. The priestesses have the gossip menu 11571 in the 5.4.8 data, but no gossip flag here; each one leaves when
-- she gives her credit, so the same priestess cannot be counted twice.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (41980, 41985);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (41980, 41985);
UPDATE `creature_template` SET `gossip_menu_id` = 11571, `npcflag` = 1 WHERE `entry` = 41985;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41980, 0, 0, 1, 62, 0, 100, 0, 11572, 0, 0, 0, 0, 11, 78267, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Cast Quest Credit Azrajar on Invoker'),
(41980, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(41985, 0, 0, 1, 62, 0, 100, 0, 11571, 0, 0, 0, 0, 11, 78266, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Gossip Option 0 Selected - Cast Quest Credit Tide Priestess on Invoker'),
(41985, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Link - Close Gossip'),
(41985, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'On Link - Despawn');
