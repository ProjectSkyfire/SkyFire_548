-- Desolace: Dumti sells items, but his template lacked the vendor flag. Safe to run twice.
UPDATE `creature_template` SET `npcflag` = 131 WHERE `entry` = 36378;
