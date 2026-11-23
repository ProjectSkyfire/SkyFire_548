-- Melee damage for the Nightmare Horror, Stalker and Mass, whose templates held 0/0. The low end is the class level
-- damage curve average / 1.35 and the high end averages out to the curve; attack power comes from the same curve
-- and the multiplier is 1 for these normal creatures. Rows with damage keep it.
UPDATE `creature_template` SET `mindmg` = 57, `maxdmg` = 98, `attackpower` = 100, `dmg_multiplier` = 1 WHERE `entry` = 37791 AND `maxdmg` = 0;
UPDATE `creature_template` SET `mindmg` = 68, `maxdmg` = 117, `attackpower` = 108, `dmg_multiplier` = 1 WHERE `entry` IN (37736, 37753) AND `maxdmg` = 0;
