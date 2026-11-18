-- Deepholm: creatures that the Deepholm spawn update brings in whose template is still a placeholder at level 1 or at the
-- friendly faction 35, so they would stand level 1 and friendly (Young Stone Drakes, Fungal Horrors, Stone Trogg Rockmagi,
-- Needlerock Pummelers, Twilight Duskwardens, Jaspertip Crystal-gorgers and others). Level, experience group and faction
-- from the 5.4.8 data. Based on Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 2290, `faction_H` = 2290 WHERE `entry` = 42792; -- Earthguard Sentinel
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2167, `faction_H` = 2167 WHERE `entry` = 42889; -- Greater Air Elemental
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 42964; -- Mor'norokk the Hateful
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 0 WHERE `entry` = 43047; -- Flint Oremantle (the escort of "Take Him to the Earthcaller")
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2298, `faction_H` = 2298 WHERE `entry` = 43031; -- Agitated Tunneler
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2297, `faction_H` = 2297 WHERE `entry` = 43039; -- Twilight Duskwarden
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 43171; -- Jaspertip Crystal-gorger
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2102, `faction_H` = 2102 WHERE `entry` = 43237; -- Deadly Crystalgazer
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (43501, 43502, 48533); -- Boomshroom, Enormous Gyreworm, Fungal Horror
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 3, `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 43635; -- Catapult Driver
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (43652, 43847, 45099); -- Magmatooth, Needlerock Pummeller, Stone Trogg Rockmagus
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 0, `faction_A` = 2263, `faction_H` = 2263 WHERE `entry` = 43996; -- Reactivated War Construct
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 44038; -- Young Stone Drake
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2147, `faction_H` = 2147 WHERE `entry` = 44136; -- Zoltrik's Pyremaw
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2281, `faction_H` = 2281 WHERE `entry` = 44137; -- Crystalspire Stone Drake
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 82, `exp` = 0, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 44138; -- Jadecrest Basilisk
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 0, `faction_A` = 1735, `faction_H` = 1735 WHERE `entry` = 45397; -- Felhunter Minion
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 3, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 46561; -- Mercurial Droplet
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 48640; -- Enormous Gyreworm Bunny
