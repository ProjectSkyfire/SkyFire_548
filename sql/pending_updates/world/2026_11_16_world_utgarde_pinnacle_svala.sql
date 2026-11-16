-- Utgarde Pinnacle: Svala Sorrowgrave stayed immune to players after her intro
UPDATE `creature_template` SET `unit_flags`=`unit_flags`&~0x00000100 WHERE `entry` IN (26668,29281);
