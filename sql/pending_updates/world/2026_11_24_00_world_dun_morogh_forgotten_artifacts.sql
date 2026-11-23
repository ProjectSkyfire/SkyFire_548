-- Dwarven Artifacts (24477): the 18 Forgotten Dwarven Artifacts (201608) of Coldridge Valley were spawned with state 0
-- (already used), so a click only answered "that is already being used" and the quest could not be finished. Legends of
-- Azeroth 5.4.8 spawns them with state 1 (ready). Upstream issue 1351.

UPDATE `gameobject` SET `state` = 1 WHERE `id` = 201608;
