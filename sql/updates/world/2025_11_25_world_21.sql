-- Fix gossip_menu_option errors

-- 1. Remove non-existing action_poi_id references
-- Menu 3356, id 2 and 3 reference action_poi_id 431 and 432 which don't exist in points_of_interest table
-- Setting action_poi_id to 0 (no POI) to fix the errors

UPDATE `gossip_menu_option` SET `action_poi_id` = 0 WHERE `menu_id` = 3356 AND `id` = 2 AND `action_poi_id` = 431;
UPDATE `gossip_menu_option` SET `action_poi_id` = 0 WHERE `menu_id` = 3356 AND `id` = 3 AND `action_poi_id` = 432;

-- 2. Fix unknown icon id
-- Menu 12094, id 0 has unknown icon id 21 (not in GOSSIP_ICON enum)
-- Setting option_icon to 0 (GOSSIP_ICON_CHAT) to fix the error

UPDATE `gossip_menu_option` SET `option_icon` = 0 WHERE `menu_id` = 12094 AND `id` = 0 AND `option_icon` = 21;

