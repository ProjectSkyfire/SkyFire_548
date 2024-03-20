DELETE FROM `creature_template_addon` WHERE `entry`=39270;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(39270, 0, 0, 7, 1, 0, '76143');

UPDATE `creature_template` SET `npcflag`= 16777216, `ScriptName`= 'npc_injured_razor_hill_grunt' WHERE `entry`= 39270;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`= 39270;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(39270, 73705, 1, 0);

DELETE FROM `spell_script_names` WHERE `spell_id`= 73705;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(73705, 'spell_injured_razor_hill_grunt_cancel_feign_death');

DELETE FROM `creature_text` WHERE `entry`=39270;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(39270, 0, 0, 'I can\'t fight any longer...', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 0, 1, 'Somebody... anybody...', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 0, 2, 'I\'ll never see my wife again...', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 0, 3, 'I\'ll never see my husband again...', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 0, 4, 'I can\'t fight any longer...', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 0, 5, 'H-help me', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Injured Razor Hill Grunt'),
(39270, 1, 0, 'Thank you, $r... I was close to death. May I stay by your side for a moment?', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 1, 1, 'You\'ve saved my life, $c. Protect me for a few more moments while I recover.', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 1, 2, 'For the Warchief You lead, I will follow.', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 1, 3, 'I was supposed to die in battle... ah well, another day. I will follow you until I recover.', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 2, 0, 'Thank you again. I will return to Razor Hill now.', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 2, 1, 'I feel much better now. For the Horde', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 2, 2, 'It was an honor to watch you fight, $n.', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player'),
(39270, 2, 3, 'You fight well. I will leave you now. Lok\'tar ogar', 12, 0, 100, 0, 0, 0, 'Injured Razor Hill Grunt to Player');
