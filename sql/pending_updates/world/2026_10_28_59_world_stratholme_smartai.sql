-- Stratholme: SmartAI and creature texts the dungeon was missing. Source: 3.3.5 or 4.3.4, whichever has more rows for the creature
-- (the two sets agree where both exist; the dungeon was re-levelled by Cataclysm). Columns are identical to SkyFire's. event_flags are kept, minus the 4.3.4
-- WHILE_CHARMED bit (0x200), which is the 40-man difficulty bit in SkyFire. Every spell used was checked against the 5.4.8 spell data.
-- Creatures with a C++ ScriptName keep it. Safe to run twice: DELETE before INSERT by key.

-- Enable SmartAI only where no C++ script is bound.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (10382, 10405, 10406, 10407, 10411, 10422, 10424, 10425, 10558, 11032, 16046) AND `ScriptName` = '';

-- Texts missing in SkyFire (existing rows of the same entries are left alone).
DELETE FROM `creature_text` WHERE (`entry` = 10382 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10405 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10411 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10440 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10440 AND `groupid` = 1 AND `id` = 0) OR (`entry` = 10812 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10813 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 10813 AND `groupid` = 1 AND `id` = 0) OR (`entry` = 16031 AND `groupid` = 0 AND `id` = 0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(10382, 0, 0, '%s collapses but the broken body rises again!', 16, 0, 100, 0, 0, 0, 'Mangled Cadaver'),
(10405, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Plague Ghoul'),
(10411, 0, 0, 'The living are here!', 14, 0, 100, 0, 0, 6596, 'Eye of Naxxramas'),
(10440, 0, 0, '%s raises an undead servant back to life!', 16, 0, 0, 0, 0, 0, 'Baron Rivendare (Stratholme) - EMOTE_RAISE_DEAD'),
(10440, 1, 0, '%s attempts to cast Death Pact on his servants!', 16, 0, 0, 0, 0, 0, 'Baron Rivendare (Stratholme) - EMOTE_DEATH_PACT'),
(10812, 0, 0, 'Today you have unmade what took me years to create! For this you shall all die by my hand!', 14, 0, 100, 0, 0, 0, 'Grand Crusader Dathrohan SAY_AGGRO'),
(10813, 0, 0, 'You fools think you can defeat me so easily? Face the true might of the Nathrezim!', 14, 0, 100, 0, 0, 0, 'Balnazzar SAY_TRANSFORM'),
(10813, 1, 0, 'Damn you mortals! All my plans of revenge, all my hate...I will be avenged...', 12, 0, 100, 0, 0, 0, 'Balnazzar SAY_DEATH'),
(16031, 0, 0, 'You did it... you''ve slain Baron Rivendare!  The Argent Dawn shall hear of your valiant deeds!', 12, 0, 0, 0, 0, 0, '');

-- Creature SmartAI rows.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (10382, 10405, 10406, 10407, 10411, 10422, 10424, 10425, 10558, 11032, 16046);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(10382, 0, 0, 0, 0, 0, 100, 5, 0, 0, 30000, 30000, 0, 11, 16143, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mangled Cadaver - In Combat - Cast ''Cadaver Worms'' (No Repeat) (Normal Dungeon)'),
(10382, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mangled Cadaver - On Just Died - Say Line 0'),
(10405, 0, 0, 0, 0, 0, 100, 5, 0, 0, 40000, 40000, 0, 11, 16458, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plague Ghoul - In Combat - Cast ''Ghoul Plague'''),
(10405, 0, 1, 0, 0, 0, 100, 4, 7000, 7000, 8000, 8000, 0, 11, 40505, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Plague Ghoul - In Combat - Cast ''Cleave'''),
(10405, 0, 2, 3, 2, 0, 100, 5, 0, 30, 0, 0, 0, 11, 8599, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Ghoul - Between 0-30% Health - Cast ''Enrage'' (No Repeat) (Normal Dungeon)'),
(10405, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Ghoul - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(10405, 0, 4, 0, 1, 0, 100, 0, 3000, 5000, 8000, 12000, 0, 5, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plague Ghoul - Out of Combat - Play Emote 1'),
(10406, 0, 0, 0, 0, 0, 100, 4, 5000, 5000, 5000, 5000, 0, 11, 16172, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ghoul Ravener - In Combat - Cast ''Head Crack'' (No Repeat) (Normal Dungeon)'),
(10406, 0, 1, 0, 0, 0, 100, 4, 7000, 7000, 8000, 8000, 0, 11, 16553, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ghoul Ravener - In Combat - Cast ''Ravenous Claw'' (No Repeat) (Normal Dungeon)'),
(10406, 0, 2, 0, 1, 0, 100, 0, 3000, 5000, 8000, 12000, 0, 5, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ghoul Ravener - Out of Combat - Play Emote 1'),
(10407, 0, 0, 0, 0, 0, 75, 4, 5000, 5000, 6000, 6000, 0, 11, 12787, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleshflayer Ghoul - In Combat - Cast ''Thrash'' (No Repeat) (Normal Dungeon)'),
(10407, 0, 1, 0, 0, 0, 100, 4, 3000, 3000, 20000, 20000, 0, 11, 13738, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fleshflayer Ghoul - In Combat - Cast ''Rend'' (No Repeat) (Normal Dungeon)'),
(10407, 0, 2, 0, 1, 0, 100, 0, 3000, 5000, 8000, 12000, 0, 5, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleshflayer Ghoul - Out of Combat - Play Emote 1'),
(10411, 0, 0, 0, 1, 0, 100, 5, 0, 0, 100, 100, 0, 11, 16380, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Eye of Naxxramas - Out of Combat - Cast ''Greater Invisibility'' (No Repeat) (Normal Dungeon)'),
(10411, 0, 1, 2, 4, 0, 100, 4, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Eye of Naxxramas - On Aggro - Say Line 0 (No Repeat) (Normal Dungeon)'),
(10411, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 16381, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Eye of Naxxramas - On Aggro - Cast ''Summon Rockwing Gargoyles'' (No Repeat) (Normal Dungeon)'),
(10422, 0, 0, 0, 1, 0, 100, 4, 1000, 1000, 1800000, 1800000, 0, 11, 12544, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crimson Sorcerer - Out of Combat - Cast ''Frost Armor'' (Normal Dungeon)'),
(10422, 0, 1, 0, 1, 0, 100, 4, 3000, 3000, 1800000, 1800000, 0, 11, 17150, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crimson Sorcerer - Out of Combat - Cast ''Arcane Might'' (Normal Dungeon)'),
(10422, 0, 2, 0, 0, 0, 100, 4, 0, 0, 2400, 3800, 0, 11, 15230, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crimson Sorcerer - In Combat CMC - Cast ''Arcane Bolt'' (Normal Dungeon)'),
(10422, 0, 3, 0, 0, 0, 100, 4, 7000, 14000, 21000, 26000, 0, 11, 13323, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Crimson Sorcerer - In Combat - Cast ''Polymorph'' (Normal Dungeon)'),
(10422, 0, 4, 0, 0, 0, 100, 4, 5000, 6000, 8500, 10000, 0, 11, 14145, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crimson Sorcerer - In Combat - Cast ''Fire Blast'' (Normal Dungeon)'),
(10424, 0, 0, 0, 0, 0, 100, 4, 5000, 5000, 8000, 8000, 0, 11, 17143, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crimson Gallant - In Combat - Cast ''Holy Strike'' (Phase 1) (No Repeat)'),
(10424, 0, 1, 0, 0, 0, 100, 4, 5000, 5000, 10000, 10000, 0, 11, 14518, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crimson Gallant - In Combat - Cast ''Crusader Strike'' (Phase 1) (No Repeat)'),
(10424, 0, 2, 0, 4, 0, 100, 4, 0, 0, 0, 0, 0, 11, 8990, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crimson Gallant - On Aggro - Cast ''Retribution Aura'' (Phase 1) (No Repeat)'),
(10424, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Crimson Gallant - Between 0-15% Health - Flee For Assist (No Repeat)'),
(10425, 0, 0, 0, 0, 0, 80, 4, 2000, 2000, 20000, 20000, 0, 11, 15732, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crimson Battle Mage - In Combat - Cast ''Immolate'' (Phase 1) (No Repeat)'),
(10425, 0, 1, 0, 0, 0, 100, 4, 12000, 12000, 9000, 9000, 0, 11, 15253, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crimson Battle Mage - In Combat - Cast ''Arcane Explosion'' (Phase 1) (No Repeat)'),
(10425, 0, 2, 0, 0, 0, 100, 4, 8000, 8000, 13000, 13000, 0, 11, 17145, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crimson Battle Mage - In Combat - Cast ''Blast Wave'' (Phase 1) (No Repeat)'),
(10558, 0, 0, 0, 0, 0, 100, 4, 5, 30, 2300, 3900, 0, 11, 16100, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Hearthsinger Forresten - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(10558, 0, 1, 0, 0, 0, 100, 4, 3000, 5000, 12000, 19000, 0, 11, 16244, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hearthsinger Forresten - In Combat - Cast ''Demoralizing Shout'' (Normal Dungeon)'),
(10558, 0, 2, 0, 0, 0, 100, 4, 7000, 9000, 20000, 25000, 0, 11, 16798, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Hearthsinger Forresten - In Combat - Cast ''Enchanting Lullaby'' (Normal Dungeon)'),
(10558, 0, 3, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Hearthsinger Forresten - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(16046, 0, 0, 1, 38, 0, 100, 1, 1, 1, 0, 0, 0, 12, 16101, 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jarien and Sothos Trigger - On Data Set - Spawn Jarien'),
(16046, 0, 1, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 12, 16102, 8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Jarien and Sothos Trigger - Linked with Previous Event - Spawn Sothos'),
(11032, 0, 0, 0, 0, 0, 100, 4, 4000, 4000, 10000, 10000, 0, 11, 12734, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Malor the Zealous - In Combat - Cast ''Ground Smash'''),
(11032, 0, 1, 0, 74, 0, 100, 4, 0, 40, 100, 3600000, 3600000, 11, 6788, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 'Malor the Zealous - On Friendly Between 0-40% Health - Cast ''Lay on Hands'''),
(11032, 0, 2, 0, 0, 0, 100, 4, 2000, 2000, 30000, 35000, 0, 11, 16172, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Malor the Zealous - In Combat - Cast ''Head Crack'''),
(11032, 0, 3, 0, 2, 0, 100, 5, 21, 40, 0, 0, 0, 11, 15493, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Malor the Zealous - Between 21-40% Health - Cast ''Holy Light'''),
(11032, 0, 4, 0, 2, 0, 100, 5, 0, 20, 0, 0, 0, 11, 15493, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Malor the Zealous - Between 0-20% Health - Cast ''Holy Light'''),
(11032, 0, 5, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 104, 0, 0, 0, 0, 0, 0, 14, 47278, 176112, 0, 0, 0, 0, 0, 'Malor the Zealous - On Death - Set Malors Strongbox Selectable');
