-- Pit of Saron: SmartAI for the Ymirjar, Wrathbone, Plagueborn Horror, Stonespine Gargoyle and Deathwhisper trash
-- Ported from TrinityCore 3.3.5. Heroic casts use the normal spell id, whose heroic numbers are in the 5.4.8 client data. The rope and Scourge Package rows of the Plagueborn Horror are left out: the package has no AI here.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36840,36842,36879,36892,36896,37712,37728) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36840,36842,36879,36892,36896,37712,37728);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36840, 0, 0, 0, 0, 0, 100, 6, 7000, 7000, 8000, 8000, 0, 11, 69603, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ymirjar Wrathbringer - In Combat - Cast ''Blight'''),
(36842, 0, 0, 0, 0, 0, 100, 6, 100, 100, 3000, 3000, 0, 11, 69573, 64, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wrathbone Coldwraith - In Combat - Cast ''Frostbolt'''),
(36842, 0, 2, 0, 0, 0, 100, 6, 9000, 9000, 15000, 15000, 0, 11, 69574, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wrathbone Coldwraith - In Combat - Cast ''Freezing Circle'''),
(36879, 0, 0, 0, 0, 0, 100, 6, 5000, 5000, 10000, 10000, 0, 11, 69581, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Plagueborn Horror - In Combat - Cast ''Pustulant Flesh'' (Normal Dungeon)'),
(36879, 0, 2, 0, 0, 0, 100, 6, 8000, 8000, 8000, 8000, 0, 11, 70274, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Plagueborn Horror - In Combat - Cast ''Toxic Waste'' (Dungeon)'),
(36879, 0, 3, 0, 2, 0, 100, 7, 15, 15, 0, 0, 0, 11, 69582, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Plagueborn Horror - Between 15-15% Health - Cast ''Blight Bomb'' (Dungeon)'),
(36892, 0, 0, 0, 0, 0, 100, 6, 4000, 4000, 8000, 8000, 0, 11, 69528, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ymirjar Deathbringer - In Combat - Cast ''Empowered Shadow Bolt'''),
(36896, 0, 0, 0, 0, 0, 100, 6, 1000, 1000, 5000, 5000, 0, 11, 69520, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Stonespine Gargoyle - In Combat - Cast ''Gargoyle Strike'''),
(36896, 0, 2, 0, 2, 0, 100, 7, 10, 10, 0, 0, 0, 11, 69575, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonespine Gargoyle - Between 10-10% Health - Cast ''Stoneform'''),
(37712, 0, 0, 0, 0, 0, 100, 6, 100, 100, 3000, 3000, 0, 11, 70386, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Deathwhisper Shadowcaster - In Combat - Cast ''Shadow Bolt'''),
(37728, 0, 0, 0, 0, 0, 100, 6, 3000, 3000, 8000, 8000, 0, 11, 75330, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wrathbone Sorcerer - In Combat - Cast ''Shadow Bolt''');
