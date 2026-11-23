-- A Lashtail Hatchling (26317, 26738), I Think She's Hungry (26321, 26739), Favored Skull (26323, 26745) and A Nose for
-- This Sort of Thing (26325, 26746): the Lashtail Raptor Egg Fragment (item 58165, spell 79648) summons a Lashtail
-- Hatchling, who follows the player, eats the murlocs the player kills and digs up the fetishes the quests ask for. The
-- hatchling and the helper creatures had no AI at all, so the hatchling only stood there: "Bites of Murkgill meat" and
-- "Bites of basilisk meat" never filled and the Zuuldaian and Tkashi Fetishes could not be dug up.
-- How it works (rows as in Legends of Azeroth 5.4.8): a Murkgill Oracle or Warrior that dies summons a Generic Trigger
-- LAB for ten seconds, the trigger tells the hatchling within 35 yards, the hatchling charges it and casts the credit
-- spell 79661 (kill credit 42742 for its summoner) three times. A hatchling that comes within 10 yards of a Zuuldaian
-- Fetish (Horde, spell 79714) or a Tkashi Fetish (Alliance, spell 82324) digs there, tells the fetish to despawn and
-- creates the fetish item for its summoner. The first row of the hatchling sets the flags 768 (immune), which the source
-- names "Set NpcFlag". The Murkgill Warrior keeps its existing flee row, the new rows follow it.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (4459, 40350, 42736, 42772, 43977) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 4459 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 4461 AND `id` IN (1, 2);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 40350 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 42736 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 42772 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43977 AND `id` IN (0);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4273600 AND `id` IN (0, 1, 2, 3);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4273601 AND `id` IN (0, 1, 2, 3, 4, 5, 7, 8, 9);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4273602 AND `id` IN (0, 1, 2, 3, 4, 5, 7, 8, 9);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(4459, 0, 0, 1, 6, 0, 100, 0, 0, 0, 0, 0, 0, 12, 40350, 3, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkgill Oracle - On Death - Summon Creature Generic Trigger Lab'),
(4459, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 7000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkgill Oracle- Linked To Id 0 - Despawn after 7 sec'),
(4461, 0, 1, 2, 6, 0, 100, 0, 0, 0, 0, 0, 0, 12, 40350, 3, 10000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkgill Warrior - On Death - Summon Creature Generic Trigger Lab'),
(4461, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 7000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Murkgill Warrior - Linked To Id 0 - Despawn after 7 sec'),
(40350, 0, 0, 0, 54, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 42736, 35, 0, 0, 0, 0, 0, 'Generic Trigger Lab - On Summoned - Set Data 0 1'),
(42736, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Summoned - Store Target List'),
(42736, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 0-1 - Set Immune Flags'),
(42736, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 1 - Set Run 1'),
(42736, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 2 - Follow Target'),
(42736, 0, 4, 5, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 79660, 0, 0, 0, 0, 0, 19, 40350, 35, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Data Set 0 1 - Cast A Lashtail Hatchling: Charge Murloc'),
(42736, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4273600, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 4 - Run Script'),
(42736, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 19, 40350, 35, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 5 - Follow Target'),
(42736, 0, 7, 8, 75, 0, 100, 1, 0, 43977, 10, 2000, 0, 29, 0, 0, 0, 0, 0, 0, 19, 43977, 10, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Distance Creature - Follow Target'),
(42736, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4273601, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 7 - Run Script'),
(42736, 0, 9, 10, 75, 0, 100, 1, 0, 42772, 10, 2000, 0, 29, 0, 0, 0, 0, 0, 0, 19, 42772, 10, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Distance Creature - Follow Target'),
(42736, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4273602, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - Linked To Id 9 - Run Script'),
(42772, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Zuuldaian Fetish - On Data Set 0 1 - Despawn'),
(43977, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tkashi Fetish - On Data Set 0 1 - Despawn'),
(4273600, 9, 0, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79661, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Eat Murloc Kill Credit'),
(4273600, 9, 1, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79661, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Eat Murloc Kill Credit'),
(4273600, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79661, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Eat Murloc Kill Credit'),
(4273600, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Follow Target'),
(4273601, 9, 0, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273601, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 75, 82328, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Add Aura A Lashtail Hatchling: Fetish in Mouth Visual (A)'),
(4273601, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273601, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273601, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 43977, 5, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Set Data 0 1'),
(4273601, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 82324, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - A Lashtail Hatchling: Create Fetish (A)'),
(4273601, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 29, 82328, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Remove Aura A Lashtail Hatchling: Fetish in Mouth Visual (A)'),
(4273601, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Set Run 1'),
(4273601, 9, 9, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Follow Target'),
(4273602, 9, 0, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273602, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 75, 79702, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Add Aura A Lashtail Hatchling: Fetish in Mouth Visual (H)'),
(4273602, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273602, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 11, 79706, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Cast A Lashtail Hatchling: Dig Visual'),
(4273602, 9, 4, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 42772, 5, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Set Data 0 1'),
(4273602, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 79714, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - A Lashtail Hatchling: Create Fetish (H)'),
(4273602, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 29, 79702, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Remove Aura A Lashtail Hatchling: Fetish in Mouth Visual (H)'),
(4273602, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Set Run 1'),
(4273602, 9, 9, 0, 0, 0, 100, 0, 1000, 1000, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Lashtail Hatchling - On Script - Follow Target');
