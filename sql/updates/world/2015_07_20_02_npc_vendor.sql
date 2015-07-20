SET @VENDOR := 67181;
DELETE FROM `npc_vendor` WHERE `entry`=@VENDOR;
INSERT INTO `npc_vendor` (`entry`, `item`, `slot`, `maxcount`, `ExtendedCost`, `Type`) VALUES
(@VENDOR, 81402, 1, 0, 0, 1), -- 81402
(@VENDOR, 81410, 2, 0, 0, 1), -- 81410
(@VENDOR, 117, 3, 0, 0, 1), -- 117
(@VENDOR, 19304, 4, 0, 0, 1), -- 19304
(@VENDOR, 67230, 5, 0, 0, 1), -- 67230
(@VENDOR, 83097, 6, 0, 0, 1), -- 83097
(@VENDOR, 81406, 7, 0, 0, 1), -- 81406
(@VENDOR, 81414, 8, 0, 0, 1), -- 81414
(@VENDOR, 74636, 9, 0, 0, 1), -- 74636
(@VENDOR, 6256, 455, 0, 0, 1), -- 6256
(@VENDOR, 6365, 456, 1, 0, 1), -- 6365
(@VENDOR, 6529, 457, 0, 0, 1), -- 6529
(@VENDOR, 6530, 458, 0, 0, 1), -- 6530
(@VENDOR, 6532, 459, 0, 0, 1); -- 6532
