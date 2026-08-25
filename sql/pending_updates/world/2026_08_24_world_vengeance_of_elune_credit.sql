-- Quest 14005 The Vengeance of Elune: Bough suicide granted no kill credit
-- (self-instakill + low ability damage vs m_PlayerDamageReq). C++ credits the
-- hitting player. Turn-in RewardSpellCast 66166 needs a script to clear 65602.

DELETE FROM `spell_script_names` WHERE `spell_id`=66166 AND `ScriptName`='spell_q14005_cancel_vengeance_of_elune';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66166, 'spell_q14005_cancel_vengeance_of_elune');
