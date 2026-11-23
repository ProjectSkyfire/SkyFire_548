-- Kil'jaeden rose from the Sunwell but could not be selected: the template kept the not selectable flag and the script
-- never clears it. Use the flags of the Hand of the Deceiver, which are also those of the Legends of Azeroth row.
UPDATE `creature_template` SET `unit_flags` = 32832 WHERE `entry` = 25315;
