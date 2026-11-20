-- Ice Barrier and Incanter's Ward add their spell power part in these scripts.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (11426, 1463);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(11426, 'spell_mage_ice_barrier'),
(1463, 'spell_mage_incanters_ward');
