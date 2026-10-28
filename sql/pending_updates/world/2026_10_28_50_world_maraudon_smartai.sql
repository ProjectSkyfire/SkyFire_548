-- Maraudon: SmartAI and creature texts the dungeon was missing. Source: 3.3.5 or 4.3.4, whichever has more rows for the creature
-- (the two sets agree where both exist; the dungeon was re-levelled by Cataclysm). Columns are identical to SkyFire's. event_flags are kept, minus the 4.3.4
-- WHILE_CHARMED bit (0x200), which is the 40-man difficulty bit in SkyFire. Every spell used was checked against the 5.4.8 spell data.
-- Creatures with a C++ ScriptName keep it. Safe to run twice: DELETE before INSERT by key.

-- Enable SmartAI only where no C++ script is bound.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (11783, 11784, 11790, 11791, 11792, 11793, 11794, 12206, 12207, 12216, 12217, 12218, 12219, 12220, 12221, 12222, 12223, 12224, 13141, 13142, 13323, 13533, 13596) AND `ScriptName` = '';

-- Texts missing in SkyFire (existing rows of the same entries are left alone).
DELETE FROM `creature_text` WHERE (`entry` = 11793 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 11793 AND `groupid` = 0 AND `id` = 1) OR (`entry` = 11794 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 11794 AND `groupid` = 0 AND `id` = 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(11793, 0, 0, 'You do not belong in these gardens.  Your body shall nourish our lovely creations!', 12, 0, 100, 0, 0, 0, 'Celebrian Dryad'),
(11793, 0, 1, 'Nothing must befoul the gardens! You must be destroyed!', 12, 0, 100, 0, 0, 0, 'Celebrian Dryad'),
(11794, 0, 0, 'You do not belong in these gardens.  Your body shall nourish our lovely creations!', 12, 0, 100, 0, 0, 0, 'Sister of Celebrian'),
(11794, 0, 1, 'Nothing must befoul the gardens! You must be destroyed!', 12, 0, 100, 0, 0, 0, 'Sister of Celebrian');

-- Creature SmartAI rows.
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (11783, 11784, 11790, 11791, 11792, 11793, 11794, 12206, 12207, 12216, 12217, 12218, 12219, 12220, 12221, 12222, 12223, 12224, 13141, 13142, 13323, 13533, 13596);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(11783, 0, 0, 0, 9, 0, 50, 0, 0, 5, 7000, 11000, 0, 11, 13584, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Theradrim Shardling - Within 0-5 Range - Cast ''Strike'''),
(11783, 0, 1, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 29, 2, 90, 0, 0, 0, 0, 19, 11784, 10, 0, 0, 0, 0, 0, 'Theradrim Shardling - On Just Summoned - Start Follow Closest Creature ''Theradrim Guardian'''),
(11784, 0, 0, 0, 9, 0, 100, 4, 0, 5, 7000, 12000, 0, 11, 16790, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Theradrim Guardian - Within 0-5 Range - Cast ''Knockdown'' (Normal Dungeon)'),
(11784, 0, 1, 0, 1, 0, 100, 5, 1000, 1000, 0, 0, 0, 11, 21057, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Theradrim Guardian - Out of Combat - Cast ''Summon Theradrim Shardling'' (No Repeat) (Normal Dungeon)'),
(11790, 0, 0, 0, 9, 0, 100, 4, 0, 5, 7000, 11000, 0, 11, 12540, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Putridus Satyr - Within 0-5 Range - Cast Gouge'),
(11790, 0, 1, 0, 0, 0, 100, 4, 6000, 8000, 22000, 25000, 0, 11, 21062, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Putridus Satyr - In Combat - Cast Putrid Breath'),
(11790, 0, 2, 0, 0, 0, 100, 4, 8000, 15000, 12000, 15000, 0, 11, 15667, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridus Satyr - In Combat - Cast Sinister Strike'),
(11791, 0, 0, 0, 0, 0, 100, 4, 5000, 8000, 6000, 9000, 0, 11, 15657, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridus Trickster - On Behind Target - Cast Backstab'),
(11791, 0, 1, 0, 0, 0, 100, 4, 5000, 8000, 22000, 25000, 0, 11, 21062, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Putridus Trickster - In Combat - Cast Putrid Breath'),
(11791, 0, 2, 0, 25, 0, 100, 4, 0, 0, 0, 0, 0, 11, 3417, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Putridus Trickster - On Reset - Cast Thrash'),
(11791, 0, 3, 0, 0, 0, 100, 4, 5000, 8000, 17000, 19000, 0, 11, 13298, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridus Trickster - In Combat - Cast Poison'),
(11792, 0, 0, 0, 0, 0, 100, 4, 8000, 16000, 21000, 30000, 0, 11, 15087, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Putridus Shadowstalker - In Combat - Cast Evasion'),
(11792, 0, 1, 0, 9, 0, 100, 4, 0, 5, 12000, 15000, 0, 11, 9080, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Putridus Shadowstalker - Within 0-5 Range - Cast Hamstring'),
(11793, 0, 0, 0, 0, 0, 100, 4, 0, 0, 2300, 3900, 0, 11, 16000, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Celebrian Dryad - In Combat - Cast Throw'),
(11793, 0, 1, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Celebrian Dryad - Between 0-15% Health - Flee For Assist (No Repeat)'),
(11793, 0, 2, 0, 0, 0, 100, 4, 8000, 16000, 12000, 16000, 0, 11, 17201, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Celebrian Dryad - In Combat - Cast Dispel Magic'),
(11793, 0, 3, 0, 0, 0, 100, 4, 5000, 7000, 12000, 13000, 0, 11, 7992, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Celebrian Dryad - In Combat - Slowing Poison'),
(11793, 0, 4, 0, 4, 0, 30, 4, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Celebrian Dryad - On Aggro - Say Line 0'),
(11794, 0, 0, 0, 0, 0, 100, 4, 0, 0, 2300, 3900, 0, 11, 15795, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sister of Celebrian - In Combat - Cast Throw'),
(11794, 0, 1, 0, 9, 0, 100, 4, 0, 5, 8000, 9000, 0, 11, 14516, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Sister of Celebrian - Within 0-5 Range - Cast Strike'),
(11794, 0, 2, 0, 4, 0, 30, 4, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sister of Celebrian - On Aggro - Say Line 0'),
(12206, 0, 0, 0, 0, 0, 100, 4, 0, 0, 2300, 3900, 0, 11, 21071, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Primordial Behemoth - In Combat - Cast Boulder'),
(12206, 0, 1, 0, 9, 0, 100, 4, 0, 8, 8000, 13000, 0, 11, 5568, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Primordial Behemoth - Within 0-8 Range - Cast Trample'),
(12207, 0, 0, 0, 0, 0, 100, 4, 10000, 13000, 12000, 18000, 0, 11, 21790, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thessala Hydra - In Combat - Cast  Aqua Jet'),
(12207, 0, 1, 0, 25, 0, 100, 4, 0, 0, 0, 0, 0, 11, 3417, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thessala Hydra - On Reset - Cast Thrash'),
(12207, 0, 2, 0, 0, 0, 100, 4, 6000, 10000, 17000, 18000, 0, 11, 21787, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Thessala Hydra - In Combat - Cast Deadly Poison'),
(12216, 0, 0, 0, 0, 0, 100, 4, 0, 0, 3000, 4500, 0, 11, 21067, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Poison Sprite - In Combat - Cast Poison Bolt'),
(12217, 0, 0, 0, 9, 0, 100, 4, 0, 5, 30000, 45000, 0, 11, 5413, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Corruptor - Within 0-5 Range - Cast Noxious Catalyst'),
(12217, 0, 1, 0, 0, 0, 100, 4, 2000, 5000, 14000, 21000, 0, 11, 21068, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Corruptor - In Combat - Cast Corruption'),
(12218, 0, 0, 0, 9, 0, 100, 4, 0, 10, 5000, 9000, 0, 11, 21069, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vile Larva - Within 0-10 Range - Cast Larva Goo'),
(12218, 0, 1, 0, 0, 0, 100, 4, 5000, 8000, 17000, 18000, 0, 11, 744, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Vile Larva - In Combat - Cast Poison'),
(12219, 0, 0, 0, 0, 0, 100, 4, 2000, 5000, 6000, 11000, 0, 11, 21749, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Barbed Lasher - In Combat - Cast Thorn Volley'),
(12220, 0, 0, 0, 0, 0, 100, 4, 3000, 5000, 9000, 17000, 0, 11, 11922, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Constrictor Vine - In Combat - Cast Entangling Roots'),
(12221, 0, 0, 0, 6, 0, 100, 4, 0, 0, 0, 0, 0, 11, 21070, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Noxious Slime - On Just Died - Cast Noxious Cloud'),
(12223, 0, 0, 0, 0, 0, 100, 4, 5000, 8000, 6000, 11000, 0, 11, 11428, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cavern Lurker - In Combat - Cast Knockdown'),
(12224, 0, 0, 0, 0, 0, 100, 4, 5000, 8000, 6000, 11000, 0, 11, 16790, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cavern Shambler - In Combat - Cast Knockdown'),
(12224, 0, 1, 0, 2, 0, 100, 4, 0, 75, 21000, 26000, 0, 11, 7948, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cavern Shambler - Between 0-75% Health - Cast Wild Regeneration'),
(13141, 0, 0, 0, 0, 0, 100, 4, 11000, 16000, 15000, 21000, 0, 11, 11876, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deeprot Stomper - In Combat - Cast War Stomp'),
(13141, 0, 1, 0, 0, 0, 100, 4, 5000, 8000, 7000, 10000, 0, 11, 13446, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Deeprot Stomper - In Combat - Cast Strike'),
(13142, 0, 0, 0, 1, 0, 100, 4, 1000, 1000, 600000, 600000, 0, 11, 21337, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deeprot Tangler - Out of Combat - Cast Thorns'),
(13142, 0, 1, 0, 0, 0, 100, 4, 3000, 5000, 17000, 21000, 0, 11, 21331, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Deeprot Tangler - In Combat - Cast Entangling Roots'),
(13323, 0, 0, 0, 0, 0, 100, 4, 6000, 8000, 10000, 14000, 0, 11, 8281, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Subterranean Diemetradon - In Combat - Cast Sonic Burst'),
(13533, 0, 0, 0, 9, 0, 100, 4, 6000, 9000, 30000, 45000, 0, 11, 5413, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spewed Larva - In Combat - Cast Noxious Catalyst'),
(13596, 0, 0, 0, 0, 0, 100, 4, 6000, 6000, 12000, 12000, 0, 11, 16495, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotgrip - In Combat - Cast ''Fatal Bite'' (Phase 1) (No Repeat) (Normal Dungeon)'),
(13596, 0, 1, 0, 0, 0, 100, 4, 6000, 8000, 13000, 18000, 0, 11, 15976, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Rotgrip - In Combat - Cast Puncture'),
(12222, 0, 0, 0, 0, 0, 100, 0, 4000, 6000, 7000, 10000, 0, 11, 26419, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Creeping Sludge - IC - Cast Acid Spray'),
(12222, 0, 1, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 75, 22638, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Creeping Sludge - On reset - Add Poison Shock');
