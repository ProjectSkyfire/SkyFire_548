-- Darkmoon Faire cannon scorer visibility cleanup.

SET @DARKMOON_CANNON_TARGET_GUID := 8200300;

START TRANSACTION;

UPDATE `creature`
SET `npcflag` = 0,
    `unit_flags` = `unit_flags` | 33554944
WHERE `guid` = @DARKMOON_CANNON_TARGET_GUID
  AND `id` = 54224;

UPDATE `creature_template`
SET `modelid2` = 11686,
    `npcflag` = 0,
    `unit_flags` = `unit_flags` | 33554944,
    `unit_flags2` = `unit_flags2` | 2048,
    `flags_extra` = `flags_extra` | 128
WHERE `entry` = 54224;

COMMIT;
