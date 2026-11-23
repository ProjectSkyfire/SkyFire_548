-- Public Pilgrim's Bounty feasts must work without a player owner.
-- Player-summoned feasts retain the template's party-only restriction.
UPDATE `gameobject_template` SET `ScriptName`='go_pilgrims_bountiful_feast'
 WHERE `entry`=195303;

-- The feast's script effect starts food, drink and the ten-second Well Fed timer.
DELETE FROM `spell_script_names` WHERE `spell_id`=66477 AND `ScriptName`='spell_pilgrims_bountiful_feast';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`)
 VALUES (66477,'spell_pilgrims_bountiful_feast');
