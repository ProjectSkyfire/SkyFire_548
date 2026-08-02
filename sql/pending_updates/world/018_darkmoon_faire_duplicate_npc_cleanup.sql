-- Darkmoon Faire: remove duplicate island NPC spawns.

DROP TEMPORARY TABLE IF EXISTS `tmp_darkmoon_duplicate_creatures`;
CREATE TEMPORARY TABLE `tmp_darkmoon_duplicate_creatures` (
    `guid` INT UNSIGNED NOT NULL PRIMARY KEY
);

INSERT INTO `tmp_darkmoon_duplicate_creatures` (`guid`) VALUES
(8850),
(8852),
(8854),
(8857),
(8858),
(8870),
(8871),
(8872),
(8874),
(8896),
(8899),
(8900),
(8905),
(8916),
(8926),
(8927),
(8928),
(8929),
(8930),
(8931),
(8932),
(8935),
(8937),
(8938),
(8968),
(8985),
(8993),
(8996),
(9006),
(9009),
(9020),
(9026),
(9028),
(9032),
(9037),
(9039),
(9040),
(9057),
(9086),
(9099),
(9101),
(9111);

DELETE `smart_scripts`
FROM `smart_scripts`
JOIN `tmp_darkmoon_duplicate_creatures` ON `smart_scripts`.`entryorguid` = 0 - CAST(`tmp_darkmoon_duplicate_creatures`.`guid` AS SIGNED)
WHERE `smart_scripts`.`source_type` = 0;

DELETE `creature_addon`
FROM `creature_addon`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `creature_addon`.`guid`;

DELETE `creature_formations`
FROM `creature_formations`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `creature_formations`.`leaderGUID`
    OR `tmp_darkmoon_duplicate_creatures`.`guid` = `creature_formations`.`memberGUID`;

DELETE `linked_respawn`
FROM `linked_respawn`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `linked_respawn`.`guid`
    OR `tmp_darkmoon_duplicate_creatures`.`guid` = `linked_respawn`.`linkedGuid`;

DELETE `game_event_creature`
FROM `game_event_creature`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `game_event_creature`.`guid`;

DELETE `game_event_model_equip`
FROM `game_event_model_equip`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `game_event_model_equip`.`guid`;

DELETE `game_event_npc_vendor`
FROM `game_event_npc_vendor`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `game_event_npc_vendor`.`guid`;

DELETE `game_event_npcflag`
FROM `game_event_npcflag`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `game_event_npcflag`.`guid`;

DELETE `pool_creature`
FROM `pool_creature`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `pool_creature`.`guid`;

DELETE `creature`
FROM `creature`
JOIN `tmp_darkmoon_duplicate_creatures` ON `tmp_darkmoon_duplicate_creatures`.`guid` = `creature`.`guid`;

DROP TEMPORARY TABLE IF EXISTS `tmp_darkmoon_duplicate_creatures`;
