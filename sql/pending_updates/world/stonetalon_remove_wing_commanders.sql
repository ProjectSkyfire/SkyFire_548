-- The 49 Gnome Wing Commanders (42017) standing in the air over Krom'gar Fortress have no vehicle and no AI, so they cannot
-- be reached. The commanders are the riders of the Gnomish Bombers (vehicle_template_accessory 42015 to 42017), and the
-- spawns look like sniffed positions of pilots parachuting down. They came in with
-- 2026_11_11_38_world_stonetalon_mountains_quest_blockers.sql.
DELETE FROM `creature` WHERE `guid` BETWEEN 9255160 AND 9255208 AND `id` = 42017;
