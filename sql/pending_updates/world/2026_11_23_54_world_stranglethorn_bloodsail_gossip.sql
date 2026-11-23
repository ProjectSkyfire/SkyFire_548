-- Bloodsail Buccaneers and Booty Bay quests (Cape of Stranglethorn): the gossip that gives the credits and items of
-- eight quests had no script. The menus and options existed, the NPCs and barrels behind them did not react, and several
-- options had no quest conditions or were missing altogether:
--   26595 Kill-Collect: Juntz Fitztittle and Midriff Moonfall turn hostile when asked to pay up (and the second one had
--     no condition to show the option).
--   26602 A Dish Best Served Huge: the Ruined Lifeboat summoned Negolash only on the removed quest 619, with no condition
--     on the option and without the feast; it now does as the source: menu option while the quest is taken, Negolash and
--     the food on the beach, and Negolash's walk to it.
--   26630 Looks Like a Tauren Pirate to Me: Bossy (the head), Narkk (the hat, hat option shown first as in the sniffed
--     menu) and Fleet Master Seahorn, who takes both items, rewards the quest and gets kicked about by the Baron.
--   26633 Swabbing Duty: Duncan's option casts the credit spell 96202 (the only source of the Deck Stain credit).
--   26662 The Brashtide Crew, 26663 Sinking From Within and 26664 Making Mutiny: the crew members, the grog, gunpowder and
--     cannonball barrels and the Bloodsail Corsairs; the barrel menus (11750, 11751, 11753) had no options.
-- Rows, texts, options and conditions are as in Legends of Azeroth 5.4.8, with these changes: Narkk's hat option has
-- number 0 here (the sniffed menu order), so his rows and the condition use 0 instead of 1; the one guid in a script
-- (the Baron, 184095 in the source) is his guid here; the objective conditions of the source ("objective complete",
-- negated) are written as this core's "objective progress equal to 0" (the existing rows of the Brashtide Crew menus and
-- of menu 11735 used the negated form, which hid the options until the objective was already done, and are corrected);
-- the second row of Fleet Master Firallon (credit on aggro and teleport of the party) belongs to the boat fight and is
-- not part of this file. The new rows of Fitztittle, Midriff, the Corsair and the three Brashtide Crew members follow the
-- combat rows these creatures already had. The Corsair's faction row linked to its text-over row, which the core cannot
-- follow (it logs an error), so that link is dropped: the text-over row starts the attack by itself.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (2487, 2496, 2501, 2545, 2663, 43505, 43506) AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (204454, 204455, 204456) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2487 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 248700 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2496 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 249600 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43506 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4350600 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2501 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2545 AND `id` IN (0, 1, 2, 3, 4, 5, 6);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 2663 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43505 AND `id` IN (0, 1, 2, 3, 4, 5, 6);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43636 AND `id` IN (2, 3, 4, 5, 6);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43659 AND `id` IN (2, 3, 4, 5, 6, 7, 8, 9);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43660 AND `id` IN (2, 3, 4, 5, 6, 7, 8, 9);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43661 AND `id` IN (2, 3, 4, 5, 6, 7, 8, 9);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43376 AND `id` IN (3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43377 AND `id` IN (3, 4, 5, 6, 7, 8, 9, 10, 11);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204454 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204455 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204456 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 2289 AND `id` IN (0, 1, 2);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 228900 AND `id` IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 1494 AND `id` IN (0, 1, 2, 3, 4);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2487, 0, 0, 1, 62, 0, 100, 0, 11745, 0, 0, 0, 0, 33, 2487, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Gossip Selected - Quest Credit'),
(2487, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 0 - Close Gossip'),
(2487, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 19, 2496, 10, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 1 - Set Orientation'),
(2487, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 2 - Say Line 0'),
(2487, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 3 - Remove NpcFlag 3'),
(2487, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 4 - Set UnitFlag 33554432'),
(2487, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 248700, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - Linked To Id 5 - Run Script'),
(2487, 0, 7, 0, 20, 0, 100, 0, 26630, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Reward Quest - Say Line 2'),
(248700, 9, 0, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 12, 43506, 3, 13000, 0, 0, 0, 8, 0, 0, 0, -14444.1, 486.679, 28.8566, 0, 'Fleet Master Seahorn - On Script - Summon Creature Seahorn''s Head'),
(248700, 9, 1, 0, 0, 0, 100, 0, 1600, 1600, 0, 0, 0, 11, 81329, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Cast Quest - Kick Seahorn''s Head'),
(248700, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 19848, 2496, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Set Data 0 1'),
(248700, 9, 3, 0, 0, 0, 100, 0, 4600, 4600, 0, 0, 0, 11, 81329, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Cast Quest - Kick Seahorn''s Head'),
(248700, 9, 4, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 66, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Set Orientation'),
(248700, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Say Line 1'),
(248700, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 81, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Set NpcFlag 3'),
(248700, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 33554432, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fleet Master Seahorn - On Script - Remove UnitFlag 33554432'),
(2496, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 80, 249600, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baron Revilgaz - On Data Set 0 1 - Run Script'),
(249600, 9, 0, 0, 0, 0, 100, 0, 1600, 1600, 0, 0, 0, 11, 81329, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baron Revilgaz - On Script - Cast Quest - Kick Seahorn''s Head'),
(249600, 9, 1, 0, 0, 0, 100, 0, 4600, 4600, 0, 0, 0, 11, 81329, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Baron Revilgaz - On Script - Cast Quest - Kick Seahorn''s Head'),
(43506, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4350600, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Seahorn''s Head - On Summoned - Run Script'),
(4350600, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 97, 5, 5, 0, 0, 0, 0, 1, 0, 0, 0, -14439.8, 484.031, 28.829, 0, 'Seahorn''s Head - On Script - Jump To Pos'),
(4350600, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 97, 5, 5, 0, 0, 0, 0, 1, 0, 0, 0, -14444.1, 486.679, 28.8566, 0, 'Seahorn''s Head - On Script - Jump To Pos'),
(4350600, 9, 2, 0, 0, 0, 100, 0, 2600, 2600, 0, 0, 0, 97, 5, 5, 0, 0, 0, 0, 1, 0, 0, 0, -14439.8, 484.031, 28.829, 0, 'Seahorn''s Head - On Script - Jump To Pos'),
(4350600, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 97, 5, 5, 0, 0, 0, 0, 1, 0, 0, 0, -14444.1, 486.679, 28.8566, 0, 'Seahorn''s Head - On Script - Jump To Pos'),
(2501, 0, 0, 1, 20, 0, 100, 0, 26595, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Sea Wolf" MacKinley - On Reward Quest - Store Target List'),
(2501, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Sea Wolf" MacKinley - Linked To Id 0 - Say Line 0'),
(2501, 0, 2, 0, 52, 0, 100, 0, 0, 2501, 0, 0, 0, 1, 1, 3000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Sea Wolf" MacKinley - On Text Over - Say Line 1'),
(2501, 0, 3, 0, 52, 0, 100, 0, 1, 2501, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, '"Sea Wolf" MacKinley - On Text Over - Say Line 2'),
(2545, 0, 0, 1, 62, 0, 100, 0, 12723, 0, 0, 0, 0, 86, 96202, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Gossip Select - Cast Swabbing Duty: Credit (Cheating)'),
(2545, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Gossip Select - Close Gossip'),
(2545, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Gossip Select - Say Line 0'),
(2545, 0, 3, 0, 52, 0, 100, 0, 0, 2545, 0, 0, 0, 1, 1, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Text Over - Say Line 1'),
(2545, 0, 4, 0, 52, 0, 100, 0, 1, 2545, 0, 0, 0, 1, 2, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Text Over - Say Line 2'),
(2545, 0, 5, 0, 52, 0, 100, 0, 2, 2545, 0, 0, 0, 1, 3, 10000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Text Over - Say Line 3'),
(2545, 0, 6, 0, 52, 0, 100, 0, 3, 2545, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, '"Pretty Boy" Duncan - On Text Over - Say Line 4'),
(2663, 0, 0, 1, 62, 0, 100, 0, 11743, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Narkk - On Gossip Selected - Store Target List'),
(2663, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Narkk - Linked To Id 0 - Close Gossip'),
(2663, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 56, 59148, 1, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Narkk - Linked To Id 1 - Add Item Oversized Pirate Hat'),
(2663, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Narkk - Linked To Id 2 - Say Line 0'),
(43505, 0, 0, 1, 62, 0, 100, 0, 11742, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bossy - On Gossip Selected - Store Target List'),
(43505, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Bossy - Linked To Id 0 - Close Gossip'),
(43505, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 56, 59147, 1, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Bossy - Linked To Id 1 - Add Item Cow Head'),
(43505, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 81313, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bossy - Linked To Id 2 - Cast Looks Like a Tauren Pirate to Me: Bossy Death'),
(43505, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bossy - Linked To Id 3 - Despawn after 2 sec'),
(43505, 0, 5, 6, 62, 0, 100, 0, 11742, 1, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bossy - On Gossip Selected - Close Gossip'),
(43505, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bossy - Linked To Id 5 - Say Line 0'),
(43636, 0, 2, 3, 62, 0, 100, 0, 11755, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodsail Corsair - On Gossip Select - Store Target List'),
(43636, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodsail Corsair - Linked - Close Gossip'),
(43636, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 1000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodsail Corsair - Linked - Say Line Random'),
(43636, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodsail Corsair -Linked - Set Faction 14'),
(43636, 0, 6, 0, 52, 0, 100, 0, 0, 43636, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Bloodsail Corsair - On Text Over 0 - Attack Start'),
(43659, 0, 2, 3, 62, 0, 100, 0, 11759, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - On Gossip Select - Store Target List'),
(43659, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 43666, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - Linked - Quest Credit'),
(43659, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - Linked - Close Gossip'),
(43659, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Long John Copper - Linked - Say Line 0'),
(43659, 0, 6, 7, 52, 0, 100, 0, 0, 43659, 0, 0, 0, 36, 43663, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - On Text Over 0 - Update Template On Long John Copper (Worgen Form)'),
(43659, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 43659, 100, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - Linked - Create Timed Event'),
(43659, 0, 8, 9, 59, 0, 100, 0, 43659, 0, 0, 0, 0, 5, 15, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - On Timed Event Triggered - Play Emote 15'),
(43659, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Long John Copper - Linked - Despawn after 30 sec'),
(43660, 0, 2, 3, 62, 0, 100, 0, 11760, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - On Gossip Select - Store Target List'),
(43660, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 43667, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - Linked - Quest Credit'),
(43660, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - Linked - Close Gossip'),
(43660, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - Linked - Say Line 0'),
(43660, 0, 6, 7, 52, 0, 100, 0, 0, 43660, 0, 0, 0, 36, 43664, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - On Text Over 0 - Update Template On Enormous Shawn Stooker (Worgen Form)'),
(43660, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 43660, 100, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - Linked - Create Timed Event'),
(43660, 0, 8, 9, 59, 0, 100, 0, 43660, 0, 0, 0, 0, 5, 15, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - On Timed Event Triggered - Play Emote 15'),
(43660, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enormous Shawn Stooker - Linked - Despawn after 30 sec'),
(43661, 0, 2, 3, 62, 0, 100, 0, 11761, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - On Gossip Select - Store Target List'),
(43661, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 43668, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - Linked - Quest Credit'),
(43661, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - Linked - Close Gossip'),
(43661, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 2000, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - Linked - Say Line 0'),
(43661, 0, 6, 7, 52, 0, 100, 0, 0, 43661, 0, 0, 0, 36, 43665, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - On Text Over 0 - Update Template On Wailing Mary Smitts (Worgen Form)'),
(43661, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 43661, 100, 1000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - Linked - Create Timed Event'),
(43661, 0, 8, 9, 59, 0, 100, 0, 43661, 0, 0, 0, 0, 5, 15, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - On Timed Event Triggered - Play Emote 15'),
(43661, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wailing Mary Smitts - Linked - Despawn after 30 sec'),
(43376, 0, 3, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 17, 173, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - On Respawn - Set Emote State'),
(43376, 0, 4, 5, 62, 0, 100, 0, 11735, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - On Gossip Select - Store Target List'),
(43376, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Close Gossip'),
(43376, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Set Faction 72'),
(43376, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Remove NpcFlag 1'),
(43376, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Say Line 0'),
(43376, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Set React Aggressive'),
(43376, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Attack Start'),
(43376, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Set Emote State'),
(43376, 0, 12, 13, 7, 0, 100, 0, 0, 0, 0, 0, 0, 17, 173, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - On Evade - Set Emote State'),
(43376, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 2159, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Set Faction 2159'),
(43376, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle - Linked - Set NpcFlag 1'),
(43377, 0, 3, 4, 62, 0, 100, 0, 11736, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - On Gossip Select - Store Target List'),
(43377, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Close Gossip'),
(43377, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 72, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Set Faction 72'),
(43377, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Remove NpcFlag 1'),
(43377, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Say Line 0'),
(43377, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Set React Aggressive'),
(43377, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Attack Start'),
(43377, 0, 10, 11, 7, 0, 100, 0, 0, 0, 0, 0, 0, 2, 80, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Out Of Combat - Set Faction 80'),
(43377, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 81, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ephram "Midriff" Moonfall - Linked - Set NpcFlag 1'),
(204454, 1, 0, 0, 62, 0, 100, 0, 11750, 0, 0, 0, 0, 33, 43623, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grog Barrel - On Gossip Select - Quest Credit'),
(204455, 1, 0, 0, 62, 0, 100, 0, 11751, 0, 0, 0, 0, 33, 43631, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gunpowder Barrel - On Gossip Select - Quest Credit'),
(204456, 1, 0, 0, 62, 0, 100, 0, 11753, 0, 0, 0, 0, 33, 43632, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cannonball Crate - On Gossip Select - Quest Credit'),
(2289, 1, 0, 1, 62, 0, 100, 0, 11728, 0, 0, 0, 0, 12, 1494, 6, 30000, 0, 0, 0, 8, 0, 0, 0, -14612.5, 105.994, -8.61236, 0.98471, 'Ruined Lifeboat - On Gossip Select - Summon Creature Negolash'),
(2289, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ruined Lifeboat - Linked To Id 0 - Close Gossip'),
(2289, 1, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 228900, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ruined Lifeboat - Linked To Id 1 - Run Script'),
(228900, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2333, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14656.8, 147.434, 3.10207, 3.01941, 'Ruined Lifeboat - On Script - Summon GO Stranglevine Wine'),
(228900, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2333, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14655.7, 148.978, 4.0564, 3.47321, 'Ruined Lifeboat - On Script - Summon GO Stranglevine Wine'),
(228900, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2333, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14656.5, 147.595, 3.12908, 1.58825, 'Ruined Lifeboat - On Script - Summon GO Stranglevine Wine'),
(228900, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2333, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14653, 145.389, 2.852, 4.85202, 'Ruined Lifeboat - On Script - Summon GO Stranglevine Wine'),
(228900, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2333, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14657.2, 148.228, 2.88632, 1.53589, 'Ruined Lifeboat - On Script - Summon GO Stranglevine Wine'),
(228900, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2562, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14653.8, 146.204, 2.14631, 4.88692, 'Ruined Lifeboat - On Script - Summon GO Baked Bread'),
(228900, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2562, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14652.4, 145.753, 3.25464, 2.93214, 'Ruined Lifeboat - On Script - Summon GO Baked Bread'),
(228900, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2562, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14656.1, 148.367, 3.51564, 5.63741, 'Ruined Lifeboat - On Script - Summon GO Baked Bread'),
(228900, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14656.2, 147.096, 2.38777, 0.174532, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14654.5, 147.324, 2.45636, 1.81514, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 11, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14653, 146.524, 2.35559, 6.00393, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 12, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14654, 147.306, 2.46731, 0.942477, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 13, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14654.9, 147.278, 2.42524, 5.88176, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 14, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14653, 145.138, 2.65327, 5.84685, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 15, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14655.1, 146.671, 2.23095, 2.65289, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 16, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14656.8, 148.893, 3.28866, 5.09636, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 17, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14655.3, 147.802, 2.63972, 6.16101, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 18, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14652.9, 146.867, 2.50691, 2.94959, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 19, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14654.7, 146.142, 2.08906, 2.37364, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(228900, 9, 20, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 50, 2332, 60, 0, 0, 0, 0, 8, 0, 0, 0, -14652.4, 146.512, 3.50118, 0.349065, 'Ruined Lifeboat - On Script - Summon GO Barbequed Buzzard Wings'),
(1494, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 19, 32768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Negolash - On Respawn - Remove UnitFlag 32768'),
(1494, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 67, 1494, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Negolash - Linked To Id 0 - Create Timed Event'),
(1494, 0, 2, 3, 59, 0, 100, 0, 1494, 0, 0, 0, 0, 18, 16384, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Negolash - On Timed Event Triggered - Set UnitFlag 16384'),
(1494, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Negolash - Linked To Id 2 - Say Line 0'),
(1494, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, -14650.6, 141.57, 0.309, 0, 'Negolash - Linked To Id 4 - Move To Pos');

DELETE FROM `creature_text` WHERE `entry` IN (2487, 2501, 2545, 2546, 2663, 43376, 43377, 43505, 43636, 43659, 43660, 43661);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(2487, 0, 0, 'Hey Baron, go long!', 12, 0, 100, 0, 0, 0, 'Fleet Master Seahorn'),
(2487, 1, 0, 'That''ll do.', 12, 0, 100, 0, 0, 0, 'Fleet Master Seahorn'),
(2487, 2, 0, 'Bring that back up to Yancey, by the waterfall.  The ignorant human''ll have no idea that it''s not me.', 12, 0, 100, 0, 0, 0, 'Fleet Master Seahorn'),
(2501, 0, 0, 'Alright, let''s try cracking this thing open!', 12, 0, 100, 0, 0, 0, '"Sea Wolf" MacKinley'),
(2501, 1, 0, '%s tries opening Maury''s Clubbed Foot...', 16, 0, 100, 0, 0, 0, '"Sea Wolf" MacKinley'),
(2501, 2, 0, 'Aha!  It opened!  I expected that to be harder.  Thanks for your help, $n!', 12, 0, 100, 0, 0, 0, '"Sea Wolf" MacKinley'),
(2545, 0, 0, 'This is humiliating.', 12, 0, 100, 18, 0, 0, ''),
(2545, 1, 0, 'I can''t believe it''s come to this.', 12, 0, 100, 38, 0, 0, ''),
(2545, 2, 0, 'A mop?  Really?', 12, 0, 100, 38, 0, 0, ''),
(2545, 3, 0, 'I suppose sometimes y''need to hit rock bottom before y''can crawl yer way back up.', 12, 0, 100, 38, 0, 0, ''),
(2545, 4, 0, 'Maybe I need a new nickname.', 12, 0, 100, 6, 0, 0, ''),
(2546, 0, 0, 'Three cheers for $n -- Admiral o'' the Bloodsail Bucaneer Navy!', 14, 0, 100, 0, 0, 0, 'Fleet Master Firallon'),
(2546, 1, 0, 'Hip hip... arrrrr...', 14, 0, 100, 0, 0, 0, 'Fleet Master Firallon'),
(2663, 0, 0, 'Tell him this is the last one.  It breaks poor Narkk''s heart to have to carve horn-holes in his beautiful hats!', 12, 0, 100, 18, 0, 0, 'Narkk'),
(43376, 0, 0, 'How did you find me!?', 14, 0, 100, 0, 0, 0, 'Freewheelin'' Juntz Fitztittle'),
(43377, 0, 0, 'I haven''t got any money!  Get away!', 14, 0, 100, 0, 0, 0, 'Ephram "Midriff" Moonfall'),
(43505, 0, 0, '%s breathes a heavy sigh of relief.', 16, 0, 100, 0, 0, 0, 'Bossy'),
(43636, 0, 0, 'Draw yer own steel, ye harpy-lovin'' scoundrel!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 1, 'I''d never leave the Buccaneers, ye whale-tail skivvy-sniffer!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 2, 'I''d never turn sides, ye green-gilled murloc!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 3, 'I''ll poke out yer deadlights, ye lily-livered eel!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 4, 'I''ll run ye through, ye cannon-kissin'' cannonface!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 5, 'I''ll send ye down to Neptulon''s Locker, ye three-gallon pufferfish!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 6, 'I''ll send ye to feed the fishes, ye flat-shanty trogg!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43636, 0, 7, 'Mutiny?  I''ll show ye mutiny, ye cockle-knuckled squiffy!', 14, 0, 100, 0, 0, 0, 'Bloodsail Corsair'),
(43659, 0, 0, 'Yes, $g sir: ma''am;!  We''ve been biding our time in Gilneas for far too long.  We''re ready for blood!', 12, 0, 100, 0, 0, 0, 'Long John Copper'),
(43660, 0, 0, 'We used to roam the seas... ''til that damned King Greymane shut down the harbors in Gilneas!  If it weren''t for him, we wouldn''t be...', 12, 0, 100, 0, 0, 0, 'Enormous Shawn Stooker'),
(43661, 0, 0, 'Yes, $g sir: ma''am;.  We attack from the north at your signal.  The Brashtide will wash over Booty Bay!  Aroooo!!!!', 12, 0, 100, 0, 0, 0, 'Wailing Mary Smitts');

DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (11741, 11750, 11751, 11753);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11741, 0, 0, 'Bossy, I need to ask something great of you.  I need to cut off your head to prove my loyalty to the Bloodsail Buccaneers, so they will allow me to infiltrate their ranks and hopefully discover their true intentions.\nBooty Bay needs you, Bossy... now more than ever.\n', 43570, 1, 1, 11742, 0, 0, 0, NULL, 0, 0),
(11741, 1, 0, 'Moo.', 43571, 1, 1, 11742, 0, 0, 0, NULL, 0, 0),
(11750, 0, 0, '<Nudge the cork.>', 43686, 1, 1, 11749, 0, 0, 0, NULL, 0, 0),
(11751, 0, 0, '<Pour the water onto the gunpowder.>', 43690, 1, 1, 11752, 0, 0, 0, NULL, 0, 0),
(11753, 0, 0, '<Grease the cannonballs.>', 43694, 1, 1, 11754, 0, 0, 0, NULL, 0, 0);


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceEntry` = 0 AND `SourceGroup` IN (11727, 11728, 11735, 11736, 11743, 11744, 11750, 11751, 11753, 11759, 11760, 11761) AND `ConditionTypeOrReference` IN (2, 9, 48);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11727, 0, 0, 0, 9, 0, 26600, 0, 0, 0, 0, 0, '', 'Whiskey Slim - drinks only with A Giant''s Feast'),
(15, 11728, 0, 0, 0, 9, 0, 26602, 0, 0, 0, 0, 0, '', 'Ruined Lifeboat - food only with A Dish Best Served Huge'),
(15, 11735, 0, 0, 0, 9, 0, 26595, 0, 0, 0, 0, 0, '', 'Juntz Fitztittle - only with Kill-Collect'),
(15, 11735, 0, 0, 0, 48, 0, 267627, 0, 0, 0, 0, 0, '', 'Juntz Fitztittle - only while the wrench objective is open'),
(15, 11736, 0, 0, 0, 9, 0, 26595, 0, 0, 0, 0, 0, '', 'Midriff Moonfall - only with Kill-Collect'),
(15, 11736, 0, 0, 0, 48, 0, 267628, 0, 0, 0, 0, 0, '', 'Midriff Moonfall - only while the mirror objective is open'),
(15, 11743, 0, 0, 0, 9, 0, 26630, 0, 0, 0, 0, 0, '', 'Narkk - hat only with Looks Like a Tauren Pirate to Me'),
(15, 11743, 0, 0, 0, 2, 0, 59148, 1, 0, 1, 0, 0, '', 'Narkk - hat only without a hat'),
(15, 11744, 0, 0, 0, 9, 0, 26630, 0, 0, 0, 0, 0, '', 'Fleet Master Seahorn - only with Looks Like a Tauren Pirate to Me'),
(15, 11744, 0, 0, 0, 2, 0, 59147, 1, 0, 0, 0, 0, '', 'Fleet Master Seahorn - only with the cow head'),
(15, 11744, 0, 0, 0, 2, 0, 59148, 1, 0, 0, 0, 0, '', 'Fleet Master Seahorn - only with the hat'),
(15, 11750, 0, 0, 0, 9, 0, 26663, 0, 0, 0, 0, 0, '', 'Sinking From Within - grog only with the quest'),
(15, 11750, 0, 0, 0, 48, 0, 267452, 0, 0, 0, 0, 0, '', 'Sinking From Within - grog only while the objective is open'),
(15, 11751, 0, 0, 0, 9, 0, 26663, 0, 0, 0, 0, 0, '', 'Sinking From Within - gunpowder only with the quest'),
(15, 11751, 0, 0, 0, 48, 0, 267453, 0, 0, 0, 0, 0, '', 'Sinking From Within - gunpowder only while the objective is open'),
(15, 11753, 0, 0, 0, 9, 0, 26663, 0, 0, 0, 0, 0, '', 'Sinking From Within - cannonballs only with the quest'),
(15, 11753, 0, 0, 0, 48, 0, 267454, 0, 0, 0, 0, 0, '', 'Sinking From Within - cannonballs only while the objective is open'),
(15, 11759, 0, 0, 0, 9, 0, 26662, 0, 0, 0, 0, 0, '', 'Long John Copper - only with The Brashtide Crew'),
(15, 11759, 0, 0, 0, 48, 0, 258271, 0, 0, 0, 0, 0, '', 'Long John Copper - only while the objective is open'),
(15, 11760, 0, 0, 0, 9, 0, 26662, 0, 0, 0, 0, 0, '', 'Enormous Shawn Stooker - only with The Brashtide Crew'),
(15, 11760, 0, 0, 0, 48, 0, 258272, 0, 0, 0, 0, 0, '', 'Enormous Shawn Stooker - only while the objective is open'),
(15, 11761, 0, 0, 0, 9, 0, 26662, 0, 0, 0, 0, 0, '', 'Wailing Mary Smitts - only with The Brashtide Crew'),
(15, 11761, 0, 0, 0, 48, 0, 258273, 0, 0, 0, 0, 0, '', 'Wailing Mary Smitts - only while the objective is open');
