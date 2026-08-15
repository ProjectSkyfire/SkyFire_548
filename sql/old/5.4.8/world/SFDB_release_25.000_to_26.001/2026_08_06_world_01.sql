-- Lifebloom: Healing Touch, Nourish, and Regrowth refresh duration (unless Glyph of Blooming).
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_lifebloom_refresh';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(5185, 'spell_dru_lifebloom_refresh'),
(50464, 'spell_dru_lifebloom_refresh'),
(8936, 'spell_dru_lifebloom_refresh');
