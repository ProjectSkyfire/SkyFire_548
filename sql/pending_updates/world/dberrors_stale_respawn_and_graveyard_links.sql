-- Remove stale loader links that point to missing creature or area data.

DELETE lr
FROM `linked_respawn` lr
LEFT JOIN `creature` slave ON slave.`guid` = lr.`guid`
LEFT JOIN `creature` master ON master.`guid` = lr.`linkedGuid`
WHERE lr.`linkType` = 0
  AND (slave.`guid` IS NULL OR master.`guid` IS NULL);

DELETE FROM `game_graveyard_zone`
WHERE `ghost_zone` = 8505;
