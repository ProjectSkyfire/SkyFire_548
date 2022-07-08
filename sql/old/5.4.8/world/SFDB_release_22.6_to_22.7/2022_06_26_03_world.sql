-- Fix damage calculations. cant multiply by 0.
UPDATE `creature_template` SET `dmg_multiplier`='1' WHERE `dmg_multiplier`='0';

