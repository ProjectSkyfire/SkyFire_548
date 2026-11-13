-- Heroic Siamat (51088) was a level 1 friendly placeholder that the normal template did not link, so heroic ran the
-- normal boss with the normal loot, and the heroic items were in no table. Link it, give it the normal template's
-- level, faction and melee values with the heroic damage multiplier (4.5, as on the other heroic bosses of this dungeon;
-- its Health_mod 30 already gives the heroic health listed on Wowhead), and add the ten heroic items in two groups of
-- five (the shape of the normal table, with the same item in the same group) plus the Chaos Orb that the other heroic
-- final bosses of Cataclysm dungeons drop, for example Rajh.

UPDATE `creature_template` SET `difficulty_entry_1` = 51088 WHERE `entry` = 44819;

UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3, `faction_A` = 16, `faction_H` = 16, `speed_walk` = 2.8, `speed_run` = 1.42857, `mindmg` = 4184, `maxdmg` = 7114, `dmg_multiplier` = 4.5, `baseattacktime` = 2000, `rangeattacktime` = 2000, `unit_class` = 2, `unit_flags` = 32832, `lootid` = 51088, `flags_extra` = 1, `detection_range` = 20 WHERE `entry` = 51088;

DELETE FROM `creature_loot_template` WHERE `entry` = 51088;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(51088, 52078, 100, 1, 0, 1, 1),
(51088, 56394, 0, 1, 1, 1, 1),
(51088, 56396, 0, 1, 1, 1, 1),
(51088, 56398, 0, 1, 1, 1, 1),
(51088, 56401, 0, 1, 1, 1, 1),
(51088, 56402, 0, 1, 1, 1, 1),
(51088, 56395, 0, 1, 2, 1, 1),
(51088, 56397, 0, 1, 2, 1, 1),
(51088, 56399, 0, 1, 2, 1, 1),
(51088, 56400, 0, 1, 2, 1, 1),
(51088, 56403, 0, 1, 2, 1, 1);
