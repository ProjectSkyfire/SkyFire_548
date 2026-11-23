-- Howling Fjord: quest credit for objectives that are given by a spell hit, a link or a death.
-- The creatures had no SmartAI here, so these quests could not be completed.
-- SmartAI of 23876, 23957, 23972, 24182, 24183, 24184, 24185, 24210, 24290, 24874, 24875, 24876, 24877
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (23876,23957,23972,24182,24183,24184,24185,24210,24290,24874,24875,24876,24877) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (23876,23957,23972,24182,24183,24184,24185,24210,24290,24874,24875,24876,24877);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(23876, 0, 0, 0, 0, 0, 100, 0, 5000, 12000, 23000, 28000, 0, 11, 42526, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Spore - In Combat - Cast ''Spore Cloud'''),
(23876, 0, 1, 2, 8, 0, 100, 1, 43354, 0, 0, 0, 0, 33, 51087, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On SpellHit kill credit for quest 11319'),
(23876, 0, 2, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 41, 6000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Despawn'),
(23957, 0, 0, 0, 8, 0, 100, 0, 43076, 0, 0, 0, 0, 33, 23957, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Westguard Cannon Credit Trigger East - On Spellhit ''Mark Cannon'' - Quest Credit ''Guide Our Sights'' (No Repeat) (Dungeon)'),
(23972, 0, 0, 0, 8, 0, 100, 0, 43076, 0, 0, 0, 0, 33, 23972, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Westguard Cannon Credit Trigger West - On Spellhit ''Mark Cannon'' - Quest Credit ''Guide Our Sights'' (No Repeat) (Dungeon)'),
(24182, 0, 0, 0, 8, 0, 100, 0, 43233, 0, 0, 0, 0, 33, 24182, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterskorn Dwelling Credit - On Spellhit ''Burn Structure'' - Quest Credit ''Baleheim Must Burn!'''),
(24183, 0, 0, 0, 8, 0, 100, 0, 43233, 0, 0, 0, 0, 33, 24183, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterskorn Watchtower Credit - On Spellhit ''Burn Structure'' - Quest Credit ''Baleheim Must Burn!'''),
(24184, 0, 0, 0, 8, 0, 100, 0, 43233, 0, 0, 0, 0, 33, 24184, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterskorn Barracks Credit - On Spellhit ''Burn Structure'' - Quest Credit ''Baleheim Must Burn!'''),
(24185, 0, 0, 0, 8, 0, 100, 0, 43233, 0, 0, 0, 0, 33, 24185, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Winterskorn Bridge Credit - On Spellhit ''Burn Structure'' - Quest Credit ''Baleheim Must Burn!'''),
(24210, 0, 0, 1, 6, 0, 25, 0, 0, 0, 0, 0, 0, 11, 43289, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Riven Widow Cocoon - On Just Died - Cast ''Summon Freed Mist Whisper Scout'''),
(24210, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 24211, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Riven Widow Cocoon - On Just Died - Quest Credit ''Rivenwood Captives'''),
(24290, 0, 0, 0, 8, 0, 100, 0, 43404, 0, 0, 0, 0, 33, 24290, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'On Spell Hit - kill credit'),
(24874, 0, 0, 1, 8, 0, 100, 1, 45323, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fengir the Disgraced - On Spellhit ''Returning Vrykul Artifact'' - Say Line 0 (No Repeat)'),
(24874, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 24874, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fengir the Disgraced - On Spellhit ''Returning Vrykul Artifact'' - Quest Credit ''A Return to Resting'' (No Repeat)'),
(24875, 0, 0, 1, 8, 0, 100, 1, 45323, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Windan of the Kvaldir - On Spellhit ''Returning Vrykul Artifact'' - Say Line 0 (No Repeat)'),
(24875, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 24875, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Windan of the Kvaldir - On Spellhit ''Returning Vrykul Artifact'' - Quest Credit ''A Return to Resting'' (No Repeat)'),
(24876, 0, 0, 1, 8, 0, 100, 1, 45323, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rodin the Reckless - On Spellhit ''Returning Vrykul Artifact'' - Say Line 0 (No Repeat)'),
(24876, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 24876, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rodin the Reckless - On Spellhit ''Returning Vrykul Artifact'' - Quest Credit ''A Return to Resting'' (No Repeat)'),
(24877, 0, 0, 1, 8, 0, 100, 0, 45323, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Isuldof Iceheart - On Spellhit ''Returning Vrykul Artifact'' - Say Line 0'),
(24877, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 24877, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Isuldof Iceheart - On Spellhit ''Returning Vrykul Artifact'' - Quest Credit ''A Return to Resting''');
DELETE FROM `creature_text` WHERE `entry` = 24874 AND `groupid` IN (0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24874, 0, 0, 'Your offering has come too late, little one. Can you feel the mist closing in upon you? The Kvaldir return...', 12, 0, 100, 0, 0, 0, 'Fengir the Disgraced');
DELETE FROM `creature_text` WHERE `entry` = 24875 AND `groupid` IN (0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24875, 0, 0, 'My brothers have awoken. Your efforts are wasted...', 12, 0, 100, 0, 0, 0, 'Windan of the Kvaldir');
DELETE FROM `creature_text` WHERE `entry` = 24876 AND `groupid` IN (0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24876, 0, 0, 'From the mist and fog the Kvaldir approach. Flee while you still breathe the air of the living...', 12, 0, 100, 0, 0, 0, 'Rodin the Reckless');
DELETE FROM `creature_text` WHERE `entry` = 24877 AND `groupid` IN (0);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24877, 0, 0, 'Look to the seas, as your doom comes with the swell of tides.', 12, 0, 100, 0, 0, 0, 'Isuldof Iceheart');
