-- Mei Lin offers ground riding training in Stormwind.
-- https://www.wowhead.com/mop-classic/npc=70296/mei-lin
UPDATE `creature_template`
SET `npcflag` = `npcflag` | 16, `trainer_type` = 1, `trainer_class` = 0, `trainer_race` = 0
WHERE `entry` = 70296;

-- Use the core's existing ground-riding teaching spells and base prices.
-- Do not reference the complete riding list, which also includes flight training.
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
VALUES
(70296, 33389, 40000, 0, 0, 20),
(70296, 33392, 500000, 762, 75, 40)
ON DUPLICATE KEY UPDATE `spell` = VALUES(`spell`);
