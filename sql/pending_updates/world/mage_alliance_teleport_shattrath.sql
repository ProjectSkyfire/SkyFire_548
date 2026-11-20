-- Alliance mage trainers did not list Teleport: Shattrath, the Horde list has its counterpart.
DELETE FROM `npc_trainer` WHERE `entry` = 200200 AND `spell` = 33690;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(200200, 33690, 81000, 0, 0, 62);
