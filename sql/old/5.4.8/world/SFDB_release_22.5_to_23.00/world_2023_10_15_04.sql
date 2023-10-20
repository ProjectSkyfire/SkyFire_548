-- Elder Muur (First Aid Trainer)
UPDATE `creature_template` SET `trainer_type`=2 WHERE `entry`=66222;
DELETE FROM `npc_trainer` WHERE `entry`=66222;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(66222, -200303, 0, 0, 0, 0),
(66222, 110408, 300000, 129, 500, 80),
(66222, 102697, 90000, 129, 500, 0),
(66222, 102698, 110000, 129, 550, 0);

