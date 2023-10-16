-- Trapper Ri (Skinning Trainer)
UPDATE `creature_template` SET `trainer_type`=2 WHERE `entry`=66981;
DELETE FROM `npc_trainer` WHERE `entry`=66981;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(66981, -200409, 0, 0, 0, 0),
(66981, 102220, 600000, 393, 500, 80);

