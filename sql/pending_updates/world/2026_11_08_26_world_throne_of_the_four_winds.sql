-- Throne of the Four Winds: script bindings for the Conclave of Wind and Al'Akir, their adds and triggers, the area
-- triggers of the slipstreams and the fall catcher, the boss texts, and the slipstreams and ice storm triggers that were
-- not spawned. Safe to run twice.
UPDATE `instance_template` SET `script` = 'instance_throne_of_the_four_winds' WHERE `map` = 754;
UPDATE `creature_template` SET `ScriptName` = 'npc_fall_catcher' WHERE `entry` = 45710;
UPDATE `creature_template` SET `ScriptName` = 'npc_ravenous_creeper' WHERE `entry` = 45812;
UPDATE `creature_template` SET `ScriptName` = 'npc_ravenous_creeper_trigger' WHERE `entry` = 45813;
UPDATE `creature_template` SET `ScriptName` = 'boss_anshal' WHERE `entry` = 45870;
UPDATE `creature_template` SET `ScriptName` = 'boss_nezir' WHERE `entry` = 45871;
UPDATE `creature_template` SET `ScriptName` = 'boss_rohash' WHERE `entry` = 45872;
UPDATE `creature_template` SET `ScriptName` = 'npc_tornado' WHERE `entry` = 46207;
UPDATE `creature_template` SET `ScriptName` = 'npc_soothing_breeze' WHERE `entry` = 46246;
UPDATE `creature_template` SET `ScriptName` = 'npc_ice_storm_initial' WHERE `entry` = 46734;
UPDATE `creature_template` SET `ScriptName` = 'boss_alakir' WHERE `entry` = 46753;
UPDATE `creature_template` SET `ScriptName` = 'npc_ice_storm_ground' WHERE `entry` = 46973;
UPDATE `creature_template` SET `ScriptName` = 'npc_squall_line_vehicle' WHERE `entry` = 47034;
UPDATE `creature_template` SET `ScriptName` = 'npc_slipstream_raid' WHERE `entry` = 47066;
UPDATE `creature_template` SET `ScriptName` = 'npc_stormling' WHERE `entry` = 47175;
UPDATE `creature_template` SET `ScriptName` = 'npc_lightning_clouds' WHERE `entry` = 48190;
UPDATE `creature_template` SET `ScriptName` = 'npc_squall_line_vehicle' WHERE `entry` = 48852;
UPDATE `creature_template` SET `ScriptName` = 'npc_squall_line' WHERE `entry` = 48854;
UPDATE `creature_template` SET `ScriptName` = 'npc_squall_line' WHERE `entry` = 48855;
UPDATE `creature_template` SET `ScriptName` = 'npc_lightning_strike_trigger' WHERE `entry` = 48977;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (86481, 86489, 86490, 86491, 86492, 87652, 87855, 88858, 89588, 89667);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(86481, 'spell_hurricane'),
(86489, 'spell_hurricane'),
(86490, 'spell_hurricane'),
(86491, 'spell_hurricane'),
(86492, 'spell_hurricane'),
(87652, 'spell_squall_line_activate'),
(87855, 'spell_squall_line'),
(88858, 'spell_wind_burst'),
(89588, 'spell_lightning_clouds'),
(89667, 'spell_lightning_rod');
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (6274, 6275, 6276, 6277, 6278, 6279, 6280, 6281, 6290);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(6274, 'at_slipstream'),
(6275, 'at_slipstream'),
(6276, 'at_slipstream'),
(6277, 'at_slipstream'),
(6278, 'at_slipstream'),
(6279, 'at_slipstream'),
(6280, 'at_slipstream'),
(6281, 'at_catch_fall'),
(6290, 'at_slipstream');
DELETE FROM `creature_text` WHERE `entry` IN (45870, 45871, 45872, 46753);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45870, 0, 0, 'It shall be I that earns the favor of our lord by casting out the intruders. My calmest wind shall still prove too much for them!', 14, 0, 100, 0, 0, 0, 'Anshal - Aggro'),
(45870, 1, 0, 'You think to outrun the wind? A fatal mistake.', 14, 0, 100, 0, 0, 0, 'Anshal - Out of Range'),
(45870, 1, 1, 'Your presence shall no longer defile our home!', 14, 0, 100, 0, 0, 0, 'Anshal - SAY_ANSHAL_KILL'),
(45870, 2, 0, 'My power grows feeble, brothers. I shamefully must rely on you for a time.', 14, 0, 100, 0, 0, 0, 'Anshal - Gather Strength'),
(45870, 3, 0, '%s begins to gather strength from the remaining Wind Lords!', 41, 0, 100, 0, 0, 0, 'Anshal - Announce Gather Strength'),
(45870, 4, 0, 'The power of our winds, UNLEASHED!', 14, 0, 100, 0, 0, 0, 'Anshal - Special Ability'),
(45870, 5, 0, 'Begone, outsider!', 14, 0, 100, 0, 0, 0, 'Anshal - Slay 1'),
(45870, 5, 1, 'Your presence shall no longer defile our home!', 14, 0, 100, 0, 0, 0, 'Anshal - Slay 2'),
(45870, 6, 0, '%s begins to gather strength from the remaining Wind Lords!', 41, 0, 100, 0, 0, 0, 'Anshal - SAY_ANSHAL_GATHER_ONE'),
(45870, 7, 0, 'The Conclave of Wind is nearing full strength!', 41, 0, 100, 0, 0, 0, 'Anshal - SAY_ULTIMATE_ANNOUNCE'),
(45871, 0, 0, 'The honor of slaying the interlopers shall be mine, brothers! Their feeble bodies will freeze solid from my wind''s icy chill!', 14, 0, 100, 0, 0, 0, 'Nezir - Aggro'),
(45871, 1, 0, 'You throw away your honor and flee as cowards? Then die!', 14, 0, 100, 0, 0, 0, 'Nezir - Out of Range'),
(45871, 1, 1, 'Another mortal has taken their last breath!', 14, 0, 100, 0, 0, 0, 'Nezir - SAY_NEZIR_KILL'),
(45871, 2, 0, 'Brothers, beware! These mortals are dangerous. I must pause and gather my strength.', 14, 0, 100, 0, 0, 0, 'Nezir - Gather Strength'),
(45871, 3, 0, '%s begins to gather strength from the remaining Wind Lords!', 41, 0, 100, 0, 0, 0, 'Nezir - Announce Gather Strength'),
(45871, 4, 0, 'The power of our winds, UNLEASHED!', 14, 0, 100, 0, 0, 0, 'Nezir - Special Ability'),
(45871, 5, 0, 'Frozen solid.', 14, 0, 100, 0, 0, 0, 'Nezir - Slay 1'),
(45871, 5, 1, 'Another mortal has taken their last breath!', 14, 0, 100, 0, 0, 0, 'Nezir - Slay 2'),
(45871, 6, 0, '%s begins to gather strength from the remaining Wind Lords!', 41, 0, 100, 0, 0, 0, 'Nezir - SAY_NEZIR_GATHER_ONE'),
(45872, 0, 0, 'As I am the strongest wind, it shall be I that tears the invaders apart!', 14, 0, 100, 0, 0, 0, 'Rohash - Aggro'),
(45872, 1, 0, 'Why do you flee, mortals? There is nowhere you can run or hide here!', 14, 0, 100, 0, 0, 0, 'Rohash - Out of Range'),
(45872, 1, 1, 'Blown away!', 14, 0, 100, 0, 0, 0, 'Rohash - SAY_ROHASH_KILL'),
(45872, 2, 0, 'The intruders stand fast, brothers, I cannot break them. Allow me a brief respite to strengthen my winds.', 14, 0, 100, 0, 0, 0, 'Rohash - Gather Strength'),
(45872, 3, 0, '%s begins to gather strength from the remaining Wind Lords!', 41, 0, 100, 0, 0, 0, 'Rohash - Announce Gather Strength'),
(45872, 4, 0, 'The power of our winds, UNLEASHED!', 14, 0, 100, 0, 0, 0, 'Roash - Special Ability'),
(45872, 5, 0, 'Blown away!', 14, 0, 100, 0, 0, 0, 'Rohash - Slay 1'),
(45872, 5, 1, 'Mere dust...', 14, 0, 100, 0, 0, 0, 'Roash - Slay 2'),
(45872, 6, 0, '|TInterface\\Icons\\spell_nature_cyclone.blp:20|t%s begins to cast|cFFFF9A20|Hspell:86193|h [Wind Blast]!|r', 41, 0, 100, 0, 0, 0, 'Rohash - Announce Wind Blast'),
(46753, 0, 0, 'The Conclave of Wind has dissipated. Your honorable conduct and determination have earned you the right to face me in battle, mortals. I await your assault on my platform! Come!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Conclave of Wind defeated'),
(46753, 1, 0, 'Your challenge is accepted, mortals! Know that you face Al''Akir, Elemental Lord of Air! You have no hope of defeating me!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Aggro'),
(46753, 2, 0, '|TINTERFACE\\ICONS\\Spell_Frost_WindWalkOn.blp:20|t%s begins to cast |cFFFF0000|Hspell:87770|h[Wind Burst]|h|r!', 41, 0, 100, 0, 0, 0, 'Al''Akir - Announce Wind Burst'),
(46753, 3, 0, 'Winds! Obey my command!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Squall Line'),
(46753, 4, 0, 'Your futile persistance angers me!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Phase 2'),
(46753, 5, 0, 'Storms! I summon you to my side!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Stormling'),
(46753, 6, 0, 'Enough! I will no longer be contained!', 14, 0, 100, 0, 0, 0, 'Al''Akir - Phase 3'),
(46753, 6, 1, 'This little one shall vex me no longer.', 14, 0, 100, 0, 0, 0, 'AlAkir - SAY_KILL 2'),
(46753, 7, 0, 'This little one shall vex me no longer.', 14, 0, 100, 0, 0, 0, 'Al''Akir - Slay 1'),
(46753, 7, 1, 'Like swatting insects...', 14, 0, 100, 0, 0, 0, 'Al''Akir - Slay 2'),
(46753, 8, 0, 'After every storm, comes the calm...', 14, 0, 100, 0, 0, 0, 'Al''Akir - Death');
DELETE FROM `creature` WHERE `guid` BETWEEN 9240100 AND 9240114;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9240100, 46766, 754, 120, 0, 0, 16925, 0, -88.4149, 839.203, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240101, 46766, 754, 120, 0, 0, 16925, 0, -92.0469, 791.96, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240102, 46766, 754, 120, 0, 0, 16925, 0, -52.5764, 766.026, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240103, 46766, 754, 120, 0, 0, 16925, 0, -24.9896, 772.418, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240104, 46766, 754, 120, 0, 0, 16925, 0, -77.276, 775.408, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240105, 46766, 754, 120, 0, 0, 16925, 0, -73.5833, 855.875, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240106, 46766, 754, 120, 0, 0, 16925, 0, -95.9913, 814.078, 191.208, 0, 7200, 0, 0, 0, 0, 0, 0, 0, 0),
(9240107, 47066, 754, 120, 0, 0, 0, 0, -237.767, 865.41, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240108, 47066, 754, 120, 0, 0, 0, 0, -96.6776, 1004.52, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240109, 47066, 754, 120, 0, 0, 0, 0, 2.17354, 1002.03, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240110, 47066, 754, 120, 0, 0, 0, 0, 137.942, 865.361, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240111, 47066, 754, 120, 0, 0, 0, 0, 138.48, 763.254, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240112, 47066, 754, 120, 0, 0, 0, 0, -1.87219, 625.81, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240113, 47066, 754, 120, 0, 0, 0, 0, -100.734, 626.52, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(9240114, 47066, 754, 120, 0, 0, 0, 0, -237.337, 765.919, 196.9, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0);

-- The Ravenous Creeper and the Stormling were level 1 placeholders and friendly. Level, faction and flags from the 5.4.8
-- data; melee stats from the live level 85 elite templates.
UPDATE `creature_template` SET `minlevel` = 88, `maxlevel` = 88, `exp` = 3, `faction_A` = 14, `faction_H` = 14, `unit_class` = 1, `mindmg` = 2568, `maxdmg` = 4366, `attackpower` = 1004, `dmg_multiplier` = 2.25, `baseattacktime` = 2000, `rangeattacktime` = 2000 WHERE `entry` IN (45812, 47175);
