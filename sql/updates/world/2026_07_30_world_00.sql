-- The Fight Continues
UPDATE `quest_template` SET `OfferRewardText`='Even now, only the S.A.F.E. teams dare to venture far into the city to search for survivors. Meanwhile, Thermaplugg has sent one of his followers, Crushcog, to distract us and buy time for him to dig in.' WHERE `Id`=26208;

-- High Tinker Mekkatorque
DELETE FROM `creature_text` WHERE `entry`=42317;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42317, 0, 0, 'We assembled a mighty force and defeated Thermaplugg\'s servants, fighting our way into the city. But, the usurper Thermaplugg had one last trick up his sleeve...', 12, 0, 100, 0, 0, 0, 'High Tinker Mekkatorque - SAY_0'),
(42317, 1, 0, 'Take a look at this holotable. I want to show you a bit about Operation: Gnomeregan.', 12, 0, 100, 25, 0, 0, 'High Tinker Mekkatorque - SAY_1'),
(42317, 2, 0, 'We were forced to retreat. Thermaplugg continues to hide behind the radiation, sending his crony, Crushcog, to harass us here on the surface.', 12, 0, 100, 25, 0, 0, 'High Tinker Mekkatorque - SAY_2');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42317;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42317;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42317, 0, 0, 0, 19, 0, 100, 0, 26208, 2000, 2000, 0, 0, 80, 4231700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Quest Accept - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4231700;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4231700, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Talk 0'),
(4231700, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -5132.874, 491.64185, 395.915, 0, 'High Tinker Mekkatorque - On Script - Move Position'),
(4231700, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Talk 1'),
(4231700, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42422, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5135.18994, 494.82998, 396.41934, 5.5676, 'High Tinker Mekkatorque - On Script - Summon Image of Hinkles Fastblast'),
(4231700, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42452, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5132.72998, 493.80599, 396.55935, 2.11184, 'High Tinker Mekkatorque - On Script - Summon Irradiator 3000'),
(4231700, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42441, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5133.27978, 496.009, 396.41934, 4.43313, 'High Tinker Mekkatorque - On Script - Summon Irradiated Trogg Corpse 1'),
(4231700, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42441, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5134.97998, 494.03298, 396.41833, 0.75049, 'High Tinker Mekkatorque - On Script - Summon Irradiated Trogg Corpse 2'),
(4231700, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42419, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5134.8999, 495.59201, 396.41833, 5.61996, 'High Tinker Mekkatorque - On Script - Summon Image of High Tinker Mekkatorque'),
(4231700, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 42420, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5134.27001, 496.13198, 396.41934, 5.53269, 'High Tinker Mekkatorque - On Script - Summon Image of "Doc" Cogspin'),
(4231700, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 203862, 70000, 0, 0, 0, 0, 8, 0, 0, 0, -5133.672, 494.83682, 395.42572, 0, 'High Tinker Mekkatorque - On Script - Summon Op: Gnomeregan Recap Light'),
(4231700, 9, 10, 0, 0, 0, 100, 0, 11000, 11000, 0, 0, 0, 12, 42423, 1, 120000, 0, 0, 0, 8, 0, 0, 0, -5133.4707, 494.375, 396.44332, 1.6406, 'High Tinker Mekkatorque - On Script - Summon Thermaplugg\'s Brag-bot'),
(4231700, 9, 11, 0, 0, 0, 100, 0, 67000, 67000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Talk 3'),
(4231700, 9, 12, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 33, 42463, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'High Tinker Mekkatorque - On Script - Give Credit'),
(4231700, 9, 13, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, -5131.25, 488.022, 395.586, 2.30383, 'High Tinker Mekkatorque - On Script - Move Position');


-- Image of High Tinker Mekkatorque
DELETE FROM `creature_text` WHERE `entry`=42419;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42419, 0, 0, 'It\'s too quiet. Where are Thermaplugg\'s defense forces?', 12, 0, 100, 0, 0, 0, 'Image of High Tinker Mekkatorque - SAY_0'),
(42419, 1, 0, 'IT\'S A TRAP! That\'s a fully functional irradiator!', 12, 0, 100, 0, 0, 0, 'Image of High Tinker Mekkatorque - SAY_1'),
(42419, 2, 0, 'Ten minutes? Plenty of time to disarm the device.', 12, 0, 100, 0, 0, 0, 'Image of High Tinker Mekkatorque - SAY_2'),
(42419, 3, 0, 'Hinkles, hand me that Whirring Bronze Gizmo and some Fresh Spring Water. Cogspin, toss me that Dirty Trogg Cloth and a Handful of Copper Bolts...', 12, 0, 100, 0, 0, 0, 'Image of High Tinker Mekkatorque - SAY_3'),
(42419, 4, 0, 'Whuh-oh! We\'ve got to get out of here! Lapforge, beam us back, quickly!', 12, 0, 100, 0, 0, 0, 'Image of High Tinker Mekkatorque - SAY_3');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42419;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42419;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42419, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4241900, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Respawn - Run Script');

-- 12363 blue -- holo 46684
DELETE FROM `smart_scripts` WHERE `entryorguid`=4241900;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4241900, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 43, 46684, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Mount Up'),
(4241900, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Talk 0'),
(4241900, 9, 2, 0, 0, 0, 100, 0, 23000, 23000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Talk 1'),
(4241900, 9, 3, 0, 0, 0, 100, 0, 16000, 16000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Talk 2'),
(4241900, 9, 4, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Talk 3'),
(4241900, 9, 5, 0, 0, 0, 100, 0, 17000, 17000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Talk 4'),
(4241900, 9, 6, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 42422, 15, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Set Data to Image of Hinkles Fastblast'),
(4241900, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 42420, 15, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Set Data to Image of "Doc" Cogspin'),
(4241900, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51347, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Cast Spell Teleport'),
(4241900, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of High Tinker Mekkatorque - On Script - Despawn');

-- Image of Hinkles Fastblast
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42422;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42422;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42422, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 43, 12363, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Respawn - Mount Up'),
(42422, 0, 1, 0, 38, 0, 100, 0, 1, 1, 1000, 1000, 0, 80, 4242200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - Data Set - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4242200;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4242200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51347, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Script - Cast Spell Teleport'),
(4242200, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Script - Despawn');

-- Image of "Doc" Cogspin
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42420;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42420;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42420, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 43, 12363, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Respawn - Mount Up'),
(42420, 0, 1, 0, 38, 0, 100, 0, 1, 1, 1000, 1000, 0, 80, 4242000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - Data Set - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4242000;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4242000, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51347, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Script - Cast Spell Teleport'),
(4242000, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Image of Hinkles Fastblast - On Script - Despawn');

-- Thermaplugg's Brag-bot
DELETE FROM `creature_text` WHERE `entry`=42423;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42423, 0, 0, 'No! No, no, NO!!! I won\'t allow you in my kingdom, usurper! I WON\'T ALLOW IT!', 12, 0, 100, 0, 0, 0, 'Thermaplugg\'s Brag-bot - SAY_0'),
(42423, 1, 0, 'Activate the Ultimate Atomic Protection System.', 12, 0, 100, 0, 0, 0, 'Thermaplugg\'s Brag-bot - SAY_1'),
(42423, 2, 0, '...DISINTEGRATE THEM ALL!', 12, 0, 100, 0, 0, 0, 'Thermaplugg\'s Brag-bot - SAY_2'),
(42423, 3, 0, 'TEN MINUTES?! You left the factory setting on? Give me that controller, you idiot!', 12, 0, 100, 0, 0, 0, 'Thermaplugg\'s Brag-bot - SAY_3');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42423;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42423;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42423, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4242300, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Respawn - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4242300;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4242300, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Script - Talk 0'),
(4242300, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Script - Talk 1'),
(4242300, 9, 2, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Script - Talk 2'),
(4242300, 9, 3, 0, 0, 0, 100, 0, 36000, 36000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Script - Talk 3'),
(4242300, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 19000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thermaplugg\'s Brag-bot - On Script - Despawn');

-- Irradiator 3000
DELETE FROM `creature_text` WHERE `entry`=42452;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42452, 0, 0, 'Irradiator 3000 activated. Attention. Emergency. You now have ten minutes to reach minimum safe distance.', 12, 0, 100, 0, 0, 0, 'Irradiator 3000 - SAY_0'),
(42452, 1, 0, '*BEEP* Attention. Emergency. You now have ten seconds to reach minimum safe distance.', 12, 0, 100, 0, 0, 0, 'Irradiator 3000 - SAY_1');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42452;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42452;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42452, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4245200, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Irradiator 3000 - On Respawn - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4245200;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4245200, 9, 0, 0, 0, 0, 100, 0, 32000, 32000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Irradiator 3000 - On Script - Talk 0'),
(4245200, 9, 1, 0, 0, 0, 100, 0, 33000, 33000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Irradiator 3000 - On Script - Talk 1'),
(4245200, 9, 2, 0, 0, 0, 100, 0, 13000, 13000, 0, 0, 0, 11, 46419, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Irradiator 3000 - On Script - Cast Explosion'),
(4245200, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Irradiator 3000 - On Script - Despawn');

-- Trogg Corpse
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=42441;

DELETE FROM `smart_scripts` WHERE `entryorguid`=42441;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(42441, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4244100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trogg Corpse - On Respawn - Run Script');

DELETE FROM `smart_scripts` WHERE `entryorguid`=4244100;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4244100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 77000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Trogg Corpse - On Script - Despawn');
