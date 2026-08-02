-- Living Bomb explosion: bind MoP target-cap script (bomb target + up to 3 extras).
DELETE FROM `spell_script_names` WHERE `spell_id` = 44461 AND `ScriptName` = 'spell_mage_living_bomb_explosion';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(44461, 'spell_mage_living_bomb_explosion');
