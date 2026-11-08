-- Mount Hyjal: the quest creatures that spells summon had no AI, texts or loot: Spawn of Smolderos (credit for its quest when
-- fed), Kristoff Manheim (summoned form and the cross-cast one), Thisalee Crow (holds Marion Wormwing hostage), Thol'embaar and
-- his summon bunny (drops the Black Heart of Thol'embaar), and the two death summons of Lycanthoth (the spirits of Lo'Gosh and
-- Goldrinn, whose conditions already exist).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (39659, 39640, 39797, 40107, 40121, 41114) AND `AIName` = '';
UPDATE `creature_template` SET `lootid` = 40107 WHERE `entry` = 40107 AND `lootid` = 0;

DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` IN (39659, 39640, 39797, 40107, 40121, 41114)) OR (`source_type` = 9 AND `entryorguid` IN (4012100, 4111401, 4111402)) OR (`source_type` = 0 AND `entryorguid` = 39446 AND `id` IN (1, 2));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39640, 0, 0, 1, 19, 0, 100, 0, 25332, 0, 0, 0, 0, 86, 74351, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Accepted Quest - Cross Cast Shadowed by Kristoff'),
(39640, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 0 - Despawn'),
(39659, 0, 0, 0, 8, 0, 100, 0, 74142, 0, 0, 0, 0, 33, 39673, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spawn of Smolderos - Give Kill Credit'),
(39797, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Summoned - Set React State Passive'),
(39797, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 24222, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 0 - Cast Vanish Visual'),
(39797, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 39797, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim -Linked To Id 1 - Create Timed Event'),
(39797, 0, 3, 0, 59, 0, 100, 0, 39797, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Timed Event Triggered - Say Line 0'),
(39797, 0, 4, 5, 52, 0, 100, 0, 0, 39797, 0, 0, 0, 11, 58506, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Text Over 0 - Cast Stealth'),
(39797, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 21, 23, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 4 - Follow Target'),
(39797, 0, 6, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 11, 58506, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Evade - Cast Stealth'),
(39797, 0, 7, 8, 38, 0, 100, 1, 0, 1, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 10, 8267270, 35845, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Data Set 0 1 - Follow Target'),
(39797, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 39808, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 7 - Quest Credit'),
(39797, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 74351, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 8 - Remova Aura Shadowed by Kristoff'),
(39797, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - Linked To Id 9 - Say Line 1'),
(39797, 0, 11, 0, 52, 0, 100, 0, 1, 39797, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kristoff Manheim - On Text Over 1 - Despawn'),
(40107, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4010700, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - On just summoned - Run script'),
(40107, 0, 1, 0, 9, 0, 100, 0, 0, 8, 15800, 18300, 0, 11, 80600, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Cast Flame Stomp on Close'),
(40107, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - On death - Say line 1'),
(40107, 0, 3, 0, 21, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar - Reached home - Force despawn'),
(40121, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4012100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - On just summoned - Run script'),
(41114, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Summoned - Set Run 1'),
(41114, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 1, 0, 0, 19, 41112, 30, 0, 0, 0, 0, 0, 'Thisalee Crow - Linked To Id 0 - Move To Pos'),
(41114, 0, 2, 3, 75, 0, 100, 1, 0, 41112, 1, 1000, 0, 11, 76863, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Distance Creature - Cast Thisalee Transform Aura'),
(41114, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 76868, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - Linked To Id 2 - Cast Holding Hostage'),
(41114, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 41169, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - Linked To Id 3 - Quest Credit'),
(41114, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 41112, 1, 0, 0, 0, 0, 0, 'Thisalee Crow - Linked To Id 4 - Set Data 0 1'),
(41114, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 41114, 1000, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - Linked To Id 5 - Create Timed Event'),
(41114, 0, 7, 0, 59, 0, 100, 0, 41114, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Timed Event Triggered - Say Line 0'),
(41114, 0, 8, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Data Set 0 1 - Say Line 1 1'),
(41114, 0, 9, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 80, 4111401, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Data Set 0 2 - Run Script'),
(41114, 0, 10, 0, 38, 0, 100, 0, 0, 3, 0, 0, 0, 80, 4111402, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Data Set 0 3 - Run Script'),
(4012100, 9, 0, 0, 0, 0, 100, 0, 0, 1000, 0, 0, 0, 11, 74796, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - Script - Cast Flame Circle'),
(4012100, 9, 1, 0, 0, 0, 100, 0, 2000, 3000, 0, 0, 0, 11, 74787, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - Script - Ground Burst and Lava'),
(4012100, 9, 2, 0, 0, 0, 100, 0, 0, 1000, 0, 0, 0, 12, 40107, 1, 120000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - Script - Summon Thol''embaar'),
(4012100, 9, 3, 0, 0, 0, 100, 0, 0, 1000, 0, 0, 0, 28, 74787, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - Script - Remove Ground Burst and Lava'),
(4012100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thol''embaar Summon Bunny - Script - Force despawn'),
(4111401, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 41170, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Quest Credit'),
(4111401, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 76868, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Remove Aura Holding Hostage'),
(4111401, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 57346, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Remove Aura Ride Vehicle'),
(4111401, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Say Line 2'),
(4111401, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 42880, 0, 0, 0, 0, 0, 19, 41112, 10, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Cast Cosmetic - Combat Attack 1H'),
(4111401, 9, 5, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 52866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Cast Transform Visual'),
(4111401, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 3, 41114, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Set Morph To Entry'),
(4111401, 9, 7, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Say Line 4'),
(4111401, 9, 8, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 53, 1, 41114, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Waypoint Start'),
(4111401, 9, 9, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Despawn'),
(4111402, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 33, 41170, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Quest Credit'),
(4111402, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 76868, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Remove Aura Holding Hostage'),
(4111402, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 28, 57346, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Remove Aura Ride Vehicle'),
(4111402, 9, 3, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Say Line 3'),
(4111402, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 19, 41112, 10, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Set Data 0 2'),
(4111402, 9, 5, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 11, 52866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Cast Transform Visual'),
(4111402, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 3, 41114, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Set Morph To Entry'),
(4111402, 9, 7, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 21, 10, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Say Line 4'),
(4111402, 9, 8, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 53, 1, 41114, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Waypoint Start'),
(4111402, 9, 9, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thisalee Crow - On Script - Despawn'),
(39446, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 12, 39622, 1, 60000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lycanthoth - Summon Spirit of Lo''Gosh'),
(39446, 0, 2, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 12, 39627, 1, 60000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lycanthoth - Summon Spirit of Goldrinn');

DELETE FROM `creature_text` WHERE `entry` IN (40107, 39797, 41114);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39797, 0, 0, 'Okay, let''s do this! You fight the ogres, and I will... not fight them. Head for the exit!', 12, 0, 100, 5, 0, 0, ''),
(39797, 1, 0, 'We made it! I''ll find my way back on my own. Do me a favor and go tell Royce Duskwhisper where he can stick his stupid Eye of Twilight.', 12, 0, 100, 4, 0, 0, ''),
(40107, 0, 0, 'Fool of a mortal. Flesh burns.', 12, 0, 100, 0, 0, 0, 'Thol''embaar'),
(40107, 1, 0, 'Others will come... the age of mortals... has passed.', 12, 0, 100, 0, 0, 0, 'Thol''embaar'),
(41114, 0, 0, 'Gotcha Chickenlips! You''d best get talky, or I''ll get stabby.', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 0, 'We don''t need to tell Omnuron about this.', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 1, 'Wait, was I supposed to be the good cop?', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 2, 'Easy, friend. We still need the info.', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 3, 'Hey, you don''t waste any time.', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 4, 'There''s more where that came from, Marion!', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 1, 5, 'Brutal!', 12, 0, 100, 0, 0, 0, 'Thisalee Crow'),
(41114, 2, 0, 'It''s your call, $n. Marion brought this on herself when she attacked our matron''s sacred shrine.', 12, 0, 100, 36, 0, 0, 'Thisalee Crow'),
(41114, 3, 0, 'You''re a better person than I, $n. But I suppose the harpies are just pawns here.', 12, 0, 100, 274, 0, 0, 'Thisalee Crow'),
(41114, 4, 0, 'Meet me back at Aviana''s Shrine, $n!', 12, 0, 100, 0, 0, 0, 'Thisalee Crow');

DELETE FROM `creature_loot_template` WHERE `entry` = 40107 AND `item` = 53454;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(40107, 53454, -100, 1, 0, 1, 1);
