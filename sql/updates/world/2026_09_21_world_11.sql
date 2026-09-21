-- Quest 26694 "The Grand Magus Doane": the Ward of Ilgalar (204582) is spawned six times on one
-- set of coordinates, so unlocking the door you clicked leaves five closed doors in the doorway.
--
-- All six rows are identical in every column -- map 0, spawnMask 1, phaseId 0, phaseGroup 0,
-- (-9282.32, -3327.47, 112.287), orientation 6.19592, rotation (0,0,0,1), spawntimesecs 120,
-- animprogress 255, state 1 -- and differ only by guid: 1192, 3233, 5425, 9296, 79542, 82519.
--
-- 204582 is gameobject_template type 0 (GAMEOBJECT_TYPE_DOOR) with Data1 = lock 1931. Lock 1931
-- is LOCK_KEY_ITEM on item 59522 "Key of Ilgalar", which is quest 26694's SourceItemId. Grand
-- Magus Doane (397, the quest's only objective) has one spawn behind that door.
--
-- The lowest guid is kept. None of the six is referenced anywhere: pool_gameobject,
-- game_event_gameobject and the negative-guid forms of smart_scripts have no rows for them, and
-- every one of the 4722 linked_respawn rows is linkType 0 (CREATURE_TO_CREATURE), which resolves
-- both columns as creature guids, so no gameobject guid is referenced there at all.

DELETE FROM `gameobject` WHERE `guid` IN (3233, 5425, 9296, 79542, 82519) AND `id` = 204582;
