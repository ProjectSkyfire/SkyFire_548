-- Remove waypoint scrips for npc's that are not spawned yet.
DELETE FROM `script_waypoint` WHERE `entry` IN (7807, 2917, 10427, 3465, 9623, 10646);
