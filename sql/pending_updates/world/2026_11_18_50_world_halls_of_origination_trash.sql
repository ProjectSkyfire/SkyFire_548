-- Halls of Origination trash: the Sun-Touched Servant, the Flux Animator, the Lifewarden Nymph, the Void Lord, the Temple Runecaster and the Temple
-- Fireshaper had no AI and so no abilities. They get the Smart AI scripts of the 5.4.8 data (event flags 2 and 4: normal and heroic), where the row of
-- the Temple Fireshaper that carries the Molten Barrier at 35% health used the id of a harpoon summon instead of 89542.
-- The heroic copies of the trash that drops items had no loot table (only their gold) and now use the table of their normal entry.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (39366, 40033, 40715, 41364, 48140, 48143) AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (39366, 40033, 40715, 41364, 48140, 48143);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39366, 0, 0, 0, 2, 0, 100, 13, 1, 40, 0, 0, 0, 11, 88097, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sun-Touched Servant - At 40% HP - Cast Disperse'),
(39366, 0, 1, 0, 0, 0, 100, 12, 5000, 5000, 15000, 16000, 0, 11, 76160, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Sun-Touched Servant - In Combat - Cast Flame Dispersion'),
(39366, 0, 2, 0, 0, 0, 100, 12, 4000, 7000, 10000, 18500, 0, 11, 74101, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Sun-Touched Servant - In Combat - Cast Searing Flames'),
(40033, 0, 0, 0, 0, 0, 100, 12, 0, 0, 3400, 4700, 0, 11, 81013, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Flux Animator - In Combat - Cast Arcane Barrage'),
(40715, 0, 0, 0, 0, 0, 100, 12, 0, 0, 2300, 3900, 0, 11, 75958, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lifewarden Nymph - In Combat - Cast Shoot'),
(40715, 0, 1, 0, 0, 0, 100, 12, 5000, 6000, 12000, 14000, 0, 11, 75961, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Lifewarden Nymph - In Combat - Cast Entangling Shot'),
(40715, 0, 2, 0, 2, 0, 100, 12, 0, 40, 14000, 16000, 0, 11, 75940, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lifewarden Nymph - At 40% HP - Cast Tranquility'),
(41364, 0, 0, 1, 4, 0, 100, 13, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Void Lord - On Aggro - Stop Moving'),
(41364, 0, 1, 0, 61, 0, 100, 13, 0, 0, 0, 0, 0, 11, 77470, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Void Lord - On Link - Cast Void Infusion'),
(41364, 0, 2, 0, 0, 0, 100, 12, 34000, 35000, 45000, 48000, 0, 11, 76146, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Void Lord - In Combat - Cast Shadow Bolt Volley'),
(41364, 0, 3, 0, 0, 0, 100, 12, 55000, 58000, 67000, 75000, 0, 11, 77475, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Void Lord - In Combat - Cast Void Burst'),
(48140, 0, 0, 0, 9, 0, 100, 12, 0, 5, 9000, 11000, 0, 11, 89554, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Temple Runecaster - On Close - Cast Runic Cleave'),
(48140, 0, 1, 0, 0, 0, 100, 12, 5000, 7500, 14500, 18000, 0, 11, 89551, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Temple Runecaster - In Combat - Cast Curse of the Runecaster'),
(48143, 0, 0, 0, 2, 0, 100, 13, 0, 35, 0, 0, 0, 11, 89542, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Temple Fireshaper - At 35% HP - Cast Molten Barrier'),
(48143, 0, 1, 0, 0, 0, 100, 12, 8000, 8000, 25000, 25000, 0, 11, 89547, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Temple Fireshaper - In Combat - Cast Blazing Eruption'),
(48143, 0, 2, 0, 0, 0, 100, 12, 4000, 6000, 17500, 18500, 0, 11, 84032, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 'Temple Fireshaper - In Combat - Cast Meteor'),
(48143, 0, 3, 0, 0, 0, 100, 12, 0, 0, 3400, 4700, 0, 11, 89538, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Temple Fireshaper - In Combat - Cast Fireball');

UPDATE `creature_template` SET `lootid` = 48141 WHERE `entry` = 49309 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 48140 WHERE `entry` = 49308 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 48139 WHERE `entry` = 49307 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 48143 WHERE `entry` = 49310 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40668 WHERE `entry` = 49299 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 39440 WHERE `entry` = 49286 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40310 WHERE `entry` = 49294 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40787 WHERE `entry` = 49301 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40311 WHERE `entry` = 49295 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40033 WHERE `entry` = 49291 AND `lootid` = 0;
UPDATE `creature_template` SET `lootid` = 40715 WHERE `entry` = 49300 AND `lootid` = 0;
