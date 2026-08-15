-- Cenarion Ward (102351): on damage taken, apply the healing HoT (102352) to the
-- ward bearer instead of the attacker (default dummy TriggerSpell target).
DELETE FROM `spell_script_names`
WHERE `ScriptName` = 'spell_dru_cenarion_ward';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102351, 'spell_dru_cenarion_ward');
