-- Coastal Delicacies! (28146): the player takes a Hungry Mine Creeper to the Western Strand and it eats the murlocs it
-- kills. The creeper's summon aura (88762) was never granted, and the creeper had no script, so no murloc ever counted.
-- Added as in Legends of Azeroth 5.4.8: the area aura that summons the creeper while the quest is in progress in Hillsbrad
-- Foothills, and the creeper's SmartAI (follows its summoner on the defensive stance, credits the summoner for every Torn
-- Fin Muckdweller (2374) or Coastrunner (2375) it kills, the proxy credit 47752).

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 47662;

DELETE FROM `spell_area` WHERE `spell` = 88762 AND `area` = 267 AND `quest_start` = 28146;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(88762, 267, 28146, 28146, 0, 0, 2, 1, 74, 11);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 47662 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47662, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hungry Mine Creeper - On Summon - Store Target'),
(47662, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hungry Mine Creeper - On Summon - State Defensive'),
(47662, 0, 2, 0, 5, 0, 100, 0, 0, 0, 0, 2374, 0, 33, 47752, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Hungry Mine Creeper - On Kill Npc - Killcredit'),
(47662, 0, 3, 0, 5, 0, 100, 0, 0, 0, 0, 2375, 0, 33, 47752, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Hungry Mine Creeper - On Kill Npc - Killcredit');
