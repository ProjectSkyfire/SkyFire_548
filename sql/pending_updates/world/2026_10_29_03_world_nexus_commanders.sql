-- Bind the commander script to Commander Stoutbeard and Commander Kolurg
UPDATE `creature_template` SET `ScriptName` = 'boss_nexus_commanders' WHERE `entry` IN (26796, 26798) AND `ScriptName` = '' AND `AIName` = '';

-- Commander Stoutbeard is the heroic commander, Alliance Commander is the normal one
UPDATE `creature` SET `spawnMask` = 4 WHERE `guid` = 4764 AND `id` = 26796;

-- Retail pack layout (guid 8305000 and up are the reserved Nexus guids)
UPDATE `creature` SET `id` = 26800, `modelid` = 0 WHERE `guid` = 126666 AND `map` = 576;
UPDATE `creature` SET `id` = 26800, `modelid` = 0 WHERE `guid` = 126668 AND `map` = 576;
UPDATE `creature` SET `id` = 26805, `modelid` = 0 WHERE `guid` = 126664 AND `map` = 576;

DELETE FROM `creature` WHERE `guid` BETWEEN 8305000 AND 8305009;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8305000, 26737, 576, 6, 0, 0, 0, 0, 527.551, -9.89882, -14.9402, 0.0349066, 3600, 0, 0, 37164, 0, 0, 0, 33588032, 0),
(8305001, 26800, 576, 6, 0, 0, 0, 1, 408.059, 187.066, -34.9363, 5.0091, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305002, 26800, 576, 6, 0, 0, 0, 1, 443.366, 183.462, -34.9374, 4.31096, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305003, 26800, 576, 6, 0, 0, 0, 1, 450.258, 175.462, -34.9375, 4.11898, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305004, 26802, 576, 6, 0, 0, 0, 1, 442.812, 176.398, -34.9374, 4.27606, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305005, 26802, 576, 6, 0, 0, 0, 1, 460.961, 147.602, -34.9378, 3.45575, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305006, 26802, 576, 6, 0, 0, 0, 1, 460.826, 162.661, -34.9377, 3.76991, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305007, 26805, 576, 6, 0, 0, 0, 1, 428.6, 189.332, -34.9369, 4.60767, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305008, 26805, 576, 6, 0, 0, 0, 1, 420.703, 189.507, -34.9366, 4.7822, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(8305009, 27949, 576, 2, 0, 0, 0, 1, 424.547, 185.962, -34.9367, 4.69494, 7200, 0, 0, 0, 0, 0, 0, 0, 0);

-- SmartAI
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (26722, 26727, 26793, 26801, 26802) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26722, 26727, 26793, 26801, 26802);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(26722, 0, 0, 0, 0, 0, 100, 4, 0, 0, 3400, 4800, 0, 11, 15530, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Azure Magus - In Combat CMC - Cast ''Frostbolt'' (Normal Dungeon)'),
(26722, 0, 1, 0, 0, 0, 100, 8, 0, 0, 3400, 4800, 0, 11, 56775, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Azure Magus - In Combat CMC - Cast ''Frostbolt'' (Heroic Dungeon)'),
(26722, 0, 2, 0, 0, 0, 100, 4, 9000, 11000, 9000, 15000, 0, 11, 37132, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Azure Magus - In Combat - Cast ''Arcane Shock'' (Normal Dungeon)'),
(26722, 0, 3, 0, 0, 0, 100, 8, 9000, 11000, 9000, 15000, 0, 11, 56776, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Azure Magus - In Combat - Cast ''Arcane Shock'' (Heroic Dungeon)'),
(26727, 0, 0, 0, 0, 0, 100, 1, 0, 0, 0, 0, 0, 31, 1, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Set Random Phase (1-3) (No Repeat)'),
(26727, 0, 1, 0, 0, 1, 100, 4, 0, 0, 2400, 3800, 0, 11, 12466, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat CMC - Cast ''Fireball'' (Phase 1) (Normal Dungeon)'),
(26727, 0, 2, 0, 0, 1, 100, 8, 0, 0, 2400, 3800, 0, 11, 17290, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat CMC - Cast ''Fireball'' (Phase 1) (Heroic Dungeon)'),
(26727, 0, 3, 0, 0, 1, 100, 4, 7000, 9000, 15000, 28000, 0, 11, 36808, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Rain of Fire'' (Phase 1) (Normal Dungeon)'),
(26727, 0, 4, 0, 0, 1, 100, 8, 7000, 9000, 15000, 28000, 0, 11, 39376, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Rain of Fire'' (Phase 1) (Heroic Dungeon)'),
(26727, 0, 5, 0, 0, 1, 100, 0, 4000, 6000, 12000, 18000, 0, 11, 47784, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Immolation Trap'' (Phase 1)'),
(26727, 0, 6, 0, 0, 2, 100, 4, 0, 0, 2400, 3800, 0, 11, 12737, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat CMC - Cast ''Frostbolt'' (Phase 2) (Normal Dungeon)'),
(26727, 0, 7, 0, 0, 2, 100, 8, 0, 0, 2400, 3800, 0, 11, 56837, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat CMC - Cast ''Frostbolt'' (Phase 2) (Heroic Dungeon)'),
(26727, 0, 8, 0, 0, 2, 100, 4, 10000, 12000, 11000, 15000, 0, 11, 15244, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Cone of Cold'' (Phase 2) (Normal Dungeon)'),
(26727, 0, 9, 0, 0, 2, 100, 8, 10000, 12000, 11000, 15000, 0, 11, 38384, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Cone of Cold'' (Phase 2) (Heroic Dungeon)'),
(26727, 0, 10, 0, 0, 2, 100, 0, 7000, 9000, 11000, 18000, 0, 11, 55040, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Freezing Trap'' (Phase 2)'),
(26727, 0, 11, 0, 0, 4, 100, 4, 4000, 6000, 8000, 11000, 0, 11, 34933, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Arcane Explosion'' (Phase 4) (Normal Dungeon)'),
(26727, 0, 12, 0, 0, 4, 100, 8, 4000, 6000, 8000, 11000, 0, 11, 56825, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Arcane Explosion'' (Phase 4) (Heroic Dungeon)'),
(26727, 0, 13, 0, 0, 4, 100, 4, 1000, 1200, 60000, 60000, 0, 11, 50182, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Aura of Arcane Haste'' (Phase 4) (Normal Dungeon)'),
(26727, 0, 14, 0, 0, 4, 100, 8, 1000, 1200, 60000, 60000, 0, 11, 56827, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Aura of Arcane Haste'' (Phase 4) (Heroic Dungeon)'),
(26727, 0, 15, 0, 0, 4, 100, 0, 9000, 14000, 19000, 22000, 0, 11, 13323, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Polymorph'' (Phase 4)'),
(26727, 0, 16, 0, 0, 4, 100, 0, 8000, 9000, 9000, 15000, 0, 11, 47789, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mage Hunter Ascendant - In Combat - Cast ''Arcane Trap'' (Phase 4)'),
(26793, 0, 0, 0, 4, 0, 100, 12, 0, 0, 0, 0, 0, 11, 48058, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - On Aggro - Cast ''Crystal Bloom'''),
(26793, 0, 1, 0, 0, 0, 100, 12, 3000, 4000, 9500, 15000, 0, 11, 48053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - In Combat - Cast ''Ensnare'''),
(26793, 0, 2, 3, 2, 0, 100, 4, 0, 100, 3800, 5000, 0, 11, 52067, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 0-100% Health - Cast ''Aura of Regeneration'' (Normal Dungeon)'),
(26793, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 57056, 32, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 0-100% Health - Cast ''Aura of Regeneration'' (Normal Dungeon)'),
(26793, 0, 4, 5, 2, 0, 100, 13, 1, 35, 0, 0, 0, 11, 48082, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 1-35% Health - Cast ''Seed Pod'' (No Repeat) (Dungeon)'),
(26793, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 256, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 1-35% Health - Set ImmunePC (No Repeat) (Dungeon)'),
(26793, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 1-35% Health - Set Uninteractible (No Repeat) (Dungeon)'),
(26793, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 1-35% Health - Set Event Phase 1 (No Repeat) (Dungeon)'),
(26793, 0, 8, 9, 2, 0, 100, 13, 85, 100, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 85-100% Health - Evade (No Repeat) (Dungeon)'),
(26793, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 85-100% Health - Set Event Phase 0 (No Repeat) (Dungeon)'),
(26793, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 256, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 85-100% Health - Remove ImmunePC (No Repeat) (Dungeon)'),
(26793, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crystalline Frayer - Between 85-100% Health - Remove Uninteractible (No Repeat) (Dungeon)'),
(26801, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 28, 47543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - On Aggro - Remove Aura ''Frozen Prison'''),
(26801, 0, 1, 0, 0, 0, 100, 4, 0, 0, 2300, 3800, 0, 11, 15620, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(26801, 0, 2, 0, 0, 0, 100, 8, 0, 0, 2300, 3800, 0, 11, 22907, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - In Combat CMC - Cast ''Shoot'' (Heroic Dungeon)'),
(26801, 0, 3, 0, 9, 0, 100, 4, 5, 45, 9500, 10000, 0, 11, 47777, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - Within 5-45 Range - Cast ''Incendiary Shot'' (Normal Dungeon)'),
(26801, 0, 4, 0, 9, 0, 100, 8, 5, 45, 9500, 10000, 0, 11, 56933, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - Within 5-45 Range - Cast ''Incendiary Shot'' (Heroic Dungeon)'),
(26801, 0, 5, 0, 0, 0, 100, 0, 12000, 15000, 16100, 19000, 0, 11, 48191, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Horde Ranger - In Combat - Cast ''Rapid Shot'''),
(26802, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 28, 47543, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - On Aggro - Remove Aura ''Frozen Prison'''),
(26802, 0, 1, 0, 0, 0, 100, 4, 0, 0, 2300, 3800, 0, 11, 15620, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(26802, 0, 2, 0, 0, 0, 100, 8, 0, 0, 2300, 3800, 0, 11, 22907, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - In Combat CMC - Cast ''Shoot'' (Heroic Dungeon)'),
(26802, 0, 3, 0, 9, 0, 100, 4, 5, 45, 9500, 10000, 0, 11, 47777, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - Within 5-45 Range - Cast ''Incendiary Shot'' (Normal Dungeon)'),
(26802, 0, 4, 0, 9, 0, 100, 8, 5, 45, 9500, 10000, 0, 11, 56933, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - Within 5-45 Range - Cast ''Incendiary Shot'' (Heroic Dungeon)'),
(26802, 0, 5, 0, 0, 0, 100, 0, 12000, 15000, 16100, 19000, 0, 11, 48191, 4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Alliance Ranger - In Combat - Cast ''Rapid Shot''');

-- Creature texts
DELETE FROM `creature_text` WHERE (`entry` = 26723 AND `groupid` = 5 AND `id` = 0) OR (`entry` = 26763 AND `groupid` = 4 AND `id` = 0) OR (`entry` = 26763 AND `groupid` = 5 AND `id` = 0) OR (`entry` = 26794 AND `groupid` = 6 AND `id` = 0) OR (`entry` = 26796 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 26796 AND `groupid` = 1 AND `id` = 0) OR (`entry` = 26796 AND `groupid` = 2 AND `id` = 0) OR (`entry` = 26798 AND `groupid` = 0 AND `id` = 0) OR (`entry` = 26798 AND `groupid` = 1 AND `id` = 0) OR (`entry` = 26798 AND `groupid` = 2 AND `id` = 0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(26723, 5, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Keristrasza'),
(26763, 4, 0, '%s opens a Chaotic Rift!', 41, 0, 100, 0, 0, 0, 'Anomalus'),
(26763, 5, 0, '%s shields himself and diverts his power to the rifts!', 41, 0, 100, 0, 0, 0, 'Anomalus'),
(26794, 6, 0, '%s goes into a frenzy!', 41, 0, 100, 0, 0, 0, 'Ormorok the Tree-Shaper'),
(26796, 0, 0, 'What? Where in the-- don''t just stand around lads; kill somebody!', 14, 0, 100, 0, 0, 13193, 'Commander Stoutbeard'),
(26796, 1, 0, 'Now we''re gettin'' someplace!', 14, 0, 100, 0, 0, 13195, 'Commander Stoutbeard'),
(26796, 2, 0, 'Is that all you''ve...', 14, 0, 100, 0, 0, 13194, 'Commander Stoutbeard'),
(26798, 0, 0, 'What is this? Mok-thorin ka! Kill them!', 14, 0, 100, 0, 0, 13458, 'Commander Kolurg'),
(26798, 1, 0, 'Our task is not yet done!', 14, 0, 100, 0, 0, 13459, 'Commander Kolurg'),
(26798, 2, 0, 'Gaagh...', 14, 0, 100, 0, 0, 13460, 'Commander Kolurg');
