-- Rise of the Brotherhood (26322): accepting the quest from Marshal Gryan Stoutmantle (234) in Sentinel Hill is meant to play a
-- scene. The player summons Lieutenant Horatio Laine, Hope Saldean, two investigators, Captain Danuvin, Scout Galiaan, Ripsnarl and
-- a copy of the marshal at the positions of the spells 79663 to 79670 (already in `spell_target_position`), the copy (42750) runs
-- the script, Hope shows herself as Vanessa VanCleef, and the players around get the quest credit (upstream issue 1337). The
-- summoned NPCs had no AI, script or speech, and Gryan did not start the scene. AIName, rows, action lists and speech are as in
-- Legends of Azeroth 5.4.8. The existing speech and first row of Gryan stay as they are. Ripsnarl is an elite with the
-- placeholder level 1 in every source, so he gets the level of the quest (12).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (42744, 42745, 42748, 42749, 42750, 42751, 42752) AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 12, `maxlevel` = 12 WHERE `entry` = 42748 AND `ScriptName` = '';

DELETE FROM `creature_text` WHERE `entry` IN (42744, 42748, 42749, 42750);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42744, 0, 0, 'Just tell me one thing, Vanessa.', 12, 0, 100, 0, 0, 0, ''),
(42744, 1, 0, 'Why''d you have the Furlbrows killed?', 12, 0, 100, 0, 0, 0, ''),
(42748, 0, 0, 'Thank you, my dear.', 12, 0, 100, 1, 0, 0, ''),
(42749, 0, 0, 'You bastards will burn for what you did.', 12, 0, 100, 25, 0, 0, ''),
(42749, 1, 0, 'Hope?  Like the hope Fandral had for bringing our people  back to glory?  Back to immortality?', 12, 0, 100, 1, 0, 0, ''),
(42749, 2, 0, 'Hope is a cruel joke, played upon us by a harsh and uncaring world. There is no Hope, there is only Vanessa. Vanessa VanCleef.', 12, 0, 100, 1, 0, 0, ''),
(42749, 3, 0, 'RISE UP BROTHERHOOD! THE DAWNING DAY IS UPON US!', 14, 0, 100, 22, 0, 0, ''),
(42749, 4, 0, 'Tie them up.', 12, 0, 100, 25, 0, 0, ''),
(42749, 5, 0, 'Admiral, your hat.', 12, 0, 100, 1, 0, 0, ''),
(42749, 6, 0, 'And you, $n, I will spare your life. You have done much to help our cause, albeit unwittingly, but the next time we meet it will be as enemies. ', 12, 0, 100, 25, 0, 0, ''),
(42749, 7, 0, 'I had no choice, lieutenant. They recognized me. The only people in the world who even knew I existed, recognized my face from when I was an infant.', 12, 0, 100, 1, 0, 0, ''),
(42749, 8, 0, 'I took no pleasure in their deaths.', 12, 0, 100, 1, 0, 0, ''),
(42749, 9, 0, 'Leave nothing but ashes in your wake, brothers! Burn Sentinel Hill to the ground!', 14, 0, 100, 22, 0, 0, ''),
(42750, 0, 0, 'I don''t like this, $n. Stay alert!', 12, 0, 100, 1, 0, 0, ''),
(42750, 1, 0, 'Hope! Wha..', 12, 0, 100, 5, 0, 0, ''),
(42750, 2, 0, '$n, get to Stormwind. Tell King Wrynn everything. EVERYTHING! GO NOW!', 12, 0, 100, 0, 0, 0, '');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (234, 42744, 42745, 42748, 42749, 42750, 42751, 42752);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(234, 0, 0, 0, 20, 0, 100, 0, 166, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gryan Stoutmantle - On Quest ''The Defias Brotherhood'' Finished - Say Line 0'),
(234, 0, 1, 0, 19, 0, 100, 0, 26322, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - On Accepted Quest - Set Data 1'),
(234, 0, 2, 0, 38, 0, 100, 0, 1, 0, 120000, 120000, 0, 80, 4275000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - On Data Set 1 - Run Script'),
(42744, 0, 0, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lieutenant Horatio Laine - On Data Set 8 - Despawn'),
(42745, 0, 0, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stormwind Investigator - On Data Set 8 - Despawn'),
(42748, 0, 0, 0, 38, 0, 100, 0, 4, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Ripsnarl - On Data Set 4 - Set Orientation'),
(42748, 0, 1, 0, 38, 0, 100, 0, 5, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, -10515.6, 1061.47, 55.8623, 0, 'Ripsnarl - On Data Set 5 - Move to pos'),
(42748, 0, 2, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ripsnarl - On Data Set 8 - Despawn'),
(42749, 0, 0, 0, 38, 0, 100, 0, 2, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, -10507.1, 1042.37, 60.5173, 0, 'Hope Saldean - On Data Set 2 - Move to pos'),
(42749, 0, 1, 0, 38, 0, 100, 0, 3, 0, 0, 0, 0, 69, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, -10512.1, 1044.35, 60.5181, 0, 'Hope Saldean - On Data Set 3 - Move to pos'),
(42749, 0, 2, 0, 38, 0, 100, 0, 6, 0, 0, 0, 0, 69, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, -10512.2, 1056.16, 57.8817, 0, 'Hope Saldean - On Data Set 6 - Move to pos'),
(42749, 0, 3, 0, 38, 0, 100, 0, 7, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 42744, 50, 0, 0, 0, 0, 0, 'Hope Saldean - On Data Set 7 - Set Orientation'),
(42749, 0, 4, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hope Saldean - On Data Set 8 - Despawn'),
(42750, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4275001, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - On Respawn - Run Script'),
(42751, 0, 0, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Captain Danuvin - On Data Set 8 - Despawn'),
(42752, 0, 0, 0, 38, 0, 100, 0, 8, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scout Galiaan - On Data Set 8 - Despawn');

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4275000, 4275001);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4275000, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79664, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Horatio Laine'),
(4275000, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79665, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Hope Saldean'),
(4275000, 9, 2, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79666, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Stormwind Investigator 01'),
(4275000, 9, 3, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79667, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Stormwind Investigator 02'),
(4275000, 9, 4, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79668, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Captain Danuvin'),
(4275000, 9, 5, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79669, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Scout Galiaan'),
(4275000, 9, 6, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79670, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Ripsnarl'),
(4275000, 9, 7, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 86, 79663, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Summon Gryan Stoutmantle'),
(4275001, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 0'),
(4275001, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 45, 2, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 2'),
(4275001, 9, 2, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Orientation'),
(4275001, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 0'),
(4275001, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 1'),
(4275001, 9, 5, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 1'),
(4275001, 9, 6, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 2'),
(4275001, 9, 7, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 0, 86, 79709, 0, 19, 42749, 50, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Vanessa Transform'),
(4275001, 9, 8, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 3'),
(4275001, 9, 9, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 4'),
(4275001, 9, 10, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 86, 79706, 0, 19, 42749, 50, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast A Lashtail Hatchling: Dig Visual'),
(4275001, 9, 11, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Emote Kneel'),
(4275001, 9, 12, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 19, 42744, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Emote Kneel'),
(4275001, 9, 13, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 9, 42745, 0, 50, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Emote Kneel'),
(4275001, 9, 14, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 19, 42751, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Emote Kneel'),
(4275001, 9, 15, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 90, 8, 0, 0, 0, 0, 0, 19, 42752, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Emote Kneel'),
(4275001, 9, 16, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 45, 3, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 3'),
(4275001, 9, 17, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 5'),
(4275001, 9, 18, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 86, 79745, 0, 19, 42748, 50, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Ripsnarl Transform Human'),
(4275001, 9, 19, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 45, 4, 0, 0, 0, 0, 0, 19, 42748, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 4'),
(4275001, 9, 20, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 42748, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 0'),
(4275001, 9, 21, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 86, 79750, 0, 19, 42748, 50, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Cast Ripsnarl Admiral''s Hat'),
(4275001, 9, 22, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 5, 0, 0, 0, 0, 0, 19, 42748, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 5'),
(4275001, 9, 23, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 6'),
(4275001, 9, 24, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 45, 6, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 6'),
(4275001, 9, 25, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 42744, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 0'),
(4275001, 9, 26, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 42744, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 1'),
(4275001, 9, 27, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 45, 7, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Set Data 7'),
(4275001, 9, 28, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 7'),
(4275001, 9, 29, 0, 0, 0, 100, 0, 12000, 12000, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 8'),
(4275001, 9, 30, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 9'),
(4275001, 9, 31, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 15, 26322, 0, 0, 0, 0, 0, 17, 0, 100, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Questcredit'),
(4275001, 9, 32, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 91, 8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Remove Emote Kneel'),
(4275001, 9, 33, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Say Line 2'),
(4275001, 9, 34, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 35, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 19, 42744, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 36, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 9, 42745, 0, 50, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 37, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 19, 42751, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 38, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 19, 42752, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 39, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 19, 42749, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn'),
(4275001, 9, 40, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 45, 8, 0, 0, 0, 0, 0, 19, 42748, 50, 0, 0, 0, 0, 0, 'Marshal Gryan Stoutmantle - Script - Despawn');
