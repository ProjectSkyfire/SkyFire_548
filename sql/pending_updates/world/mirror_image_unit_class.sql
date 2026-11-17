-- Mirror Image: the images were warrior class, so they had no mana pool and could never pay the mana cost of their Frostbolt, Fireball and Arcane Blast.
UPDATE `creature_template` SET `unit_class` = 8 WHERE `entry` IN (31216, 47243, 47244);
