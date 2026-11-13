-- "In Defense of Krom'gar Fortress" (26058) asks for 25 Gnome Wing Commanders (42017). Each one rides a Gnomish Bomber
-- (42015) in seat 0 and bails out with a parachute when the bomber is shot down. Based on Legends of Azeroth 5.4.8.
--
-- No commander ever existed: the server drops a vehicle accessory row when the vehicle has no spellclick row ("has no
-- data in npc_spellclick_spells" at start), and the commander boards through that row. Add it, as the Warsong War
-- Rider (34832) has. The accessory row is repeated here so this update works on its own.
DELETE FROM `vehicle_template_accessory` WHERE `entry` = 42015 AND `accessory_entry` = 42017;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(42015, 42017, 0, 0, 'Gnomish Bomber - Gnome Wing Commander', 6, 30000);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 42015 AND `spell_id` = 46598;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42015, 46598, 1, 0);

-- The accessory is not a minion, so the commander survives the bomber and is ejected into the air when it dies. The
-- script opens Parachute (79404) the moment the commander has left its seat (the seat aura 46598 is gone) and sends it to
-- the ground at its running speed, so the spot the client shows is the spot the server has. A commander that nobody
-- shoots is removed after 45 seconds.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 42017;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 42017;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42017, 0, 0, 1, 23, 0, 100, 65537, 46598, 0, 0, 0, 0, 11, 79404, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gnome Wing Commander - On seat aura gone - Cast Parachute'),
(42017, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gnome Wing Commander - On seat aura gone - Set event phase 2'),
(42017, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gnome Wing Commander - On seat aura gone - Set run'),
(42017, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 46, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gnome Wing Commander - On seat aura gone - Move down to the ground'),
(42017, 0, 4, 0, 60, 2, 100, 65537, 45000, 45000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gnome Wing Commander - 45 seconds after bailing out - Despawn');
