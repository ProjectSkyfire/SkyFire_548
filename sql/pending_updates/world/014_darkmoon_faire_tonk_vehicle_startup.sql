-- Darkmoon Faire Tonk Challenge vehicle startup data.

START TRANSACTION;

UPDATE `creature_template`
SET `unit_flags` = 16809984,
    `unit_flags2` = 2048,
    `VehicleId` = 1734,
    `spell1` = 102292,
    `spell2` = 102297,
    `AIName` = '',
    `ScriptName` = 'npc_darkmoon_steam_tonk'
WHERE `entry` = 54588;

DELETE FROM `spell_script_names`
WHERE `spell_id` = 100752
   OR `ScriptName` = 'spell_darkmoon_tonk_controller';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(100752, 'spell_darkmoon_tonk_controller');

COMMIT;
