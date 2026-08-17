-- Restore sniffed Northshire NPC reactions and remove false quest-giver flags.
UPDATE `creature_template`
SET `faction_A`=32, `faction_H`=32, `npcflag`=0
WHERE `entry`=49871;

UPDATE `creature_template`
SET `npcflag`=0
WHERE `entry`=42940;

DELETE FROM `creature_queststarter`
WHERE `id`=42940 AND `quest`=26391;
