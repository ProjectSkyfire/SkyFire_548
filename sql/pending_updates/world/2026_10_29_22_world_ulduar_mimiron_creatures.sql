-- Ulduar: Mimiron creature data. The 25 player vehicles were level 80 with a different faction and no crowd control
-- immunity, the Emergency Fire Bots, the Frost Bomb and the 25 player Junk Bot were friendly, and the template addons
-- (bot, flame and bomb auras, flying Aerial Command Unit) were missing.

-- Leviathan Mk II (10 / 25 player), instance bound
UPDATE `creature_template` SET `mechanic_immune_mask` = 646135679, `flags_extra` = 524321 WHERE `entry` IN (33432, 34106);
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 34106;
-- VX-001 (10 / 25 player), its loot belongs to the Mk II only
UPDATE `creature_template` SET `mechanic_immune_mask` = 646135679, `flags_extra` = 524320, `lootid` = 0 WHERE `entry` IN (33651, 34108);
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 34108;
-- Aerial Command Unit (10 / 25 player)
UPDATE `creature_template` SET `mechanic_immune_mask` = 646135679, `flags_extra` = 524320, `HoverHeight` = 15 WHERE `entry` IN (33670, 34109);
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 34109;

-- Junk Bot (25), Emergency Fire Bot (10 / 25) and Frost Bomb are hostile; the Frost Bomb is not selectable
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (34114, 34147, 34148, 34149);
UPDATE `creature_template` SET `unit_flags` = 33554432 WHERE `entry` = 34149;
-- Flames are not immune to player characters, they have to hurt them
UPDATE `creature_template` SET `unit_flags` = 33554432 WHERE `entry` IN (34121, 34363);
-- The Mk II turret is only ever fired by the script
UPDATE `creature_template` SET `AIName` = 'PassiveAI' WHERE `entry` = 34071 AND `AIName` = '';

-- Template addons: Aerial Command Unit (flying), Bomb Bot aura, Flames, Emergency Fire Bot and Frost Bomb auras.
DELETE FROM `creature_template_addon` WHERE `entry` IN (33670, 34109, 33836, 34218, 34121, 34363, 34147, 34148, 34149);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(33670, 0, 0, 50331648, 1, 0, NULL),
(34109, 0, 0, 50331648, 1, 0, NULL),
(33836, 0, 0, 0, 1, 0, '63767'),
(34218, 0, 0, 0, 1, 0, '63767'),
(34121, 0, 0, 0, 1, 0, '64561'),
(34363, 0, 0, 0, 1, 0, '64561'),
(34147, 0, 0, 0, 1, 0, '64617'),
(34148, 0, 0, 0, 1, 0, '64616 64617'),
(34149, 0, 0, 0, 1, 0, '64624');

-- A second, static Mk II turret hovered above the Mk II; the vehicle spawns the turret itself.
DELETE FROM `creature_addon` WHERE `guid` = 137619;
DELETE FROM `creature` WHERE `guid` = 137619 AND `id` = 34071;
-- Mimiron DB Target, Spinning Up aims at it
UPDATE `creature` SET `position_x` = 2785.42, `position_y` = 2673.12 WHERE `guid` = 137620 AND `id` = 33576;
