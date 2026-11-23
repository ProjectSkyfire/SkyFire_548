-- Splintertree Guard (56894): the template carried not selectable (0x02000000), stunned (0x00040000) and the feign
-- death unit flag 2049 plus the dead dynamic flag, on top of the feign death aura 29266. They drew dead and could not
-- be selected. The aura alone gives the fallen look and the guards stay selectable.
UPDATE `creature_template` SET `unit_flags` = 0, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 56894;
