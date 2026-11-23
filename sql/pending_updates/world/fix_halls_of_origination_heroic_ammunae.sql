-- Heroic Ammunae (48715) was a level 1 friendly placeholder that the normal template did not link, so heroic ran the
-- normal boss with the normal loot, and the heroic items were in no table. Link it, give it the normal template's
-- level, faction and melee values with the heroic damage multiplier (4.5, as on the other heroic bosses of this dungeon;
-- its Health_mod 35 already gives the heroic health listed on Wowhead), and add the five heroic items as one group,
-- the same shape as the normal table.

UPDATE `creature_template` SET `difficulty_entry_1` = 48715 WHERE `entry` = 39731;

UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3, `faction_A` = 16, `faction_H` = 16, `speed_walk` = 2, `speed_run` = 1.42857, `mindmg` = 4184, `maxdmg` = 7114, `dmg_multiplier` = 4.5, `baseattacktime` = 2000, `rangeattacktime` = 2000, `unit_class` = 4, `unit_flags` = 32832, `lootid` = 48715, `VehicleId` = 758, `mingold` = 19997, `maxgold` = 19997, `flags_extra` = 1, `detection_range` = 20 WHERE `entry` = 48715;

DELETE FROM `creature_loot_template` WHERE `entry` = 48715;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48715, 56417, 0, 1, 1, 1, 1),
(48715, 56418, 0, 1, 1, 1, 1),
(48715, 56419, 0, 1, 1, 1, 1),
(48715, 56420, 0, 1, 1, 1, 1),
(48715, 56421, 0, 1, 1, 1, 1);
