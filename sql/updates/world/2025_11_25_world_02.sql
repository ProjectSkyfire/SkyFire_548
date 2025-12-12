-- Fix Rejuvenation double-tick issue
-- Rejuvenation has two periodic heal effects (EFFECT_0 and EFFECT_2)
-- Script prevents EFFECT_2 from ticking to avoid double heals

DELETE FROM `spell_script_names` WHERE `spell_id` = 774 AND `ScriptName` = 'spell_dru_rejuvenation';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(774, 'spell_dru_rejuvenation'),

