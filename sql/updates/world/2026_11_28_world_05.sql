-- Juggernaut's Focusing Crystal: item-scaled self healing from damage dealt.
-- Ra-den: snapshot current health for Murderous Strike's periodic damage.
DELETE FROM `spell_script_names` WHERE (`spell_id` = 146346 AND `ScriptName` = 'spell_item_juggernauts_focusing_crystal')
    OR (`spell_id` = 138333 AND `ScriptName` = 'spell_gen_raden_murderous_strike');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(146346, 'spell_item_juggernauts_focusing_crystal'),
(138333, 'spell_gen_raden_murderous_strike');
