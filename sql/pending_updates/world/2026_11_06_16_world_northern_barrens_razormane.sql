-- The Northern Barrens quest fix spawned a second Captured Razormane next to the one that already stands at the camp;
-- one prisoner is enough for By Hook Or By Crook.
DELETE FROM `creature` WHERE `guid` = 8311705 AND `id` = 34523;
