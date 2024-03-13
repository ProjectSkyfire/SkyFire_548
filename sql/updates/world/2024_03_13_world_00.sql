DELETE FROM `spell_area` WHERE `spell`=114611;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(114611, 5834, 29408, 0, 0, 0, 2, 1, 10, 11);

DELETE FROM `smart_scripts` WHERE  `entryorguid`=53566 AND `id`=9;

UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `entry`=54631 AND `item`=72112;

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209326;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209973;
