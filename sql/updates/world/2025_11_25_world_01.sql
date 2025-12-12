-- Fix for Quest 29524 "The Lesson of Stifled Pride"
-- Reposition Tushui Trainees (54587) in a half circle around Instructor Zhi (61411)
-- Instructor Zhi is at: 1441.26, 3372.11, 173.379, orientation 4.36139
-- Positions based on quest POI data showing trainees should be around 1436-1458, 3378-3389

-- Update existing Tushui Trainee spawns to correct positions
-- This preserves existing GUIDs and just updates positions/orientations
-- Half circle formation: 7 trainees per phase positioned around Instructor Zhi

-- Position 1: Old (1329.74, 3316.41) -> New (1436.0, 3389.0)
UPDATE `creature`
SET `position_x` = 1436.0, `position_y` = 3389.0, `position_z` = 173.4, `orientation` = 4.5
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1329.74) < 0.1 AND ABS(`position_y` - 3316.41) < 0.1;

-- Position 2: Old (1333.6, 3313.38) -> New (1439.0, 3388.0)
UPDATE `creature`
SET `position_x` = 1439.0, `position_y` = 3388.0, `position_z` = 173.4, `orientation` = 4.4
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1333.6) < 0.1 AND ABS(`position_y` - 3313.38) < 0.1;

-- Position 3: Old (1332.86, 3309.69) -> New (1441.0, 3385.0)
UPDATE `creature`
SET `position_x` = 1441.0, `position_y` = 3385.0, `position_z` = 173.4, `orientation` = 4.3
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1332.86) < 0.1 AND ABS(`position_y` - 3309.69) < 0.1;

-- Position 4: Old (1332.56, 3305.59) -> New (1445.0, 3382.0)
UPDATE `creature`
SET `position_x` = 1445.0, `position_y` = 3382.0, `position_z` = 173.4, `orientation` = 4.2
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1332.56) < 0.1 AND ABS(`position_y` - 3305.59) < 0.1;

-- Position 5: Old (1329.31, 3312.00) -> New (1457.0, 3381.0)
UPDATE `creature`
SET `position_x` = 1457.0, `position_y` = 3381.0, `position_z` = 173.4, `orientation` = 4.1
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1329.31) < 0.1 AND ABS(`position_y` - 3312.0) < 0.1;

-- Position 6: Old (1328.34, 3303.69) -> New (1458.0, 3378.0)
UPDATE `creature`
SET `position_x` = 1458.0, `position_y` = 3378.0, `position_z` = 173.4, `orientation` = 4.0
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1328.34) < 0.1 AND ABS(`position_y` - 3303.69) < 0.1;

-- Position 7: Old (1329.16, 3308.09) -> New (1443.0, 3375.0)
UPDATE `creature`
SET `position_x` = 1443.0, `position_y` = 3375.0, `position_z` = 173.4, `orientation` = 4.35
WHERE `id` = 54587 AND `map` = 860
AND `phaseId` IN (592, 593, 594, 595, 596, 597, 598)
AND ABS(`position_x` - 1329.16) < 0.1 AND ABS(`position_y` - 3308.09) < 0.1;
