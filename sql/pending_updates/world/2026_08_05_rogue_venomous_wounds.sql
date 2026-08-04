-- Venomous Wounds (#963)
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_rog_venomous_wounds';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(79134, 'spell_rog_venomous_wounds');

-- Ensure rupture script is bound (death refund is handled there)
DELETE FROM `spell_script_names` WHERE `spell_id` = 1943 AND `ScriptName` = 'spell_rog_rupture';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(1943, 'spell_rog_rupture');
