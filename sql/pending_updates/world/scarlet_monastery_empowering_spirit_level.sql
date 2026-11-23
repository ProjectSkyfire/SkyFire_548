-- The Empowering Spirits that Thalnos summons were level 90, so a level 32 group could not hurt them. The other adds of
-- the fight and the rest of the dungeon are level 32.

UPDATE `creature_template` SET `minlevel` = 32, `maxlevel` = 32 WHERE `entry` = 59893;
