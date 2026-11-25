-- Keep the existing working service option instead of activating an untyped duplicate.
-- Referenced holiday/script options in menus 342, 345, 349 and 441 remain untouched.
CREATE TEMPORARY TABLE gossip_service_duplicates (menu INT UNSIGNED, old_id INT UNSIGNED, keep_id INT UNSIGNED);
INSERT INTO gossip_service_duplicates VALUES
(344, 1, 3),
(347, 0, 2),
(1581, 1, 2),
(1582, 0, 2),
(1582, 1, 3),
(3161, 1, 0),
(3162, 1, 0),
(3186, 1, 0),
(4001, 1, 0),
(4004, 1, 0),
(4005, 1, 0),
(4006, 1, 0),
(6525, 1, 3),
(7288, 1, 3),
(7857, 0, 1),
(7857, 5, 6),
(7869, 2, 0),
(7869, 3, 1),
(7896, 1, 2),
(7949, 0, 2),
(7949, 1, 3),
(8884, 0, 2),
(8884, 1, 3),
(8903, 1, 0),
(9427, 0, 2),
(9427, 1, 3),
(9460, 0, 2),
(9478, 0, 2),
(9478, 1, 3),
(9549, 1, 0),
(9763, 0, 1),
(10029, 0, 2),
(10426, 1, 0);
DELETE old_option FROM gossip_menu_option old_option
JOIN gossip_service_duplicates d ON d.menu = old_option.MenuID AND d.old_id = old_option.OptionID
JOIN gossip_menu_option keep_option ON keep_option.MenuID = d.menu AND keep_option.OptionID = d.keep_id
WHERE old_option.OptionType = 0 AND old_option.OptionNpcflag = 0
  AND old_option.OptionText = keep_option.OptionText AND keep_option.OptionType IN (3, 8)
  AND old_option.ActionMenuID = 0 AND old_option.ActionPoiID = 0
  AND old_option.BoxCoded = 0 AND old_option.BoxMoney = 0
  AND NOT EXISTS (SELECT 1 FROM conditions c WHERE c.SourceTypeOrReferenceId = 15 AND c.SourceGroup = d.menu AND c.SourceEntry = d.old_id)
  AND NOT EXISTS (SELECT 1 FROM smart_scripts ss WHERE ss.event_type = 62 AND ss.event_param1 = d.menu AND ss.event_param2 = d.old_id);
DROP TEMPORARY TABLE gossip_service_duplicates;
