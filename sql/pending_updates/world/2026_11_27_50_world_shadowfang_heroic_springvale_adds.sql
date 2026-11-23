-- The Wailing Guardsman and Tormented Officer next to Commander Springvale only exist in heroic mode:
-- the 5.4.8 dungeon journal lists both for the heroic difficulties only, and neither the 4.3.4 data nor the
-- 5.4.8 data has a static spawn in normal mode (heroic spawns them from the boss script).
UPDATE `creature` SET `spawnMask` = 4 WHERE `map` = 33 AND `guid` IN (6954, 6956) AND `id` IN (50613, 50615);
