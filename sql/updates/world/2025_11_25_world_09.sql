-- Fix Creature KillCredit Issues
-- Creatures 71292 (Augur Narali) and 71293 (Deadeye Kail) reference non-existent creature 71066 in KillCredit2
-- Both have valid KillCredit1=70968 (Kor'kron Warmonger), so setting KillCredit2 to 0 removes the invalid reference
UPDATE `creature_template` SET `KillCredit2` = 0 WHERE `entry` IN (71292, 71293);

