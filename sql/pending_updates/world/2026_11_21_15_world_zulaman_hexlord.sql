-- Gazakroth, one of the four pets the Hex Lord brings, was a level 70 mob of the monster faction while the other
-- three are level 85 and fight on his side.
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 1890, `faction_H` = 1890 WHERE `entry` = 24244 AND `minlevel` = 70;
