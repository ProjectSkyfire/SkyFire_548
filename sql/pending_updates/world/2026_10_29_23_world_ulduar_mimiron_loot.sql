-- Ulduar: Cache of Innovation loot. The 25 player caches (194956, 194958) dropped item 45083 instead of the
-- Fragment of Val'anyr (45038).
UPDATE `gameobject_loot_template` SET `item` = 45038 WHERE `entry` IN (26963, 26967) AND `item` = 45083;
