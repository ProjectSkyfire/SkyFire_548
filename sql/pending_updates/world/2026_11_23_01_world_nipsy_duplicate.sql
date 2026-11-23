-- Remove the duplicate Nipsy in the Deeprun Tram.
-- Preserve spawn 52990 at the vendor position.
DELETE FROM `creature` WHERE `guid` = 58 AND `id` = 13018 AND `map` = 369;
