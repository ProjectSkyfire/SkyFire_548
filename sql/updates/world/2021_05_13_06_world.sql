DELETE FROM `npc_trainer` WHERE `spell`=110421;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(200406, 110421, 510000, 755, 500, 80);

DELETE FROM `spell_ranks` WHERE `first_spell_id`=25245;
INSERT INTO `spell_ranks` (`first_spell_id`, `spell_id`, `spell_rank`) VALUES
(25245, 25246, 2),
(25245, 28896, 3),
(25245, 28899, 4),
(25245, 28901, 5),
(25245, 51310, 6),
(25245, 73319, 7),
(25245, 110421, 8);
