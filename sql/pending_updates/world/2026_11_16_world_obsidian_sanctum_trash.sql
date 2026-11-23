-- The Obsidian Sanctum: SmartAI for the Onyx Sanctum Guardian, Brood General, Blaze Mistress and Flight Captain
-- Ported from TrinityCore 3.3.5. TrinityCore's 10 and 25 player difficulty bits are moved to the 10 and 25 player normal bits, and the 25 player casts that have no id in the 5.4.8 client data use the 10 player id, whose 25 player numbers the client data holds.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (30453,30680,30681,30682) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (30453,30680,30681,30682);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(30453, 0, 0, 0, 0, 0, 100, 8, 4000, 5500, 25000, 31000, 0, 11, 58948, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cast Curse of Mending'),
(30453, 0, 1, 0, 0, 0, 100, 16, 4000, 5500, 25000, 31000, 0, 11, 39647, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cast Curse of Mending'),
(30453, 0, 2, 0, 0, 0, 100, 8, 9000, 9000, 17800, 19300, 0, 11, 57728, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Shockwave'),
(30453, 0, 3, 0, 0, 0, 100, 16, 9000, 9000, 17800, 19300, 0, 11, 58947, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Shockwave'),
(30453, 0, 4, 0, 2, 0, 100, 25, 0, 30, 0, 0, 0, 11, 53801, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Frenzy at 30% HP'),
(30453, 0, 5, 0, 2, 0, 100, 25, 0, 30, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Say Text at 30% HP'),
(30453, 0, 6, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Onyx Sanctum Guardian -  On init -  say text'),
(30680, 0, 0, 0, 4, 0, 100, 9, 0, 0, 0, 0, 0, 11, 57740, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Devotion Aura on Aggro'),
(30680, 0, 1, 0, 4, 0, 100, 17, 0, 0, 0, 0, 0, 11, 58944, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Devotion Aura on Aggro'),
(30680, 0, 2, 0, 0, 0, 100, 24, 4500, 4500, 11500, 13500, 0, 11, 13737, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Mortal Strike Shock'),
(30680, 0, 3, 0, 0, 0, 100, 25, 6000, 6000, 0, 0, 0, 11, 57742, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Avenging Fury Shock'),
(30680, 0, 4, 0, 2, 0, 100, 9, 0, 30, 0, 0, 0, 11, 57733, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Draconic Rage at 30% HP'),
(30680, 0, 5, 0, 2, 0, 100, 17, 0, 30, 0, 0, 0, 11, 58942, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Draconic Rage at 30% HP'),
(30680, 0, 6, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Onyx Brood General -  On init -  say text'),
(30681, 0, 0, 0, 0, 0, 100, 8, 5000, 5000, 12000, 13000, 0, 11, 39529, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Flame Shock'),
(30681, 0, 1, 0, 0, 0, 100, 16, 5000, 5000, 12000, 13000, 0, 11, 58940, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Flame Shock'),
(30681, 0, 2, 0, 0, 0, 100, 8, 8000, 8000, 19000, 22000, 0, 11, 57757, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cast Rain of Fire'),
(30681, 0, 3, 0, 0, 0, 100, 16, 8000, 8000, 19000, 22000, 0, 11, 58936, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cast Rain of Fire'),
(30681, 0, 4, 0, 0, 0, 100, 24, 3000, 11000, 8000, 15000, 0, 11, 57753, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Cast Conjure Flame Orb'),
(30681, 0, 5, 0, 8, 0, 100, 0, 57753, 0, 0, 0, 0, 11, 57752, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Onyx Blaze Mistress - On Spellhit ''Conjure Flame Orb'' - Cast ''Flame Orb Summon'''),
(30682, 0, 0, 0, 13, 0, 100, 24, 12000, 14000, 0, 0, 0, 11, 58953, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Pummel'),
(30682, 0, 1, 0, 0, 0, 100, 24, 8000, 9000, 15000, 21000, 0, 11, 57759, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Cast Hammer Drop');

INSERT IGNORE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(30453, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Onyx Sanctum Guardian'),
(30453, 1, 0, 'Sanctum Guardians reporting in!', 14, 0, 100, 0, 0, 0, 'Onyx Sanctum Guardian'),
(30680, 0, 0, 'Brood Guardians reporting in!', 14, 0, 100, 0, 0, 0, 'Onyx Brood General');
