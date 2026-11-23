-- Blackfathom Deeps: the trash and three bosses (Ghamoo-Ra, Lady Sarevess, Lorgus Jett) had no SmartAI, so they
-- only meleed. Rows from the 4.3.4 world data; spells checked against 5.4.8.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (4815, 4821, 4818, 4819, 4820, 4805, 4822, 4807, 4887, 4831, 4798, 4799, 12902, 4809, 4811, 4812, 4827, 4810, 4813, 4814) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (4815, 4821, 4818, 4819, 4820, 4805, 4822, 4807, 4887, 4831, 4798, 4799, 12902, 4809, 4811, 4812, 4827, 4810, 4813, 4814);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4798, 0, 0, 0, 0, 0, 100, 0, 5000, 7000, 20000, 35000, 0, 11, 11980, 32, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fallenroot Shadowstalker - In Combat - Cast ''Curse of Weakness'' (Phase 1) (No Repeat)'),
(4799, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2400, 3800, 0, 11, 9613, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Fallenroot Hellcaller - In Combat CMC - Cast ''Shadow Bolt'' (Normal Dungeon)'),
(4799, 0, 1, 0, 0, 0, 100, 0, 12000, 15000, 30000, 45000, 0, 11, 8129, 1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Fallenroot Hellcaller - In Combat - Cast ''Mana Burn'' (Normal Dungeon)'),
(4805, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 1800000, 1800000, 0, 11, 12544, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackfathom Sea Witch - Out of Combat - Cast ''Frost Armor'' (No Repeat)'),
(4805, 0, 1, 0, 0, 0, 100, 0, 7000, 10000, 16000, 20000, 0, 11, 122, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackfathom Sea Witch - In Combat - Cast ''Frost Nova'' (No Repeat)'),
(4805, 0, 2, 0, 0, 0, 100, 1, 3000, 6000, 0, 0, 0, 11, 15044, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackfathom Sea Witch - In Combat - Cast ''Frost Ward'' (No Repeat)'),
(4805, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Blackfathom Sea Witch - Between 0-15% Health - Flee For Assist (No Repeat)'),
(4807, 0, 0, 0, 0, 0, 100, 0, 5000, 9000, 16000, 20000, 0, 11, 8379, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blackfathom Myrmidon - In Combat - Cast ''Disarm'' (No Repeat)'),
(4809, 0, 1, 0, 2, 0, 100, 0, 0, 50, 22000, 25000, 0, 11, 8362, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Acolyte - Between 0-50% Health - Cast ''Renew'' (No Repeat)'),
(4810, 0, 0, 0, 0, 0, 100, 0, 5000, 9000, 8000, 12000, 0, 11, 8374, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Reaver - In Combat - Cast ''Arcing Smash'' (No Repeat)'),
(4810, 0, 1, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Twilight Reaver - Between 0-15% Health - Flee For Assist (No Repeat)'),
(4811, 0, 0, 0, 1, 0, 100, 1, 1000, 1000, 0, 0, 0, 11, 8372, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Aquamancer - Out of Combat - Cast ''Summon Aqua Guardian'' (No Repeat)'),
(4811, 0, 1, 0, 0, 0, 100, 0, 0, 40, 2400, 3800, 0, 11, 9672, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Aquamancer - In Combat CMC - Cast ''Frostbolt'' (Normal Dungeon)'),
(4811, 0, 2, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Twilight Aquamancer - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4812, 0, 0, 0, 0, 0, 100, 0, 5000, 9000, 12000, 20000, 0, 11, 18972, 32, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Twilight Loreseeker - In Combat - Cast ''Slow'' (No Repeat) (Normal Dungeon)'),
(4812, 0, 1, 0, 2, 0, 100, 0, 0, 30, 10000, 10000, 0, 11, 8365, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Loreseeker - Between 0-30% Health - Cast ''Enlarge'' (No Repeat) (Normal Dungeon)'),
(4813, 0, 0, 0, 1, 0, 100, 1, 1000, 1000, 0, 0, 0, 11, 12746, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Twilight Shadowmage - Out of Combat - Cast ''Summon Voidwalker'' (No Repeat) (Normal Dungeon)'),
(4813, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2400, 3800, 0, 11, 9613, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Shadowmage - In Combat CMC - Cast ''Shadow Bolt'' (Normal Dungeon)'),
(4813, 0, 2, 0, 0, 0, 100, 0, 18000, 26000, 24000, 30000, 0, 11, 7645, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Twilight Shadowmage - In Combat - Cast ''Dominate Mind'' (Normal Dungeon)'),
(4813, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Twilight Shadowmage - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4814, 0, 0, 0, 0, 0, 100, 0, 4000, 7000, 9000, 12000, 0, 11, 13728, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Twilight Elementalist - In Combat - Cast ''Earth Shock'' (No Repeat) (Normal Dungeon)'),
(4814, 0, 1, 0, 0, 0, 100, 0, 5000, 9000, 13000, 17000, 0, 11, 15039, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Twilight Elementalist - In Combat - Cast ''Flame Shock'' (No Repeat) (Normal Dungeon)'),
(4814, 0, 2, 0, 0, 0, 100, 0, 1000, 2500, 11000, 15000, 0, 11, 12548, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Elementalist - In Combat - Cast ''Frost Shock'' (No Repeat) (Normal Dungeon)'),
(4814, 0, 3, 0, 0, 0, 100, 0, 3000, 6000, 7000, 12000, 0, 11, 11824, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Twilight Elementalist - In Combat - Cast ''Shock'' (No Repeat) (Normal Dungeon)'),
(4815, 0, 0, 0, 0, 0, 100, 0, 5000, 9000, 16000, 20000, 0, 11, 8379, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Murkshallow Snapclaw - In Combat - Cast ''Disarm'' (No Repeat) (Normal Dungeon)'),
(4818, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 7164, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Murloc - On Aggro - Cast ''Defensive Stance'' (No Repeat) (Normal Dungeon)'),
(4818, 0, 1, 0, 0, 0, 75, 0, 3000, 5000, 8000, 11000, 0, 11, 22691, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Murloc - In Combat - Cast ''Sunder Armor'' (No Repeat) (Normal Dungeon)'),
(4819, 0, 0, 0, 2, 0, 100, 0, 0, 50, 50000, 60000, 0, 11, 8382, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Muckdweller - Between 0-50% Health - Cast ''Leech Poison'' (No Repeat) (Normal Dungeon)'),
(4820, 0, 0, 0, 0, 0, 100, 0, 0, 0, 2400, 3800, 0, 11, 9532, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Oracle - In Combat CMC - Cast ''Lightning Bolt'' (Normal Dungeon)'),
(4820, 0, 2, 0, 2, 0, 100, 0, 0, 49, 22000, 22000, 0, 11, 8362, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Oracle - Between 0-50% Health - Cast ''Renew'' (Normal Dungeon)'),
(4820, 0, 3, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Blindlight Oracle - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4821, 0, 0, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Skittering Crustacean - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4822, 0, 0, 0, 2, 0, 100, 1, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Snapping Crustacean - Between 0-15% Health - Flee For Assist (No Repeat) (Normal Dungeon)'),
(4827, 0, 0, 0, 0, 0, 100, 0, 3000, 6000, 20000, 25000, 0, 11, 3604, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Deep Pool Threshfin - In Combat - Cast ''Tendon Rip'' (No Repeat) (Normal Dungeon)'),
(4831, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - On Aggro - Say Line 0 (No Repeat) (Normal Dungeon)'),
(4831, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2300, 3900, 0, 11, 6660, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - In Combat CMC - Cast ''Shoot'' (Normal Dungeon)'),
(4831, 0, 2, 0, 0, 0, 100, 0, 9000, 12000, 12000, 15000, 0, 11, 8435, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - In Combat - Cast ''Forked Lightning'' (Normal Dungeon)'),
(4831, 0, 3, 0, 0, 0, 100, 0, 6000, 8500, 9000, 13000, 0, 11, 11831, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - In Combat - Cast ''Frost Nova'' (Normal Dungeon)'),
(4831, 0, 4, 0, 0, 0, 100, 0, 7000, 9000, 9000, 13000, 0, 11, 246, 33, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - In Combat - Cast ''Slow'' (Normal Dungeon)'),
(4831, 0, 5, 0, 6, 0, 100, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lady Sarevess - On Just Died - Say Line 1 (Normal Dungeon)'),
(4887, 0, 0, 0, 0, 0, 100, 0, 5000, 8000, 9000, 13000, 0, 11, 5568, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ghamoo-ra - In Combat - Cast ''Trample'''),
(12902, 0, 0, 0, 1, 0, 100, 0, 1000, 1000, 600000, 600000, 0, 11, 12550, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lorgus Jett - Out of Combat - Cast ''Lightning Shield'' (Normal Dungeon)'),
(12902, 0, 1, 0, 0, 0, 100, 0, 0, 0, 2400, 3800, 0, 11, 12167, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Lorgus Jett - Combat CMC - Cast ''Lightning Bolt'' (Normal Dungeon)');

DELETE FROM `creature_text` WHERE (`entry` = 4831 AND `groupid` = 0) OR (`entry` = 4831 AND `groupid` = 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(4831, 0, 0, 'You should not be here! Slay them!', 14, 0, 100, 0, 0, 5799, ''),
(4831, 1, 0, 'Paltry kill.', 14, 0, 100, 0, 0, 5801, '');
