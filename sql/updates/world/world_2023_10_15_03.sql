-- Grower Miao (Herbalism Trainer)
UPDATE `creature_template` SET `trainer_type`=2 WHERE `entry`=66980;
DELETE FROM `npc_trainer` WHERE `entry`=66980;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(66980, -200404, 0, 0, 0, 0),
(66980, 110415, 600000, 182, 500, 80),
