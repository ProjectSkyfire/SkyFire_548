-- Fix GameObject Missing Lock errors
-- These gameobjects reference Lock IDs that don't exist in MoP DBC
-- Setting lockId to 0 (no lock required) for all affected gameobjects

-- GoType 0 = DOOR: lockId is in data1
UPDATE `gameobject_template` SET `data1` = 0 WHERE `entry` IN (
211614, 211615, 211963, 211970, 211976, 211977, 211981, 211982, 212246,
213090, 213091,
213268, 213269, 213270, 213271, 213395, 213396, 213397, 213398,
214641,
215130, 215363, 215364, 215365, 215366, 215381, 215382,
215391, 215459,
216060,
216354, 216355, 216356, 216357, 216358, 216359
);

-- GoType 2 = QUESTGIVER: lockId is in data0
-- Entry 202357 has data0=0 but the error indicates lock 0 doesn't exist
-- Setting to 0 is correct (no lock), but if validation requires a non-zero value,
-- we'll leave it as 0 since 0 means "no lock required"
-- Note: This entry may be a false positive if 0 is a valid "no lock" value
UPDATE `gameobject_template` SET `data0` = 0 WHERE `entry` = 202357;

