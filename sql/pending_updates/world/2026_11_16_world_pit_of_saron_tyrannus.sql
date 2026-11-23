-- Scourgelord Tyrannus stayed not selectable after his intro, so he could not be attacked
UPDATE `creature_template` SET `unit_flags`=`unit_flags`&~0x02000000 WHERE `entry`=36658;
