-- Darkmoon Faire basic zone script bindings

UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_darkmoon_deathmatch_announcer'
WHERE `entry` = 55402;

UPDATE `gameobject_template`
SET `ScriptName` = 'go_darkmoon_treasure_chest'
WHERE `entry` = 209620;

DELETE FROM `spell_script_names`
WHERE `ScriptName` IN (
    'spell_whee_trigger',
    'spell_darkmoon_free_your_mind',
    'spell_darkmoon_moonfang_tears',
    'spell_darkmoon_moonfang_curse'
);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(134815, 'spell_whee_trigger'),
(145111, 'spell_darkmoon_free_your_mind'),
(144702, 'spell_darkmoon_moonfang_tears'),
(144590, 'spell_darkmoon_moonfang_curse');
