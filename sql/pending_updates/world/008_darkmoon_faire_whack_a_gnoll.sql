-- Darkmoon Faire Whack-a-Gnoll scripts and cleanup

DELETE FROM `smart_scripts`
WHERE (`source_type` = 0 AND `entryorguid` IN (54444, 54466, 54546, 54549, 54601, 58570))
   OR (`source_type` = 9 AND `entryorguid` IN (5454600, 5454601, 5454602));

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = CASE `entry`
        WHEN 58570 THEN 'npc_whack_gnoll_bunny'
        WHEN 54601 THEN 'npc_whack_gnoll_mola'
        WHEN 54546 THEN 'npc_whack_gnoll_barrel'
        ELSE `ScriptName`
    END,
    `subname` = CASE
        WHEN `entry` = 54601 THEN 'Whack-a-Gnoll'
        ELSE `subname`
    END
WHERE `entry` IN (54546, 54601, 58570);

UPDATE `creature_template`
SET `AIName` = '',
    `ScriptName` = 'npc_whack_gnoll_target',
    `faction_A` = 2203,
    `faction_H` = 2203,
    `minlevel` = 1,
    `maxlevel` = 1,
    `unit_flags` = 33555200,
    `unit_flags2` = 0
WHERE `entry` IN (54444, 54466, 54549);

DELETE FROM `spell_script_names`
WHERE `spell_id` IN (101604, 101612)
   OR `ScriptName` IN ('spell_whack_gnoll_whack', 'spell_whack_gnoll_override_action');

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101604, 'spell_whack_gnoll_whack'),
(101612, 'spell_whack_gnoll_override_action');

DELETE FROM `areatrigger_scripts`
WHERE `entry` = 7344
   OR `ScriptName` = 'at_whack_a_gnoll_entrance';

INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7344, 'at_whack_a_gnoll_entrance');
