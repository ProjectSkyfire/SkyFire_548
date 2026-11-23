-- The Storm Peaks: quest credit given by a death or a spell hit on creatures that had no SmartAI here.
-- Iron Dwarf Assailant and Iron Sentinel (13005), Restless Frostborn Warrior and Ghost (12977), the three Lightning Forges (12988).
-- SmartAI of 29978, 29984, 30135, 30144, 30209, 30211, 30212
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (29978,29984,30135,30144,30209,30211,30212) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (29978,29984,30135,30144,30209,30211,30212);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(29978, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 30297, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Iron Dwarf Assailant - On Death - Give Kill credit'),
(29984, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 30296, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Iron Sentinel <Servant of Loken> - On Death - Give Kill credit'),
(29984, 0, 1, 0, 9, 0, 100, 0, 0, 10, 12000, 24000, 0, 11, 56490, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Iron Sentinel <Servant of Loken> - On Range - Cast Stomp'),
(29984, 0, 2, 0, 0, 0, 100, 0, 0, 3000, 5000, 8000, 0, 11, 55811, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Iron Sentinel <Servant of Loken> - IC - Cast Throw Bolder'),
(29984, 0, 3, 0, 9, 0, 100, 0, 0, 5, 5000, 7000, 0, 11, 5568, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Iron Sentinel <Servant of Loken> - IC - Cast Trample'),
(30135, 0, 0, 0, 0, 0, 100, 0, 1000, 5000, 8000, 13000, 0, 11, 57456, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Warrior - In Combat - Cast ''Frostbite'' (Phase 1) (No Repeat) (Dungeon)'),
(30135, 0, 1, 2, 8, 0, 100, 0, 55983, 0, 0, 0, 0, 33, 30139, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Warrior - On Spellhit ''Blow Hodir''s Horn'' - Quest Credit ''Blowing Hodir''s Horn'''),
(30135, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Warrior - On Spellhit ''Blow Hodir''s Horn'' - Despawn Instant'),
(30144, 0, 0, 0, 0, 0, 100, 0, 1000, 5000, 8000, 13000, 0, 11, 57456, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Ghost - In Combat - Cast ''Frostbite'''),
(30144, 0, 1, 2, 8, 0, 100, 0, 55983, 0, 0, 0, 0, 33, 30139, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Ghost - On Spellhit ''Blow Hodir''s Horn'' - Quest Credit ''Blowing Hodir''s Horn'''),
(30144, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Restless Frostborn Ghost - On Spellhit ''Blow Hodir''s Horn'' - Despawn Instant'),
(30209, 0, 0, 0, 8, 0, 100, 0, 56275, 0, 0, 0, 0, 33, 30209, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'North Lightning Forge - On Spellhit "Bouldercrags Bomb" - Give Kill Credit'),
(30211, 0, 0, 0, 8, 0, 100, 0, 56275, 0, 0, 0, 0, 33, 30211, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Central Lightning Forge - On Spellhit "Bouldercrags Bomb" - Give Kill Credit'),
(30212, 0, 0, 0, 8, 0, 100, 0, 56275, 0, 0, 0, 0, 33, 30212, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'South Lightning Forge - On Spellhit "Bouldercrags Bomb" - Give Kill Credit');
