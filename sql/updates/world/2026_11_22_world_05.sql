-- Rabbit (wild battle pet): wander randomly rather than remaining stationary.
-- https://www.wowhead.com/mop-classic/npc=61080/rabbit
-- Wild-pet replacements inherit their source's roaming radius in BattlePetSpawnMgr.
UPDATE `creature_template` SET `MovementType` = 1 WHERE `entry` = 61080;
