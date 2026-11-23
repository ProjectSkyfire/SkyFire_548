-- Sunken Temple (map 109): SmartAI for Shade of Eranikus, Ogom the Wretched, Hukku, Dreamscythe and
-- Atal'alarion, copied from 4.3.4 smart_scripts (SkyFire's columns are identical). Changes against the 4.3.4 rows:
--  * event_flags: the 4.3.4 WHILE_CHARMED bit (0x200) is cleared; in SkyFire 0x200 is now the 40-man difficulty bit.
--    Difficulty bits stay as they are (0x2 normal 5-man matches SkyFire's spawnMask 2 on this map).
--  * Atal'alarion row 7 (Respawn Idol of Hakkar) is left out: gameobject guid 5521 does not exist in SkyFire or 4.3.4.
-- Gasher (5713) and Avatar of Hakkar (8443) have no SmartAI in 4.3.4 or 3.3.5, so they are not touched.

-- Enable SmartAI (only where no C++ script is bound).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (5709, 5711, 5715, 5721, 8580) AND `ScriptName` = '';

-- Texts used by the scripts above (SkyFire had none), from 4.3.4 creature_text.
DELETE FROM `creature_text` WHERE `entry` IN (5709, 5721, 8580);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(5709, 0, 0, 'This evil cannot be allowed to enter this world!  Come my children!', 14, 0, 100, 0, 0, 0, 'Shade of Eranikus'),
(5721, 0, 0, 'Turn back! Do not wake the dreamer!', 14, 0, 100, 0, 0, 5866, 'Dreamscythe'),
(5721, 1, 0, 'You know not what you do!  We must destroy you for your own good.', 14, 0, 100, 0, 0, 0, 'Dreamscythe'),
(8580, 0, 0, 'So or''manley fi siame Dim fus siame yudo', 14, 14, 100, 0, 0, 5859, 'Atal''alarion'),
(8580, 1, 0, 'You will rest with the honored dead.', 12, 0, 100, 0, 0, 0, 'Atal''alarion');

-- SmartAI rows.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (5709, 5711, 5715, 5721, 8580);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(5709, 0, 0, 0, 1, 0, 100, 5, 1000, 1000, 1000, 1000, 0, 90, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - OOC - Set Bytes_1 (No Repeat) (Normal Dungeon)'),
(5709, 0, 1, 2, 4, 0, 100, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - On Aggro - Say Line 0 (Normal Dungeon)'),
(5709, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 91, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - On Aggro - Remove Bytes_1 (Normal Dungeon)'),
(5709, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - On Aggro - Set In Combat With Zone (Normal Dungeon)'),
(5709, 0, 4, 0, 0, 0, 100, 4, 7200, 13300, 16900, 32100, 0, 11, 12891, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - In Combat - Cast ''Acid Breath'' (Normal Dungeon)'),
(5709, 0, 5, 0, 9, 0, 100, 4, 0, 5, 7300, 19200, 0, 11, 12787, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - Within 0-5 Range - Cast ''Thrash'' (Normal Dungeon)'),
(5709, 0, 6, 0, 0, 0, 100, 4, 14300, 14300, 28800, 39200, 0, 11, 12890, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - In Combat - Cast ''Deep Slumber'' (Normal Dungeon)'),
(5709, 0, 7, 0, 0, 0, 100, 4, 17000, 20000, 18000, 22000, 0, 11, 11876, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shade of Eranikus - In Combat - Cast ''War Stomp'' (Normal Dungeon)'),
(5711, 0, 0, 0, 1, 0, 100, 5, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Out of Combat - Disable Combat Movement'),
(5711, 0, 1, 0, 1, 0, 100, 5, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Out of Combat - Set Event Phase 0'),
(5711, 0, 2, 3, 4, 0, 100, 5, 0, 0, 0, 0, 0, 11, 12471, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - On Aggro - Cast ''Shadow Bolt'' (No Repeat) (Normal Dungeon)'),
(5711, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(5711, 0, 4, 0, 9, 1, 100, 4, 0, 40, 3400, 4800, 0, 11, 12471, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Within 0-40 Range - Cast ''Shadow Bolt'' (No Repeat) (Normal Dungeon)'),
(5711, 0, 5, 6, 3, 1, 100, 5, 0, 7, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Between 0-7% Mana - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 6, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Between 0-7% Mana - Increment Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 7, 0, 9, 1, 100, 5, 35, 80, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Within 35-80 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 8, 0, 9, 1, 100, 5, 5, 15, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Within 5-15 Range - Disable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 9, 0, 9, 1, 100, 5, 0, 5, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Within 0-5 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 10, 0, 3, 0, 100, 4, 15, 100, 100, 100, 0, 23, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - Between 15-100% Mana - Decrement Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 11, 0, 0, 0, 100, 4, 9100, 15200, 37900, 57700, 0, 11, 11639, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - In Combat - Cast ''Shadow Word: Pain'' (Phase 1) (No Repeat) (Normal Dungeon)'),
(5711, 0, 12, 0, 0, 0, 100, 4, 54300, 54300, 20700, 26200, 0, 11, 12493, 33, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ogom the Wretched - In Combat - Cast ''Curse of Weakness'' (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 0, 1, 1, 0, 100, 5, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Out of Combat - Disable Combat Movement (No Repeat) (Normal Dungeon)'),
(5715, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Out of Combat - Set Event Phase 0 (No Repeat) (Normal Dungeon)'),
(5715, 0, 2, 0, 1, 0, 100, 5, 1000, 1000, 0, 0, 0, 11, 12790, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Out of Combat - Cast ''Hukku''s Guardians'' (No Repeat) (Normal Dungeon)'),
(5715, 0, 3, 4, 4, 0, 100, 5, 0, 0, 0, 0, 0, 11, 12471, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hukku - On Aggro - Cast ''Shadow Bolt'' (No Repeat) (Normal Dungeon)'),
(5715, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - On Aggro - Increment Phase By 1 (No Repeat) (Normal Dungeon)'),
(5715, 0, 5, 0, 9, 1, 100, 4, 0, 40, 3400, 4800, 0, 11, 12471, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Within 0-40 Range - Cast ''Shadow Bolt'' (No Repeat) (Normal Dungeon)'),
(5715, 0, 6, 0, 9, 1, 100, 4, 0, 30, 16400, 29900, 0, 11, 9081, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Within 0-30 Range - Cast ''Shadow Bolt Volley'' (No Repeat) (Normal Dungeon)'),
(5715, 0, 7, 8, 3, 1, 100, 5, 0, 7, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Between 0-7% Mana - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 8, 0, 61, 1, 100, 0, 0, 0, 0, 0, 0, 23, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Between 0-7% Mana - Increment Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 9, 0, 9, 1, 100, 5, 35, 80, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Within 35-80 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 10, 0, 9, 1, 100, 5, 5, 15, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Within 5-15 Range - Disable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 11, 0, 9, 1, 100, 5, 0, 5, 0, 0, 0, 21, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Within 0-5 Range - Enable Combat Movement (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 12, 0, 3, 0, 100, 4, 15, 100, 100, 100, 0, 23, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hukku - Between 15-100% Mana - Decrement Phase By 1 (Phase 1) (No Repeat) (Normal Dungeon)'),
(5715, 0, 13, 0, 0, 0, 100, 4, 9400, 18200, 21200, 31300, 0, 11, 12279, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Hukku - In Combat - Cast ''Curse of Blood'' (Phase 1) (No Repeat) (Normal Dungeon)'),
(5721, 0, 0, 0, 10, 0, 100, 5, 0, 80, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreamscythe - Within 0-80 Range Out of Combat LoS - Say Line 0'),
(5721, 0, 1, 0, 4, 0, 100, 5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreamscythe - On Aggro - Say Line 1'),
(5721, 0, 2, 0, 0, 0, 100, 4, 2800, 6100, 8300, 17900, 0, 11, 12884, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Dreamscythe - In Combat - Cast ''Acid Breath'''),
(5721, 0, 3, 0, 0, 0, 100, 4, 8700, 13900, 19300, 34700, 0, 11, 12882, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Dreamscythe - In Combat - Cast ''Wing Flap'''),
(8580, 0, 0, 0, 11, 0, 100, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - On Respawn - Say Line 0 (No Repeat)'),
(8580, 0, 1, 2, 4, 0, 100, 5, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - On Aggro - Say Line 1 (No Repeat) (Normal Dungeon)'),
(8580, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 34, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - On Aggro - Set Instance Data 1 to 1 (No Repeat) (Normal Dungeon)'),
(8580, 0, 3, 0, 0, 0, 100, 4, 10700, 14900, 21600, 41600, 0, 11, 12887, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - In Combat - Cast ''Sweeping Slam'' (No Repeat) (Normal Dungeon)'),
(8580, 0, 4, 0, 0, 0, 100, 4, 9700, 18900, 20500, 32200, 0, 11, 6524, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - In Combat - Cast ''Ground Tremor'' (No Repeat) (Normal Dungeon)'),
(8580, 0, 5, 0, 6, 0, 100, 5, 0, 0, 0, 0, 0, 34, 1, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - On Just Died - Set Instance Data 1 to 3 (No Repeat) (Normal Dungeon)'),
(8580, 0, 6, 0, 7, 0, 100, 5, 0, 0, 0, 0, 0, 34, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Atal''alarion - On Evade - Set Instance Data 1 to 0 (No Repeat) (Normal Dungeon)');
