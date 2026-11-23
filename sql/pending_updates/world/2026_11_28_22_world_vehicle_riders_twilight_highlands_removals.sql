-- Twilight Highlands: three vehicles carry their rider on seat 0 as the vehicle accessory in Legends of Azeroth 5.4.8 but
-- had no accessory row here, so no rider was ever seated: Firebeard War Gryphon (48476, Firebeard Gryphon Rider 48477),
-- Wildhammer Stormtalon (49683, Stormtalon Rider 49684) and Earthen Ring Gryphon (50473, Earthen Ring Shaman 50472). All
-- three already have the Legends of Azeroth click row (46598, cast flags 1, user type 0) and the same vehicle id as
-- Legends of Azeroth, with a seat 0 in the client data.
-- The loose riders below have the orientation of their vehicle (within 0.01 radians), stand over it (0.13 to 0.33 yards
-- across and 1.66 to 1.76 high) in phase 0 and spawn mask 1. Their entries are never summoned (no summon action, summon
-- group, script or spell names them), they are not pooled, in an event, formation, addon, path or linked respawn, have
-- no smart script row, and no quest or vendor uses them. Legends of Azeroth lists the same loose riders (unverified,
-- VerifiedBuild 0, so weaker evidence) next to the same accessory rows. With the accessory in place the loose copy is
-- the second rider on the seat, so it goes.
-- Stormtalon riders: only the three within 1.7 yards of a stormtalon spawn point go. The other 43 rows are further than
-- 2.6 yards from any stormtalon (the stormtalons fly paths) and stay until they can be matched to a vehicle.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (48476, 49683, 50473);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(48476, 48477, 0, 1, 'Earthen Ring Shaman', 6, 30000),
(49683, 49684, 0, 1, 'Wildhammer Stormtalon', 6, 30000),
(50473, 50472, 0, 1, 'Earthen Ring Shaman', 6, 30000);

-- Firebeard War Gryphon guid and the loose Firebeard Gryphon Rider guid it carries (1.73 yards, same orientation):
-- 8253087/8253101, 8253088/8253102, 8253089/8253103, 8253090/8253104, 8253091/8253105, 8253092/8253106,
-- 8253093/8253107, 8253094/8253108, 8253095/8253109, 8253096/8253110, 8253097/8253111, 8253098/8253112,
-- 8253099/8253113, 8253100/8253114.
DELETE FROM `creature` WHERE `guid` IN (8253101, 8253102, 8253103, 8253104, 8253105, 8253106, 8253107, 8253108, 8253109, 8253110, 8253111, 8253112, 8253113, 8253114) AND `id` = 48477;

-- Wildhammer Stormtalon guid and the loose Stormtalon Rider guid it carries (same orientation): 8252545/8252613 (1.69 yards),
-- 8252563/8252610 (1.67 yards; stormtalon 8252552 is 2.3 yards from the same rider and gets its own seated rider),
-- 8252565/8252612 (1.67 yards).
DELETE FROM `creature` WHERE `guid` IN (8252610, 8252612, 8252613) AND `id` = 49684;

-- Earthen Ring Gryphon guid and the loose Earthen Ring Shaman guid it carries (1.78 yards, same orientation):
-- 8248728/8248736, 8248729/8248737, 8248730/8248738, 8248731/8248739, 8248732/8248740, 8248733/8248741,
-- 8248734/8248742, 8248735/8248743.
DELETE FROM `creature` WHERE `guid` IN (8248736, 8248737, 8248738, 8248739, 8248740, 8248741, 8248742, 8248743) AND `id` = 50472;
