-- Darkmoon Faire Shooting Gallery scripts

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (14841, 24171, 54231))
   OR (`source_type` = 9 AND `entryorguid` IN (1484100, 1484101, 1484102));

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = CASE `entry`
        WHEN 14841 THEN 'npc_darkmoon_rinling'
        WHEN 54231 THEN 'npc_darkmoon_shooting_gallery_target'
        WHEN 24171 THEN ''
        ELSE `ScriptName`
    END,
    `subname` = CASE
        WHEN `entry` = 14841 THEN 'Shooting Gallery'
        ELSE `subname`
    END
WHERE `entry` IN (14841, 24171, 54231);

UPDATE `creature_template`
SET `faction_A` = 35,
    `faction_H` = 35,
    `minlevel` = 1,
    `maxlevel` = 1,
    `unit_flags` = 33555200,
    `unit_flags2` = 2048,
    `InhabitType` = 3,
    `Health_mod` = 0.01
WHERE `entry` = 54231;

DELETE FROM `spell_script_names`
WHERE `spell_id` IN (101871, 101872)
   OR `ScriptName` IN ('spell_darkmoon_shooting_gallery_override_action', 'spell_darkmoon_shooting_gallery_shoot');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101871, 'spell_darkmoon_shooting_gallery_override_action'),
(101872, 'spell_darkmoon_shooting_gallery_shoot');
