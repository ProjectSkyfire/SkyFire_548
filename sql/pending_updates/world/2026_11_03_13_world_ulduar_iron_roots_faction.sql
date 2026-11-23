-- Ulduar, Freya: the strengthened Iron Roots and the 25-player Iron Roots were friendly (faction 35), so a rooted
-- player could not be freed by attacking them. They are hostile (faction 14) like the 10-player Iron Roots.
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (33168, 33396, 33397);
