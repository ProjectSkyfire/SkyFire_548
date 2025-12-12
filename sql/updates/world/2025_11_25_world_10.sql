-- Fix GameObject Missing SpellFocus errors
-- These gameobjects (GoType 8 = SPELL_FOCUS) reference SpellFocus IDs that don't exist in MoP DBC
-- Setting data0 to 0 removes the invalid SpellFocus reference
UPDATE `gameobject_template` SET `data0` = 0 WHERE `entry` IN (
212582, 212587, 212594, 212595, 213868, 213869, 213870, 213871, 213874, 213875, 213876, 213877
);

