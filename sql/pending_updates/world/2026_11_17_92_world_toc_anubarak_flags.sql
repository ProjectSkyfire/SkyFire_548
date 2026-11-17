-- Anub'arak could not be fought: the template is immune to players, the script only clears the not attackable and not
-- selectable flags, and its MoveInLineOfSight override never starts combat. Use the flags of the Legends of Azeroth row.
UPDATE `creature_template` SET `unit_flags` = 32832 WHERE `entry` = 34564;
