-- Gameobject QuestStarters
DELETE FROM `gameobject_queststarter` WHERE `id` IN (212181, 216322, 211754, 218765, 215844, 214062, 218072, 213767, 209673, 213973, 221413, 209845, 221376, 212389, 214562, 214871, 214903, 215705, 218750);
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES 
(212181, 29743), -- GO: Ancient Statue Q: Monstrosity
(216322, 32222), -- GO: Bounty Board Q: Wanted: Chief Engineer Cogwrench
(216322, 32223), -- GO: Bounty Board Q: Wanted: Lieutenant Ethan Jacobson
(211754, 30797), -- GO: Curious Text Q: It Was Almost Alive
(218765, 32643), -- GO: Empty Crate Q: Work Order: Dominance Offensive II
(218765, 32646), -- GO: Empty Crate Q: Work Order: Operation: Shieldwall II
(218765, 32648), -- GO: Empty Crate Q: Work Order: Golden Lotus II
(218765, 32650), -- GO: Empty Crate Q: Work Order: Shado-Pan II
(218765, 32657), -- GO: Empty Crate Q: Work Order: The August Celestials II
(218765, 32659), -- GO: Empty Crate Q: Work Order: The Klaxxi II
(218765, 32943), -- GO: Empty Crate Q: Work Order: Sunreaver Onslaught II
(218765, 32945), -- GO: Empty Crate Q: Work Order: Kirin Tor Offensive II
(215844, 31999), -- GO: Flagpole Q: Nazgrim's Command
(214062, 31398), -- GO: Glowing Amber Q: Falling to Pieces
(218072, 32505), -- GO: Head of the Chamberlain Q: The Crumbled Chamberlain
(213767, 31315), -- GO: Hidden Treasure Q: The Heartland Legacy
(209673, 29782), -- GO: Jade Tiger Pillar Q: Stronger Than Bone
(213973, 31010), -- GO: Klaxxi Sonic Relay Q: In Her Clutch
(221413, 33107), -- GO: Lin Family Scroll Q: Bad Feeling, Worse Result
(209845, 29889), -- GO: Mouthwatering Brew Q: Borrowed Brew
(221376, 33018), -- GO: Old Sign Fragment Q: Noodle Secrets Long Forgotten
(212389, 31068), -- GO: Scroll of Auspice Q: Sacred Recipe
(214562, 31661), -- GO: Sha-Haunted Crystal Q: A Source of Terrifying Power
(214871, 29747), -- GO: Shattered Destroyer Q: Break the Cycle
(214871, 29748), -- GO: Shattered Destroyer Q: Simulacrumble
(214903, 29755), -- GO: The Tan-Chao Q: Pei-Back
(215705, 30534), -- GO: Tillers Shrine Q: A Second Hand
(218750, 32642), -- GO: Work Orders Q: Work Order: Dominance Offensive I
(218750, 32647), -- GO: Work Orders Q: Work Order: Golden Lotus I
(218750, 32944), -- GO: Work Orders Q: Work Order: Kirin Tor Offensive I
(218750, 32645), -- GO: Work Orders Q: Work Order: Operation: Shieldwall I
(218750, 32649), -- GO: Work Orders Q: Work Order: Shado-Pan I
(218750, 32942), -- GO: Work Orders Q: Work Order: Sunreaver Onslaught I
(218750, 32653), -- GO: Work Orders Q: Work Order: The August Celestials I
(218750, 32658); -- GO: Work Orders Q: Work Order: The Klaxxi I-- Quest Special Flags Errors
-- Add SpecialFlags Repeatable to Daily Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|4096 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Repeatable Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|8192 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Weekly Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|32768 AND SpecialFlags=0;
-- Update quest_template with SourceItems and without SourceItemCount /* Affected rows: 388 */
UPDATE quest_template SET SourceItemCount=1 WHERE SourceItemId>0 AND SourceItemCount=0;
-- Update quest_template with method 1 for quests where minlevel>90; -- most obsolete quests /* Affected rows: 74 */
UPDATE quest_template SET Method=1 WHERE MinLevel>90;-- Lupello SAI
SET @ENTRY := 56357;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,9,0,100,0,5,40,1700,3800,11,129496,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - Within 5-40 Range - Cast 'Prey Pounce'"),
(@ENTRY,0,1,0,0,0,100,0,10000,10500,10500,15500,11,129502,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - In Combat - Cast 'Fearsome Howl'"),
(@ENTRY,0,2,0,0,0,100,0,5000,10000,10000,15000,11,129497,0,0,0,0,0,2,0,0,0,0,0,0,0,"Lupello - In Combat - Cast 'Pounced'");

-- Lupello Spawn
SET @CGUID := 2572;
DELETE FROM `creature` WHERE `guid`=@CGUID;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(@CGUID, @ENTRY, 870, 1, 1, 79.69495, -548.7769, 204.871, 2.686525, 120, 0, 0);