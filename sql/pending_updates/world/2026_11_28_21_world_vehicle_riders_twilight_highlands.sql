-- Twilight Highlands: four vehicles carry their rider on seat 0 as the vehicle accessory in Legends of Azeroth 5.4.8 but
-- had no accessory row here, so no rider was ever seated: Thundermar War Gryphon (47186, Thundermar Gryphon Rider
-- 46321) and the Dragonmaw Black Drakes (48197, 50599 and 52025, Dragonmaw Skyclaw 48194, 50600 and 52026). All four
-- already have the Legends of Azeroth click row (46598, cast flags 1, user type 0) and the same vehicle id as Legends of
-- Azeroth, with a seat 0 in the client data.
-- The Thundermar riders are spawned with spawn mask 0 and never appear; they stay as they are. The Skyclaws of the
-- three drakes have no loose rider, apart from two Skyclaws 28 and 40 yards from drakes, which are separate spawns, so
-- no spawn is removed.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (47186, 48197, 50599, 52025);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(47186, 46321, 0, 0, 'Thundermar War Gryphon', 6, 30000),
(48197, 48194, 0, 0, 'Dragonmaw Skyclaw on Dragonmaw Black Drake', 6, 30000),
(50599, 50600, 0, 0, 'Dragonmaw Skyclaw on Dragonmaw Black Drake', 6, 30000),
(52025, 52026, 0, 0, 'Dragonmaw Skyclaw on Dragonmaw Black Drake', 6, 30000);
