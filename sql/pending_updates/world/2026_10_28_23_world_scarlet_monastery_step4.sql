-- Thalnos and summoned adds. Do not replace an existing AI or script binding.
UPDATE `creature_template` SET `ScriptName` = 'boss_thalnos_the_soulrender' WHERE `entry` = 59789 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_fallen_crusader' WHERE `entry` = 59884 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_empowering_spirit' WHERE `entry` = 59893 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_empowered_zombie' WHERE `entry` = 59930 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_evicted_soul' WHERE `entry` = 59974 AND `ScriptName` = '' AND `AIName` = '';

DELETE FROM `creature_text` WHERE `entry` = 59789 AND `groupid` BETWEEN 0 AND 6;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(59789, 0, 0, 'My endless agony shall be yours, as well!', 14, 0, 100, 0, 0, 27832, 'Thalnos aggro'),
(59789, 1, 0, 'No rest... for the angry dead!', 14, 0, 100, 0, 0, 27831, 'Thalnos raise'),
(59789, 2, 0, 'Seek out a vessel... and return!', 14, 0, 100, 0, 0, 27834, 'Thalnos evict soul'),
(59789, 3, 0, 'Claim a body, and wreak terrible vengeance!', 14, 0, 100, 0, 0, 27835, 'Thalnos empowering spirits'),
(59789, 4, 0, 'Can this be... the end, at last...?', 14, 0, 100, 0, 0, 27830, 'Thalnos death'),
(59789, 5, 0, '|TInterface\\Icons\\spell_warlock_demonsoul.blp:20|t%s casts |cFFFF0000|Hspell:115297|h[Evict Soul]|h|r on |cFF006EFD$n|r!', 41, 0, 100, 0, 0, 0, 'Thalnos evict soul announcement'),
(59789, 6, 0, '|TInterface\\Icons\\ability_warlock_jinx.blp:20|t%s begins to cast |cFFFF0000|Hspell:115147|h[Empowering Spirits]|h|r!', 41, 0, 100, 0, 0, 0, 'Thalnos empowering spirits announcement');

DELETE FROM `spell_script_names` WHERE `spell_id` = 115139 AND `ScriptName` = 'spell_raise_fallen_crusader';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (115139, 'spell_raise_fallen_crusader');

-- Spell 115290 creates template 220. Preserve any different existing script.
INSERT IGNORE INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(220, 0, 1, 2, 2, 2, 'sat_spirit_gale');
UPDATE `spell_areatrigger_template` SET `Flags` = 0, `CollisionType` = 1, `Radius` = 2, `ScaleX` = 2, `ScaleY` = 2, `ScriptName` = 'sat_spirit_gale' WHERE `Entry` = 220 AND `ScriptName` = '';
