-- Healing Rain Invisible Stalker DND: the invisible creature at the Healing Rain spot that casts the rain visual (147490).
DELETE FROM `creature_template` WHERE `entry` = 73400;
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `name`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `speed_walk`, `speed_run`, `scale`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`, `type_flags`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `movementId`, `RegenHealth`, `flags_extra`, `WDBVerified`) VALUES
(73400, 1126, 11686, 'Healing Ran Invisible Stalker DND', 90, 90, 4, 35, 35, 1, 2, 1, 2000, 2000, 2, 33570816, 2048, 10, 1024, 3, 1, 1, 1, 1, 1, 187, 1, 128, 18414);
