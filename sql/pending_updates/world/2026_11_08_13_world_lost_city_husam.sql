-- Lost City of the Tol'vir: General Husam, his land mines, the Bad Intentions target and the shockwave stalker.
UPDATE `creature_template` SET `ScriptName` = 'boss_general_husam' WHERE `entry` = 44577;
UPDATE `creature_template` SET `ScriptName` = 'npc_land_mine' WHERE `entry` = 44796;
UPDATE `creature_template` SET `ScriptName` = 'npc_shockwave_stalker' WHERE `entry` = 44711;
UPDATE `creature_template` SET `ScriptName` = 'npc_bad_intentios_target' WHERE `entry` = 44586;

DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN ((83113, 'spell_bad_intentions'), (83236, 'spell_hurl'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(83113, 'spell_bad_intentions'),
(83236, 'spell_hurl');

DELETE FROM `creature_text` WHERE `entry` = 44577;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(44577, 0, 0, 'Invaders, you shall go no further!', 14, 0, 100, 0, 0, 21886, 'General Husam - Aggro'),
(44577, 1, 0, 'Insolent rat!', 14, 0, 100, 0, 0, 21889, 'General Husam - Kill'),
(44577, 2, 0, 'Murkash!', 14, 0, 100, 0, 0, 21888, 'General Husam - Shockwave'),
(44577, 3, 0, '|TInterface\\Icons\\spell_nature_earthquake.blp:20|t%s begins to cast |cFFFF0000|Hspell:83445|h[Shockwave]!|h|r', 41, 0, 100, 0, 0, 0, 'General Husam - Shockwave warning'),
(44577, 4, 0, 'Tread Lightly.', 14, 0, 100, 0, 0, 21887, 'General Husam - Mystic Trap'),
(44577, 5, 0, 'Siamat must not be freed! Turn back before it is too late!', 14, 0, 100, 0, 0, 21885, 'General Husam - Death');
