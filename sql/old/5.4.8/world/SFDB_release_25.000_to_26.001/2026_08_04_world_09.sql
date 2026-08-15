-- Killing Spree (#966): require target; random hops only with Blade Flurry
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_rog_killing_spree',
    'spell_rog_killing_spree_target_selector'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(51690, 'spell_rog_killing_spree'),
(51690, 'spell_rog_killing_spree_target_selector');
