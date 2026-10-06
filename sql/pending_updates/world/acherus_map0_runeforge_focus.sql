-- Floating Acherus / Ebon Hold (Eastern Plaguelands, map 0): add the missing
-- type-8 SPELLFOCUS runeforge (entry 226777, focus id 1552), co-located with the
-- existing type-5 (GENERIC) visual runeforges (207577/207578/207579). Without the
-- spell-focus object, Runeforging (spell 53428) fails there with
-- SPELL_FAILED_REQUIRES_SPELL_FOCUS. The DK intro instance (map 609) already
-- pairs each visual runeforge with a 226777 focus; map 0 was missing it.
-- Fixes #1518.
SET @OGUID := 901363;
DELETE FROM `gameobject` WHERE `guid` BETWEEN @OGUID-2 AND @OGUID-0;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`) VALUES
(@OGUID-0, 226777, 0, 2427.28, -5544.45, 420.863, -0.983229, 0, 0, 0.292372, 0.956305, 300),
(@OGUID-1, 226777, 0, 2509.31, -5560.39, 420.863, -2.554020, 0, 0, 0.292372, 0.956305, 300),
(@OGUID-2, 226777, 0, 2493.37, -5642.43, 420.863,  2.164210, 0, 0, 0.292372, 0.956305, 300);
