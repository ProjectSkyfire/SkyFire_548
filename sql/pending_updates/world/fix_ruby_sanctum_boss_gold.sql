-- Ruby Sanctum: the bosses dropped no gold, and the mini-boss loot tables (taken from Legends of Azeroth 5.4.8) total
-- 64 to 74 percent, so a loot window was often empty. Give them the gold they have in Legends of Azeroth 5.4.8
-- (normal 10-player templates and their 25-player difficulty entries).
UPDATE `creature_template` SET `mingold` = 822508, `maxgold` = 822508 WHERE `entry` IN (39746, 39805);
UPDATE `creature_template` SET `mingold` = 852249, `maxgold` = 852249 WHERE `entry` IN (39747, 39823);
UPDATE `creature_template` SET `mingold` = 828650, `maxgold` = 828650 WHERE `entry` IN (39751, 39920);
UPDATE `creature_template` SET `mingold` = 2063554, `maxgold` = 2063554 WHERE `entry` = 39863;
