-- Remove linked respawn rows that the loader skips because both creature spawns use spawnMask 0.

DELETE lr
FROM `linked_respawn` lr
JOIN `creature` slave ON slave.`guid` = lr.`guid`
JOIN `creature` master ON master.`guid` = lr.`linkedGuid`
WHERE lr.`linkType` = 0
  AND slave.`map` IN (409, 469)
  AND slave.`map` = master.`map`
  AND slave.`spawnMask` = 0
  AND master.`spawnMask` = 0;
