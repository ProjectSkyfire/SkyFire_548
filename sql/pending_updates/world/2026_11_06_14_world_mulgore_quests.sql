-- Mulgore quest fixes.
-- Sources: 5.4.8 data for everything below. The 4.3.4 data has the quest "War Dance" (24540) asking to kill Orno
-- Grimtotem but no spawn and no script for him; the 5.4.8 data spawns him and scripts the scene. The fishing loot rows
-- are 5.4.8 (4.3.4 has no source for these quest fish). Where 5.4.8 SmartAI uses action 203 (combat stop) it is
-- written as 27 in this core. No event flag 0x200 and no cast flags are used.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.

-- "War Dance" (24540): the objective is Orno Grimtotem (36931), who is not spawned in any form, so the last quest of
-- the Bloodhoof rite chain cannot be finished. The 5.4.8 spawn stands at the Grimtotem hill. When he is fought down
-- he stops, the player gets the credit, he calls the ambushers, the High Chieftain (37058) walks in from the waypoint
-- and kills him. Orno, Baine and the ambusher already exist as templates and only lacked the AI name.
DELETE FROM `creature` WHERE `guid` = 8332000;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8332000, 36931, 1, 1, 0, 0, 0, 0, -600.932, 189.823, 53.1532, 3.22886, 60, 0, 0, 0, 0, 0, 0, 0, 0); -- Orno Grimtotem

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36931, 37058, 37062);

DELETE FROM `waypoints` WHERE `entry` = 37058;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(37058, 1, -603.096, 193.539, 53.082, 'Baine Bloodhoof');

DELETE FROM `creature_text` WHERE `entry` IN (36931, 37058);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36931, 0, 0, 'I''ve had enough of this!', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 1, 0, 'Baine! You''ve shown yourself! GET HIM!', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 2, 0, 'You will die in shame like your pathetic father.', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(36931, 3, 0, 'This isn''t over, Baine...', 12, 0, 100, 0, 0, 0, 'Orno Grimtotem'),
(37058, 0, 0, 'So have I, Grimtotem.', 12, 0, 100, 0, 0, 21166, 'Baine Bloodhoof'),
(37058, 1, 0, 'It took a coward''s poison and the fury of Hellscream to bring my father down.', 12, 0, 100, 0, 0, 21161, 'Baine Bloodhoof'),
(37058, 2, 0, 'WHAT CHANCE HAVE YOU?', 12, 0, 100, 0, 0, 21162, 'Baine Bloodhoof'),
(37058, 3, 0, 'It is for you.', 12, 0, 100, 0, 0, 21163, 'Baine Bloodhoof'),
(37058, 4, 0, 'We''re done here. Mulgore will no longer suffer the hatred of the Grimtotem.', 12, 0, 100, 0, 0, 0, 'Baine Bloodhoof'),
(37058, 5, 0, 'I will meet you again in Thunder Bluff.', 12, 0, 100, 0, 0, 0, 'Baine Bloodhoof');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36931, 37058, 37062);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (3693100, 3705800);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36931, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Reset - Remove UnitFlag 768'),
(36931, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 0 - Set Emote State 0'),
(36931, 0, 2, 3, 4, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Aggro - Store Target List'),
(36931, 0, 3, 0, 61, 0, 100, 1, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 2 - Set Invincibility Hp 1'),
(36931, 0, 4, 5, 2, 0, 100, 1, 0, 20, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Between 0-20% Health - Combat Stop'),
(36931, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 102, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 4 - Set Health Regen 0'),
(36931, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36931, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 5 - Quest Credit'),
(36931, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 6 - Set UnitFlag 768'),
(36931, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 7 - Say Line 0'),
(36931, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -600.9320068359375, 189.822998046875, 53.1531982421875, 0, 'Orno Grimtotem - Move To Pos'),
(36931, 0, 10, 11, 52, 0, 100, 0, 0, 36931, 0, 0, 0, 12, 37058, 8, 0, 0, 0, 0, 8, 0, 0, 0, -626.3090209960938, 207.38400268554688, 53.10940170288086, 0, 'Orno Grimtotem - On Text Over 0 - Summon Creature Baine Bloodhoof'),
(36931, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 10 - Set Orientation'),
(36931, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 375, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 11 - Set Emote State 375'),
(36931, 0, 13, 14, 38, 0, 100, 0, 0, 1, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 1 - Set Orientation'),
(36931, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - Linked To Id 13 - Say Line 1'),
(36931, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 37062, 6, 10000, 0, 0, 0, 8, 0, 0, 0, -602.0659790039062, 197.13099670410156, 53.13399887084961, 0, 'Orno Grimtotem - Linked To Id 14 - Summon Creature Grimtotem Ambushe'),
(36931, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 37062, 6, 10000, 0, 0, 0, 8, 0, 0, 0, -606.7930297851562, 196.00399780273438, 53.13399887084961, 0, 'Orno Grimtotem - Linked To Id 15 - Summon Creature Grimtotem Ambushe'),
(36931, 0, 17, 0, 52, 0, 100, 0, 1, 36931, 0, 0, 0, 1, 2, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Text Over 1 - Say Line 2'),
(36931, 0, 18, 0, 52, 0, 100, 0, 2, 36931, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Text Over 2 - Set Data 0 1'),
(36931, 0, 19, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 80, 3693100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 2 - Run Script'),
(36931, 0, 20, 0, 38, 0, 100, 0, 0, 3, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Data Set 0 3 - Despawn'),
(37058, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 75, 69756, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Summoned - Add Aura War Harness'),
(37058, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 37058, 0, 24540, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 0 - Waypoint Start'),
(37058, 0, 2, 3, 58, 0, 100, 0, 1, 37058, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Waypoint Ended - Say Line 0'),
(37058, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 2 - Set Home'),
(37058, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 3 - Set Orientation'),
(37058, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 375, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 4 - Set Emote State 375'),
(37058, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 5 - Remove UnitFlag 512'),
(37058, 0, 7, 0, 52, 0, 100, 0, 0, 37058, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 0 - Set Data 0 1'),
(37058, 0, 8, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 1, 1, 6000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Data Set 0 1 - Say Line 1'),
(37058, 0, 9, 10, 52, 0, 100, 0, 1, 37058, 0, 0, 0, 1, 2, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 1 - Say Line 2'),
(37058, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 69847, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 9 - Cast War Stomp'),
(37058, 0, 11, 12, 52, 0, 100, 0, 2, 37058, 0, 0, 0, 11, 69879, 0, 0, 0, 0, 0, 11, 36931, 60, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 2 - Cast Charge'),
(37058, 0, 12, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 36931, 100, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 11 - Set Data 0 2'),
(37058, 0, 13, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 1, 3, 1000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Data 0 2 - Say Line 3'),
(37058, 0, 14, 15, 52, 0, 100, 0, 3, 37058, 0, 0, 0, 5, 36, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Text Over 3 - Play Emote 36'),
(37058, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 14 - Kill Unit'),
(37058, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3705800, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - Linked To Id 15 - Run Script'),
(37062, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - On Summoned - Set React State Defensive'),
(37062, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - Linked To Id 0 - Set Orientation'),
(37062, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - Linked To Id 1 - Attack Start'),
(37062, 0, 3, 0, 0, 0, 100, 0, 0, 0, 2000, 2000, 0, 11, 69873, 0, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Grimtotem Ambushe - In Combat - Cast Cosmetic - Combat Attack 1H');

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3693100, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Set Emote State 0'),
(3693100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Say Line 3'),
(3693100, 9, 2, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 45, 0, 2, 0, 0, 0, 0, 11, 37058, 10, 0, 0, 0, 0, 0, 'Orno Grimtotem - On Script - Set Data 0 2'),
(3705800, 9, 0, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Say Line 4'),
(3705800, 9, 1, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Say Line 5'),
(3705800, 9, 2, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 45, 0, 3, 0, 0, 0, 0, 11, 36931, 10, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Set Data 0 3'),
(3705800, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baine Bloodhoof - On Script - Despawn');

-- The Thunder Bluff and Mulgore fishing quests ("Pond Predators" 29345, "The Race to Restock" 29348) ask for fish that
-- nothing drops: Azshara Snakehead, Randy Smallfish and Amorous Mud Snapper have no source in the world database. The
-- 5.4.8 fishing loot has them as independent rows next to the normal fish: the Snakehead in the Thunder Bluff zone
-- (1638), the other two in the Mulgore zone (215). "Craving Crayfish" (29349) works through the Stonebull Crayfish
-- spell click and is left as it is.
-- Quest-only (negative chance): the fish are only for the two dailies. Orno stops at 20 percent (the 5.4.8 row said 80,
-- against its own comment) and walks to the hill before Baine comes (the 5.4.8 move row was linked from nowhere).
DELETE FROM `fishing_loot_template` WHERE (`entry` = 215 AND `item` IN (69964, 69967)) OR (`entry` = 1638 AND `item` = 69934);
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(215, 69964, -3.3522, 1, 0, 1, 1),
(215, 69967, -3.3622, 1, 0, 1, 1),
(1638, 69934, -30.1476, 1, 0, 1, 1);
