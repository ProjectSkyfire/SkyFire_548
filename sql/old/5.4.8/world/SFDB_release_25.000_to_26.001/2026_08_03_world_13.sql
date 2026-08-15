DELETE FROM `spell_script_names`
WHERE `spell_id` IN (-31850, 31850)
  AND `ScriptName` = 'spell_pal_ardent_defender';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(31850, 'spell_pal_ardent_defender');
