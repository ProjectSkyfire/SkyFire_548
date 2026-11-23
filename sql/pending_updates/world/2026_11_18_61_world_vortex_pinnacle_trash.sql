-- Vortex Pinnacle trash: the creatures had no AI and so no abilities. They get the Smart AI scripts of the 4.3.4 data (the same spells in the 5.4.8 client
-- data, whose heroic versions are difficulty rows of the base spell). The Cloud Prince keeps Whipping Winds, Starfall and Typhoon; the helper script that
-- tracks the winds it summons is left out (flag 512 is not used on this core), and so is the script of the Young Storm Dragon.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (45477, 45912, 45915, 45917, 45922, 45924, 45926, 45928, 45930, 45935) AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (45477, 45912, 45915, 45917, 45922, 45924, 45926, 45928, 45930, 45935);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45477, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 87930, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - On Aggro - Cast ''Charge'''),
(45477, 0, 1, 0, 0, 0, 100, 0, 8000, 12000, 8000, 12000, 0, 11, 87923, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - In Combat - Cast ''Wind Blast'''),
(45477, 0, 2, 0, 0, 0, 100, 0, 20000, 25000, 20000, 25000, 0, 11, 87933, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - In Combat - Cast ''Air Nova'''),
(45912, 0, 0, 0, 0, 0, 100, 0, 1600, 1600, 2500, 2500, 0, 11, 88032, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wild Vortex - In Combat - Cast ''Lightning Bolt'''),
(45912, 0, 1, 0, 8, 0, 10, 0, 0, 0, 5000, 10000, 0, 11, 88010, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wild Vortex - On SpellHit - Cast ''Cyclone'''),
(45912, 0, 2, 0, 13, 0, 50, 0, 5000, 10000, 0, 0, 0, 11, 88029, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wild Vortex - On Target SpellCast - Cast ''Wind Shock'''),
(45915, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88057, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - On Aggro - Cast ''Rising Winds'''),
(45915, 0, 1, 0, 0, 0, 100, 0, 10000, 12000, 8000, 12000, 0, 11, 88061, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - In Combat - Cast ''Gale Strike'''),
(45915, 0, 2, 0, 0, 0, 100, 0, 12000, 15000, 12000, 15000, 0, 11, 88055, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gust Soldier - In Combat - Cast ''Storm Surge'''),
(45917, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 11, 88081, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cloud Prince - On Aggro - Cast ''Whipping Winds'''),
(45917, 0, 1, 0, 0, 0, 100, 0, 5000, 10000, 5000, 10000, 0, 11, 88073, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cloud Prince - In Combat - Cast ''Starfall'''),
(45917, 0, 2, 0, 0, 0, 100, 0, 10000, 15000, 10000, 15000, 0, 11, 88075, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cloud Prince - In Combat - Cast ''Typhoon'''),
(45922, 0, 0, 0, 0, 0, 100, 0, 20000, 20000, 20000, 20000, 0, 11, 88182, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Empyrean Assassin - In Combat - Cast ''Vapor Form'''),
(45924, 0, 0, 0, 0, 0, 100, 0, 5000, 8000, 12000, 20000, 0, 11, 88175, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Turbulent Squall - In Combat - Cast ''Asphyxiate'''),
(45924, 0, 1, 0, 0, 0, 100, 0, 10000, 15000, 5000, 15000, 0, 11, 88170, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Turbulent Squall - In Combat - Cast ''Cloudburst'''),
(45924, 0, 2, 0, 0, 0, 100, 0, 5000, 20000, 20000, 25000, 0, 11, 88171, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Turbulent Squall - In Combat - Cast ''Hurricane'''),
(45926, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 6000, 6000, 0, 11, 87771, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Servant of Asaad - In Combat - Cast ''Crusader Strike'''),
(45926, 0, 1, 0, 0, 0, 100, 0, 14000, 14000, 12000, 12000, 0, 11, 58127, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Servant of Asaad - In Combat - Cast ''Divine Storm'''),
(45926, 0, 2, 0, 0, 0, 100, 0, 32000, 35000, 32000, 35000, 0, 11, 87772, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Servant of Asaad - In Combat - Cast ''Hand of Protection'''),
(45928, 0, 0, 0, 0, 0, 100, 0, 6000, 6000, 6000, 6000, 0, 11, 78660, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Executor of the Caliph - In Combat - Cast ''Devastate'''),
(45928, 0, 1, 0, 0, 0, 100, 0, 15000, 18000, 15000, 18000, 0, 11, 87759, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Executor of the Caliph - In Combat - Cast ''Shockwave'''),
(45928, 0, 2, 0, 0, 0, 100, 0, 20000, 25000, 20000, 25000, 0, 11, 87761, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Executor of the Caliph - In Combat - Cast ''Rally'''),
(45930, 0, 0, 0, 0, 0, 100, 0, 0, 0, 6000, 6000, 0, 11, 87762, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Minister of Air - In Combat - Cast ''Lightning Lash'''),
(45930, 0, 1, 0, 0, 0, 100, 0, 12000, 18000, 12000, 18000, 0, 11, 87768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Minister of Air - In Combat - Cast ''Lightning Nova'''),
(45935, 0, 0, 0, 0, 0, 100, 0, 500, 1000, 4000, 6000, 0, 11, 88959, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Temple Adept - In Combat - Cast ''Holy Smite'''),
(45935, 0, 1, 0, 0, 0, 100, 0, 3000, 5000, 8000, 10000, 0, 11, 87779, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Temple Adept - In Combat - Cast ''Greater Heal'''),
(45935, 0, 2, 0, 0, 0, 100, 0, 10000, 10000, 12000, 12000, 0, 11, 87780, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Temple Adept - In Combat - Cast ''Desperate Speed''');
