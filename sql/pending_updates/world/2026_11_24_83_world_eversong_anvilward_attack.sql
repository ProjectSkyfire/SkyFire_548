-- The Dwarven Spy (8483): Prospector Anvilward (15420) turns hostile at waypoint 7 of the walk to the North Sanctum but
-- nothing starts the fight, so he stands there until he is hit, and his faction and gossip flag are not restored if the
-- fight is abandoned. A waypoint event has no invoker, so the player who picked the gossip option is stored (Store Target
-- List 1) and attacked at waypoint 7, and on reset and on evade the default faction and the gossip flag come back.
-- Rows are as in the Legends of Azeroth update 2026_11_08_00 for this script: the old row 8 (Say Line 2) becomes row 9.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 15420 AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15420, 0, 0, 1, 62, 0, 100, 0, 21311, 0, 0, 0, 0, 53, 0, 15420, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Gossip Select - Start WP'),
(15420, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Gossip Select - Start WP'),
(15420, 0, 2, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Gossip Select - Remove npcflag'),
(15420, 0, 3, 0, 40, 0, 100, 0, 1, 15420, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On WP 1 - Say Line 0'),
(15420, 0, 4, 5, 40, 0, 100, 0, 6, 15420, 0, 0, 0, 54, 2500, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On WP 6 - Pause Path'),
(15420, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On WP 6 - Say Line 1'),
(15420, 0, 6, 7, 40, 0, 100, 0, 7, 15420, 0, 0, 0, 54, 15000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On WP 7 - Pause Path'),
(15420, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 24, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On WP 7 - Set Faction'),
(15420, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Waypoint 7 Reached - Start Attacking Stored Target'),
(15420, 0, 9, 0, 1, 0, 100, 1, 10000, 10000, 24000, 29000, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - Out of Combat - Say Line 2'),
(15420, 0, 10, 11, 25, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Reset - Set Faction Default'),
(15420, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Reset - Add Npc Flag Gossip'),
(15420, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Gossip Option 0 Selected - Store Target List (invoker)'),
(15420, 0, 13, 14, 7, 0, 100, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Evade - Set Faction Default'),
(15420, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 82, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Prospector Anvilward - On Evade - Add Npc Flag Gossip');
