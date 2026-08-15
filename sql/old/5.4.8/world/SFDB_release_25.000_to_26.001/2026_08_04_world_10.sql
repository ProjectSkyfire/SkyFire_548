-- Revealing Strike (#968): Sinister Strike extra CP + Kidney Shot duration
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_sinister_strike',
    'spell_rog_kidney_shot'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(1752, 'spell_rog_sinister_strike'),
(408,  'spell_rog_kidney_shot');
