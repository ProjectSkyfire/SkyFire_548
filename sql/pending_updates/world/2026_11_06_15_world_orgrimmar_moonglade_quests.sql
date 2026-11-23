-- Orgrimmar and Moonglade quest fixes.
-- Sources: 5.4.8 data for the quest starters, the spawns of Hallegosa, Duarn, Celestine and the Elemental Gate bunnies,
-- the SmartAI rows and texts of the Hellscream Demolisher and Chief Engineer, the spell target condition and the quest
-- item drops of the Blackrock Depths and Lord Godfrey rows; 4.3.4 and 5.4.8 for the Windswept Balloon spawns, the
-- Ziradormi, Jarod Shadowsong and Ysera spawns and the Baron Silverlaine and Commander Springvale quest item drops
-- (4.3.4 stores them as quest-only rows of a loot group, written here with the quest-only sign of this table).
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- "A Personal Summons" (auto accepted) has no starter in home or 4.3.4 and its follow-up "The Eye of the Storm" needs it
-- rewarded, so the Warchief's Command quests behind it (Vashj'ir) never open. 5.4.8 lets Farseer Krogar offer it.
-- "The Lord of Blackrock" (after the Head of Nefarian) is started by an NPC that is not spawned; 5.4.8 adds Overlord Runthak,
-- who already ends it.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((45244, 28790), (14392, 7784));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(45244, 28790),
(14392, 7784);

-- Quest NPCs that were never spawned: Ziradormi (starts the first quest of the Dragonwrath line) and Hallegosa (starts
-- "The Stuff of Legends") in Grommash Hold, Commander Jarod Shadowsong (starts "The Battle Is Won, The War Goes On",
-- Mount Hyjal), Earthmender Duarn (starts "The War Has Many Fronts"), Ysera (ends "As Hyjal Burns", the quest Emissary
-- Windsong gives in Moonglade), Celestine of the Harvest (Stormwind druid trainer who starts "Moonglade Calls"), and the
-- three Elemental Gate bunnies of Blackrock Depths ("The Breath of Cenarius" credit).
DELETE FROM `creature` WHERE `guid` BETWEEN 8332200 AND 8332208;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8332200, 52382, 1, 1, 0, 0, 0, 0, 1671.54, -4354.48, 26.7564, 2.67309, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Ziradormi
(8332201, 53371, 1, 1, 0, 0, 0, 0, 1671.38, -4356.71, 26.77, 2.95, 1800, 0, 0, 0, 0, 0, 0, 0, 0), -- Hallegosa
(8332202, 40772, 1, 1, 0, 0, 0, 0, 4100.15, -3989.43, 970.67, 1.0472, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Commander Jarod Shadowsong
(8332203, 45460, 0, 1, 0, 0, 0, 0, -6903.06, 4280.84, 5.15876, 5.34071, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Earthmender Duarn
(8332204, 40289, 1, 1, 0, 0, 0, 0, 5490.67, -3563.56, 1569.33, 2.11185, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Ysera
(8332205, 44395, 0, 1, 0, 0, 0, 0, -8284.26, 724.349, 75.3668, 2.09439, 300, 0, 0, 0, 0, 0, 0, 0, 0), -- Celestine of the Harvest
(8332206, 48362, 230, 2, 0, 0, 0, 0, 593.4, -291.203, -42.3761, 1.64061, 7200, 0, 0, 0, 0, 0, 0, 0, 0), -- Elemental Gate Bunny
(8332207, 48362, 230, 2, 0, 0, 0, 0, 534.093, -256.161, -42.8589, 0.767945, 7200, 0, 0, 0, 0, 0, 0, 0, 0), -- Elemental Gate Bunny
(8332208, 48362, 230, 2, 0, 0, 0, 0, 564.302, -265.72, -42.5068, 3.9619, 7200, 0, 0, 0, 0, 0, 0, 0, 0); -- Elemental Gate Bunny

-- "The Breath of Cenarius" (druid, level 50): the Breath of Cenarius (item spell 89914) must hit a bunny to give the
-- credit; the spell needs its target restricted to the bunny, and the bunny a script. The item drops from Pyromancer
-- Loregrain (see the quest item drops below).
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 48362 AND (`AIName` = '' OR `AIName` IS NULL);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 48362;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48362, 0, 0, 1, 8, 0, 100, 0, 89914, 0, 0, 0, 0, 33, 48362, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Elemental Gate Bunny - On Spellhit - Kill Credit'),
(48362, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Elemental Gate Bunny - On Link - Despawn');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 89914;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 89914, 0, 0, 31, 0, 3, 48362, 0, 0, 0, 0, '', 'Require target 48362 for spell 89914');

-- "Weapons of Mass Dysfunction": the Hellscream Demolishers and Chief Engineers are clicked for the two credits, but the
-- click only casts a spell and nothing gives the credit. The demolisher gives its credit and dies, which readies the
-- nearest Chief Engineer; clicking him gives the second credit and removes him.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (42671, 42673) AND (`AIName` = '' OR `AIName` IS NULL);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (42671, 42673);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42671, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Respawn - Remove Spellclick'),
(42671, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Link - Say Line 0'),
(42671, 0, 2, 3, 38, 0, 100, 0, 1, 0, 0, 0, 0, 82, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Data Set 1 - Add Spellclick'),
(42671, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Link - Say Line 1'),
(42671, 0, 4, 0, 52, 0, 100, 0, 1, 42671, 0, 0, 0, 1, 2, 2000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - Text Over - Say Line 2'),
(42671, 0, 5, 0, 52, 0, 100, 0, 2, 42671, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - Text Over - Say Line 3'),
(42671, 0, 6, 7, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42675, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Spellclick - Killcredit'),
(42671, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Chief Engineer - On Link - Despawn'),
(42673, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 42674, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hellscream Demolisher - On Spellclick - Killcredit'),
(42673, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 19, 42671, 20, 0, 0, 0, 0, 0, 'Hellscream Demolisher - On Link - Set Data 1'),
(42673, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hellscream Demolisher - On Link - Die');
DELETE FROM `creature_text` WHERE `entry` = 42671;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42671, 0, 0, 'This baby is all mine! I call her, "The Throttler."', 12, 0, 100, 66, 0, 0, ''),
(42671, 0, 1, 'This baby is all mine! I call her, "The Throttler."', 12, 0, 100, 0, 0, 0, ''),
(42671, 0, 2, 'Ready for inspection! ...Be gentle.', 12, 0, 100, 0, 0, 0, ''),
(42671, 1, 0, 'Ready for inspection! ...Be gentle.', 12, 0, 100, 66, 0, 0, ''),
(42671, 2, 0, 'Uh oh. Um. Oops.', 12, 0, 100, 0, 0, 0, ''),
(42671, 3, 0, 'Look, look, it\'s okay! It\'s just a tiny little glitch. See?', 12, 0, 100, 0, 0, 0, ''),
(42671, 4, 0, 'A little paint and it\'ll be fine!', 12, 0, 100, 0, 0, 0, '');

-- Class quest items without a source. Level 20 Shadowfang Keep quests (all Horde classes): Baron Silverlaine drops the
-- Silverlaine Family Sword (warrior) and the Enchanted Crystal (mage), Commander Springvale the Holy Symbol (priest and
-- paladin quests) and the Sharpening Stone (rogue), Lord Godfrey the Crystal Scope (hunter) and the Tenebrous Orb (warlock). Level 50 Blackrock Depths quests: the General's Attack Plans
-- (warrior), Elemental Golem Blueprints and Elemental Module (hunter), Loregrain's Grimoire (mage), the Breath of Cenarius
-- (druid) and the Elemental-Imbued Weapon (paladin). The 4.3.4 table has the Silverlaine and Springvale rows; the others
-- come from the 5.4.8 table (chances of the BRD rows are 5.4.8 sniff values). Negative chance is the quest-only sign.
DELETE FROM `creature_loot_template` WHERE (`entry`, `item`) IN ((3887, 60878), (3887, 60885), (4278, 60879), (4278, 60880), (46964, 60877), (46964, 60882), (9033, 64302), (8983, 64303), (8907, 64304), (8906, 64304), (9024, 64305), (9024, 63469), (8889, 64313), (8890, 64313), (8891, 64313), (8892, 64313), (8893, 64313), (8894, 64313), (8895, 64313), (8898, 64313), (8903, 64313), (24818, 64313), (24819, 64313));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(3887, 60878, -100, 1, 0, 1, 1),
(3887, 60885, -100, 1, 0, 1, 1),
(4278, 60879, -100, 1, 0, 1, 1),
(4278, 60880, -100, 1, 0, 1, 1),
(46964, 60877, -100, 1, 0, 1, 1),
(46964, 60882, -100, 1, 0, 1, 1),
(9033, 64302, -5.9633, 1, 0, 1, 1),
(8983, 64303, -5.3786, 1, 0, 1, 1),
(8907, 64304, -9.6767, 1, 0, 1, 1),
(8906, 64304, -10.095, 1, 0, 1, 1),
(9024, 64305, -8.5427, 1, 0, 1, 1),
(9024, 63469, -100, 1, 0, 1, 1),
(8889, 64313, -4.939, 1, 0, 1, 1),
(8890, 64313, -1.6378, 1, 0, 1, 1),
(8891, 64313, -0.3882, 1, 0, 1, 1),
(8892, 64313, -0.4312, 1, 0, 1, 1),
(8893, 64313, -0.6016, 1, 0, 1, 1),
(8894, 64313, -0.6017, 1, 0, 1, 1),
(8895, 64313, -0.7998, 1, 0, 1, 1),
(8898, 64313, -1.1413, 1, 0, 1, 1),
(8903, 64313, -1.1354, 1, 0, 1, 1),
(24818, 64313, -9.8015, 1, 0, 1, 1),
(24819, 64313, -9.8751, 1, 0, 1, 1);

-- "Blown Away" (level 10 and up, Jaga in Orgrimmar) asks for 5 Windswept Balloons (item 71034, quest-only loot of the
-- balloon objects) and no balloon was ever spawned. 10 of the 4.3.4 spawns (26 there, 28 in 5.4.8), spread over the city.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930120 AND 930129;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930120, 209058, 1, 1, 0, 0, 1503.99, -4375.2, 23.263, 0, 0, 0, 0, 1, 300, 255, 1),
(930121, 209058, 1, 1, 0, 0, 1530.46, -4374.63, 18.8381, 0, 0, 0, 0, 1, 300, 255, 1),
(930122, 209058, 1, 1, 0, 0, 1590.66, -4366.84, 20.9308, 0, 0, 0, 0, 1, 300, 255, 1),
(930123, 209058, 1, 1, 0, 0, 1629.44, -4417.93, 15.5577, 0, 0, 0, 0, 1, 300, 255, 1),
(930124, 209058, 1, 1, 0, 0, 1726.03, -4441.31, 40.6545, 0, 0, 0, 0, 1, 300, 255, 1),
(930125, 209058, 1, 1, 0, 0, 1816.24, -4496.58, 46.4701, 0, 0, 0, 0, 1, 300, 255, 1),
(930126, 209058, 1, 1, 0, 0, 1904.17, -4694.14, 36.4463, 0, 0, 0, 0, 1, 300, 255, 1),
(930127, 209058, 1, 1, 0, 0, 1916.7, -4418.78, 46.3622, 0, 0, 0, 0, 1, 300, 255, 1),
(930128, 209058, 1, 1, 0, 0, 1880.02, -4237.17, 35.9671, 0, 0, 0, 0, 1, 300, 255, 1),
(930129, 209058, 1, 1, 0, 0, 1552.37, -4206.98, 53.9271, 0, 0, 0, 0, 1, 300, 255, 1);

-- Level 50 Blackrock Depths class quests whose objects were never spawned: "Twilight Scheming" (priest, five race
-- variants, 8 clicks on The Twilight Creed), "The Dark Iron Army" (rogue, 6 Grain Rations) and "Stones of Binding" (warlock,
-- 6 Elemental Binding Stones, quest-only loot). All 17 Twilight Creed spawns of 5.4.8, 16 of 26 Grain Rations and 17 of 31
-- Elemental Binding Stones (every second or third spawn), respawn 7200 seconds as in 5.4.8. No object of these entries exists
-- in home, so none is duplicated.
DELETE FROM `gameobject` WHERE `guid` BETWEEN 930150 AND 930199;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(930150, 207088, 230, 2, 0, 0, 771.88, -244.403, -43.306, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930151, 207088, 230, 2, 0, 0, 837.916, -256.237, -49.3646, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930152, 207088, 230, 2, 0, 0, 757.152, -265.482, -42, -0.506145, 0, 0, -0.25038, 0.968148, 7200, 255, 1),
(930153, 207088, 230, 2, 0, 0, 794.651, -310.043, -43.2936, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930154, 207088, 230, 2, 0, 0, 812.957, -283.725, -41.9889, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930155, 207088, 230, 2, 0, 0, 768.97, -314.267, -42.3275, 0.436331, 0, 0, 0.216439, 0.976296, 7200, 255, 1),
(930156, 207088, 230, 2, 0, 0, 815.472, -310.327, -42.3168, 2.3911, 0, 0, 0.930417, 0.366502, 7200, 255, 1),
(930157, 207088, 230, 2, 0, 0, 811.421, -261.39, -47.282, 0.314158, 0, 0, 0.156434, 0.987688, 7200, 255, 1),
(930158, 207088, 230, 2, 0, 0, 804.661, -266.478, -42.5943, -2.89724, 0, 0, -0.992546, 0.12187, 7200, 255, 1),
(930159, 207088, 230, 2, 0, 0, 822.987, -243.906, -46.4998, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930160, 207088, 230, 2, 0, 0, 814.571, -290.875, -43.2934, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930161, 207088, 230, 2, 0, 0, 776.821, -238.439, -43.2882, -0.558505, 0, 0, -0.275637, 0.961262, 7200, 255, 1),
(930162, 207088, 230, 2, 0, 0, 783.49, -316.778, -42.3252, 2.09439, 0, 0, 0.866025, 0.500001, 7200, 255, 1),
(930163, 207088, 230, 2, 0, 0, 795.259, -212.845, -42.3897, -1.67551, 0, 0, -0.743144, 0.669132, 7200, 255, 1),
(930164, 207088, 230, 2, 0, 0, 800.504, -232.943, -42.3097, 2.51327, 0, 0, 0.951056, 0.309017, 7200, 255, 1),
(930165, 207088, 230, 2, 0, 0, 790.317, -264.107, -42.4088, 3.00194, 0, 0, 0.997563, 0.0697663, 7200, 255, 1),
(930166, 207088, 230, 2, 0, 0, 754.714, -283.621, -42.8701, 1.16937, 0, 0, 0.551936, 0.833886, 7200, 255, 1),
(930167, 207165, 230, 2, 0, 0, 762.093, -47.0011, -46.2338, -2.61799, 0, 0, -0.965925, 0.258821, 7200, 255, 1),
(930168, 207165, 230, 2, 0, 0, 615.738, -36.7466, -60.0593, -2.1293, 0, 0, -0.874619, 0.48481, 7200, 255, 1),
(930169, 207165, 230, 2, 0, 0, 603.729, -72.902, -45.5333, -0.680679, 0, 0, -0.333807, 0.942641, 7200, 255, 1),
(930170, 207165, 230, 2, 0, 0, 690.752, 58.9632, -45.4674, -1.97222, 0, 0, -0.833885, 0.551938, 7200, 255, 1),
(930171, 207165, 230, 2, 0, 0, 569.685, -103.555, -44.1558, 1.78023, 0, 0, 0.777145, 0.629321, 7200, 255, 1),
(930172, 207165, 230, 2, 0, 0, 557.505, -85.7243, -45.021, 0.523599, 0, 0, 0.258819, 0.965926, 7200, 255, 1),
(930173, 207165, 230, 2, 0, 0, 689.807, -17.9762, -45.6713, 1.06465, 0, 0, 0.507538, 0.861629, 7200, 255, 1),
(930174, 207165, 230, 2, 0, 0, 752.01, -51.6703, -45.295, 0.436331, 0, 0, 0.216439, 0.976296, 7200, 255, 1),
(930175, 207165, 230, 2, 0, 0, 704.085, -134.135, -46.2018, 3.12415, 0, 0, 0.999962, 0.00873456, 7200, 255, 1),
(930176, 207165, 230, 2, 0, 0, 663.948, -141.538, -45.0388, -0.157079, 0, 0, -0.0784588, 0.996917, 7200, 255, 1),
(930177, 207165, 230, 2, 0, 0, 561.49, -67.0296, -60.0453, 1.15192, 0, 0, 0.544639, 0.838671, 7200, 255, 1),
(930178, 207165, 230, 2, 0, 0, 716.963, 62.0942, -45.021, -1.53589, 0, 0, -0.694658, 0.71934, 7200, 255, 1),
(930179, 207165, 230, 2, 0, 0, 610.758, -100.858, -45.8768, 1.53589, 0, 0, 0.694658, 0.71934, 7200, 255, 1),
(930180, 207165, 230, 2, 0, 0, 680.79, 58.6099, -60.0442, -1.44862, 0, 0, -0.66262, 0.748956, 7200, 255, 1),
(930181, 207165, 230, 2, 0, 0, 635.084, -79.6262, -45.7336, 2.79252, 0, 0, 0.984807, 0.173652, 7200, 255, 1),
(930182, 207165, 230, 2, 0, 0, 734.273, -17.6378, -46.2158, 1.0821, 0, 0, 0.515037, 0.857168, 7200, 255, 1),
(930183, 207145, 230, 2, 0, 0, 805.886, -308.349, -43.2934, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930184, 207145, 230, 2, 0, 0, 556.055, -81.9377, -45.021, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930185, 207145, 230, 2, 0, 0, 772.437, -316.979, -43.2998, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930186, 207145, 230, 2, 0, 0, 543.635, -246.288, -43.1014, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930187, 207145, 230, 2, 0, 0, 903.837, -224.706, -50.3288, -1.88495, 0, 0, -0.809016, 0.587786, 7200, 100, 1),
(930188, 207145, 230, 2, 0, 0, 674.332, -148.147, -46.2338, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930189, 207145, 230, 2, 0, 0, 665.414, 48.6198, -60.025, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930190, 207145, 230, 2, 0, 0, 873.59, -298.673, -49.936, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930191, 207145, 230, 2, 0, 0, 719.956, 32.5136, -45.439, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930192, 207145, 230, 2, 0, 0, 838.927, -343.508, -50.2733, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930193, 207145, 230, 2, 0, 0, 876.976, -240.724, -49.911, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930194, 207145, 230, 2, 0, 0, 842.188, -265.908, -49.9779, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930195, 207145, 230, 2, 0, 0, 917.68, -314.856, -49.9573, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930196, 207145, 230, 2, 0, 0, 740.511, -4.65292, -46.2338, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930197, 207145, 230, 2, 0, 0, 792.484, -305.44, -43.2949, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930198, 207145, 230, 2, 0, 0, 611.385, -274.681, -43.1078, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1),
(930199, 207145, 230, 2, 0, 0, 620.379, -119.139, -46.2061, -1.62316, 0, 0, -0.725374, 0.688355, 7200, 100, 1);
