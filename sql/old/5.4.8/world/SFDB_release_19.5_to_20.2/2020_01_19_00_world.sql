DELETE FROM `spell_learn_spell` WHERE `entry`=11366 AND `SpellID`=44448;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES (11366, 44448, 0);

DELETE FROM `spell_required` WHERE `spell_id`=44448 AND `req_spell`=11366;
INSERT INTO `spell_required` (`spell_id`, `req_spell`) VALUES (44448, 11366);

DELETE FROM `spell_script_names` WHERE `spell_id`=11366 AND `ScriptName`='spell_mage_pyroblast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (11366, 'spell_mage_pyroblast');
