DELETE FROM `spell_script_names` WHERE `spell_id` IN (596, 2060, 2061);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(2061, 'spell_pri_flash_heal'),
(2060, 'spell_pri_greater_heal'),
(596, 'spell_pri_prayer_of_healing');
