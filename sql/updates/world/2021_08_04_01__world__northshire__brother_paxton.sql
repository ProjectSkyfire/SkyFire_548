-- Brother Paxton
SET @ENTRY := 951;
DELETE FROM `smart_scripts` WHERE `entryOrGuid` = @ENTRY;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_brother_paxton' WHERE `entry`= @ENTRY;

-- remove old data for loop cast "Power Word: Fortitude"
DELETE FROM `creature_template_addon` WHERE `entry` = @ENTRY;

-- Add waypoints
DELETE FROM `waypoints` WHERE `entry`=@ENTRY;
DELETE FROM `waypoint_data` WHERE `id`=@ENTRY;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`) VALUES
(@ENTRY, 1, -8819.92, -149.533, 81.1643),
(@ENTRY, 2, -8825.13, -146.509, 80.5701),
(@ENTRY, 3, -8819.92, -149.533, 81.1643),
(@ENTRY, 4, -8816.47, -153.078, 81.6179);

-- Add text
DELETE FROM `creature_text` WHERE `entry`=@ENTRY;
INSERT INTO `creature_text`(`entry`, `groupid`, `id`, `text`, `type`, `probability`, `comment`) VALUES 
(@ENTRY, 0, 0, "Be healed, $g brother:sister;!", 12, 25, "taken from https://www.wowhead.com/npc=951/brother-paxton#abilities"),
(@ENTRY, 0, 1, "Let the Holy Light embrace you!", 12, 25, "taken from https://www.wowhead.com/npc=951/brother-paxton#abilities"),
(@ENTRY, 0, 2, "FIGHT ON, $G BROTHER:SISTER;!", 12, 25, "taken from https://www.wowhead.com/npc=951/brother-paxton#abilities"),
(@ENTRY, 1, 0, "AND I LAY MY HANDS UPON YOU!", 12, 25, "taken from https://www.wowhead.com/npc=951/brother-paxton#abilities"),
(@ENTRY, 1, 1, "BY THE LIGHT BE RENEWED!", 12, 25, "taken from https://www.wowhead.com/npc=951/brother-paxton#abilities");