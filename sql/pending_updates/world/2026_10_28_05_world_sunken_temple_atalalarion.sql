-- Sunken Temple: remove the static Atal'alarion spawn.
-- The statue event summons him when the sixth Atal'ai statue is lit (instance_sunken_temple.cpp, SummonCreature
-- of NPC_ATALALARION), and 4.3.4 and 3.3.5 have no static spawn for him. The static one made him appear twice.
-- No creature_addon, linked_respawn, pool_creature, game_event_creature, creature_formations, waypoint_data,
-- smart_scripts or conditions rows reference this guid.
DELETE FROM `creature` WHERE `guid` = 34521 AND `id` = 8580 AND `map` = 109;
