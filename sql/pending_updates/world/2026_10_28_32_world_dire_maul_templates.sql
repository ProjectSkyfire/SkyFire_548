-- Dire Maul: set the levels of the Gordok wing and related templates to the 4.3.4 values (Cataclysm re-levelled the
-- dungeon to 36 to 47; these eight templates were still at their 3.3.5 levels in SkyFire). Each statement is guarded by the
-- current levels, so the file is safe to run twice. Doomguard Minion (14385) already matches 4.3.4 and is not touched.
UPDATE `creature_template` SET `minlevel` = 47, `maxlevel` = 47 WHERE `entry` = 11501 AND `minlevel` = 62 AND `maxlevel` = 62; -- King Gordok
UPDATE `creature_template` SET `minlevel` = 47, `maxlevel` = 47 WHERE `entry` = 14325 AND `minlevel` = 61 AND `maxlevel` = 61; -- Captain Kromcrush
UPDATE `creature_template` SET `minlevel` = 46, `maxlevel` = 46 WHERE `entry` = 14324 AND `minlevel` = 60 AND `maxlevel` = 60; -- Cho'Rush the Observer
UPDATE `creature_template` SET `minlevel` = 45, `maxlevel` = 45 WHERE `entry` = 11446 AND `minlevel` = 60 AND `maxlevel` = 60; -- Gordok Spirit
UPDATE `creature_template` SET `minlevel` = 43, `maxlevel` = 43 WHERE `entry` = 11467 AND `minlevel` = 59 AND `maxlevel` = 59; -- Tsu'zee
UPDATE `creature_template` SET `minlevel` = 39, `maxlevel` = 39 WHERE `entry` = 14350 AND `minlevel` = 57 AND `maxlevel` = 57; -- Hydroling
UPDATE `creature_template` SET `minlevel` = 46, `maxlevel` = 46 WHERE `entry` = 14351 AND `minlevel` = 58 AND `maxlevel` = 59; -- Gordok Bushwacker
UPDATE `creature_template` SET `minlevel` = 42, `maxlevel` = 42 WHERE `entry` = 14396 AND `minlevel` = 61 AND `maxlevel` = 61; -- Eye of Immol'thar
