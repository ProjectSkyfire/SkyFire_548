-- Faerie Fire / Faerie Swarm: Weakened Armor (and Swarm snare) via spell scripts.
DELETE FROM `spell_script_names`
WHERE `ScriptName` IN ('spell_dru_faerie_fire', 'spell_dru_faerie_swarm')
   OR (`spell_id` IN (770, 102355) AND `ScriptName` IN ('spell_dru_faerie_fire', 'spell_dru_faerie_swarm'));

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(770,    'spell_dru_faerie_fire'),
(102355, 'spell_dru_faerie_swarm');
