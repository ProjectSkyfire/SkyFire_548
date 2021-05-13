DELETE FROM `npc_trainer` WHERE `spell`=122661;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(200406, 122661, 40000, 755, 500, 80);

-- Ornate Band
DELETE FROM `spell_loot_template` WHERE `entry`=122661;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(122661, 83793, 90, 1, 1, 1, 1),
(122661, 90904, 10, 1, 1, 1, 1);
