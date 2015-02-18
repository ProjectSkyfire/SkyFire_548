SET @CRIMSON := 60198; -- Crimson Lory
SET @DUSKY := 60196; -- Dusky Lory
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (@DUSKY, @CRIMSON);
