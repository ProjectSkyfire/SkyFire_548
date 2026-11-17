-- Type flag 0x80000000 makes the 5.4.8 client show a skull level and mishandle the creature.
-- 2026-06-28_creature_type_flags_display_fix.sql only cleared templates where it was the sole flag.
UPDATE `creature_template` SET `type_flags`=`type_flags`&~0x80000000 WHERE `type_flags`&0x80000000;
