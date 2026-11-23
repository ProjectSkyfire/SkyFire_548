-- Silverpine Forest: the Horde Haulers (44731) and Horde Coffin Haulers (44764) stand next to their Subdued Forest
-- Ettin (44737 and 46560) as separate spawns, so the ettin never moves with the cart. As in Legends of Azeroth 5.4.8
-- the ettin is the vehicle accessory of the cart on seat 1 (the front yoke seat, VehicleSeat 8391 and 8399, attachment
-- 15) and the loose spawns are gone.
-- Every loose ettin below is 6 to 7 yards in front of a hauler of the same orientation (9 pairs, 5 haulers and 4 coffin
-- haulers). Neither entry is summoned by a script, pooled or referenced by a formation, addon, path or condition.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (44731, 44764);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(44731, 44737, 1, 1, 'Horde Hauler - Subdued Forest Ettin', 8, 0),
(44764, 46560, 1, 1, 'Horde Coffin Hauler - Subdued Forest Ettin', 8, 0);

-- Kept hauler and loose ettin pairs, all 6.7 yards apart with the same orientation: 195587 and 195588, 195856 and
-- 195855, 196348 and 196350, 196877 and 196878, 204562 and 204564 (haulers); 195549 and 195550, 195896 and 195898,
-- 204941 and 204940, 205605 and 205604 (coffin haulers).
DELETE FROM `creature` WHERE `guid` IN (195588, 195855, 196350, 196878, 204564, 195550, 195898, 204940, 205604) AND `id` IN (44737, 46560);
