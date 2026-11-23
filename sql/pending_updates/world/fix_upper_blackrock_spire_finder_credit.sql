-- Upper Blackrock Spire (Dungeon Finder id 330) never got a completion: no encounter row names it.
-- General Drakkisath is its last boss; Overlord Wyrmthalak keeps Lower Blackrock Spire (id 32).

UPDATE `instance_encounters` SET `lastEncounterDungeon` = 330 WHERE `entry` = 280 AND `creditType` = 0 AND `creditEntry` = 10363;
