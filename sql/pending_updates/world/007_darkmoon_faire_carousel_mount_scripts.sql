-- Darkmoon Faire carousel mount ticket enforcement

UPDATE `creature_template`
SET `ScriptName` = 'vehicle_darkmoon_carousel_mount'
WHERE `entry` IN (68231, 68232, 68233, 68238, 68239);
