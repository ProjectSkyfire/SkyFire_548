-- Keep Ilthalaine's original Shadowglen spawn and remove duplicate placements.
DELETE ca FROM `creature_addon` ca
INNER JOIN `creature` c ON c.`guid` = ca.`guid`
WHERE c.`id` = 2079 AND c.`guid` IN (8467026,8467027);
DELETE FROM `creature` WHERE `id` = 2079 AND `guid` IN (8467026,8467027);
