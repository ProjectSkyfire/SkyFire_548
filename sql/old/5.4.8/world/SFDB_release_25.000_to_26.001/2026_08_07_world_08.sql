-- Soul of the Forest (114107): per-spec talent scripting.
-- Balance procs Astral Insight; Feral finishers refund Energy; Guardian buffs Mangle;
-- Restoration grants haste on Swiftmend. Astral Communion grants eclipse and consumes Insight.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN (
    'spell_dru_soul_of_the_forest',
    'spell_dru_soul_of_the_forest_feral',
    'spell_dru_soul_of_the_forest_mangle',
    'spell_dru_swiftmend',
    'spell_dru_astral_communion'
);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(114107, 'spell_dru_soul_of_the_forest'),
(33878, 'spell_dru_soul_of_the_forest_mangle'),
(1079, 'spell_dru_soul_of_the_forest_feral'),
(22568, 'spell_dru_soul_of_the_forest_feral'),
(22570, 'spell_dru_soul_of_the_forest_feral'),
(52610, 'spell_dru_soul_of_the_forest_feral'),
(127538, 'spell_dru_soul_of_the_forest_feral'),
(18562, 'spell_dru_swiftmend'),
(127663, 'spell_dru_astral_communion');
