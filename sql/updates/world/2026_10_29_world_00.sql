-- Consecration (43499): untargetable trigger that creatures must not attack; casts the ground visual on summon
UPDATE `creature_template` SET `unit_flags` = 33554440, `minlevel` = 1, `maxlevel` = 1, `spell1` = 81298, `AIName` = 'TriggerAI' WHERE `entry` = 43499;
