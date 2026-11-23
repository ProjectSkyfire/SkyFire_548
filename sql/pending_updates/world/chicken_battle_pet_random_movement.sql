-- Wild battle-pet chickens replace ordinary chicken spawns at runtime.
-- Their movement comes from this template, not the source spawn's MovementType.
-- BattlePetSpawnMgr supplies the source roaming radius (or a five-yard fallback)
-- and home position before initializing the replacement's movement generator.
UPDATE `creature_template` SET `MovementType` = 1 WHERE `entry` = 62664;
