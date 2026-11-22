-- Nerub'enkan's Undead Scarabs poison their targets (journal: "melee attacks have a chance to poison their current
-- target"), but the entry carried no aura for it: only a Poison (34969) in the unused spell list. Poison Proc (13299)
-- has a 15 percent chance on melee hits and triggers Poison (13298), the spell the journal names. The scarabs are only
-- summoned by the boss, so the template addon is the place for the aura (Deathclasp, 15196, gets it the same way).
DELETE FROM `creature_template_addon` WHERE `entry` = 10876;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(10876, 0, 0, 0, 0, 0, '13299');
