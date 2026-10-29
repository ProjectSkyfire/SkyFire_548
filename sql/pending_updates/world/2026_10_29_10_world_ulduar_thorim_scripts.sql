-- Ulduar: Thorim script bindings (creatures, spells, achievement criteria and the Leap condition).

-- Creature scripts.
UPDATE `creature_template` SET `ScriptName` = 'boss_thorim' WHERE `entry` = 32865;
UPDATE `creature_template` SET `ScriptName` = 'npc_runic_colossus' WHERE `entry` = 32872;
UPDATE `creature_template` SET `ScriptName` = 'npc_ancient_rune_giant' WHERE `entry` = 32873;
UPDATE `creature_template` SET `ScriptName` = 'npc_sif' WHERE `entry` = 33196;
-- Jormungar Behemoth, Captured Mercenary Soldiers (both factions), Dark Rune Acolyte and Captured Mercenary Captains (both factions).
UPDATE `creature_template` SET `ScriptName` = 'npc_thorim_pre_phase' WHERE `entry` IN (32882, 32883, 32885, 32886, 32907, 32908);
-- Iron Ring Guard, Iron Honor Guard, Dark Rune Champion, Warbringer, Evoker, Commoner and the hallway Dark Rune Acolyte.
UPDATE `creature_template` SET `ScriptName` = 'npc_thorim_arena_phase' WHERE `entry` IN (32874, 32875, 32876, 32877, 32878, 32904, 33110);

-- Spell scripts.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (61934, 62016, 62042, 62057, 62058, 62184, 62320, 62466, 62576, 62580, 62602, 62604, 64767, 64909) AND `ScriptName` LIKE 'spell\_thorim\_%';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(61934, 'spell_thorim_arena_leap'),
(62016, 'spell_thorim_charge_orb'),
(62042, 'spell_thorim_stormhammer'),
(62057, 'spell_thorim_runic_smash'),
(62058, 'spell_thorim_runic_smash'),
(62184, 'spell_thorim_activate_lightning_orb_periodic'),
(62320, 'spell_thorim_aura_of_celerity'),
(62466, 'spell_thorim_lightning_charge'),
(62576, 'spell_thorim_blizzard_effect'),
(62580, 'spell_thorim_frostbolt_volley'),
(62602, 'spell_thorim_blizzard_effect'),
(62604, 'spell_thorim_frostbolt_volley'),
(64767, 'spell_thorim_stormhammer_sif'),
(64909, 'spell_thorim_stormhammer_boomerang');

-- Achievement criteria: Don't Stand in the Lightning, I'll Take You All On and Lose Your Illusion (10 and 25 player).
-- The old rows used the instance script data type, which this core does not load.
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10287, 10288, 10305, 10309, 10311, 10312, 10440, 10457);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10287, 11, 0, 0, 'achievement_i_ll_take_you_all_on'),
(10288, 11, 0, 0, 'achievement_i_ll_take_you_all_on'),
(10305, 11, 0, 0, 'achievement_dont_stand_in_the_lightning'),
(10309, 11, 0, 0, 'achievement_dont_stand_in_the_lightning'),
(10311, 11, 0, 0, 'achievement_i_ll_take_you_all_on'),
(10312, 11, 0, 0, 'achievement_i_ll_take_you_all_on'),
(10440, 11, 0, 0, 'achievement_lose_your_illusion'),
(10457, 11, 0, 0, 'achievement_lose_your_illusion');

-- Leap (effect 0) may only land on a Thorim Event Bunny at arena level.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 61934;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 61934, 0, 0, 31, 0, 3, 32892, 0, 0, 0, 0, 'condition_thorim_arena_leap', 'Spell Leap (effect 0) will hit the potential target of the spell if target is unit Thorim Event Bunny.');
