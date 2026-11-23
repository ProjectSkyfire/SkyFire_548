-- Ulduar: Mimiron script bindings (creatures, button, spells, achievement criteria), the spell target
-- conditions and the linked spells the encounter relies on.

-- Creature scripts.
UPDATE `creature_template` SET `ScriptName` = 'boss_mimiron' WHERE `entry` = 33350;
UPDATE `creature_template` SET `ScriptName` = 'boss_leviathan_mk_ii' WHERE `entry` = 33432;
UPDATE `creature_template` SET `ScriptName` = 'boss_vx_001' WHERE `entry` = 33651;
UPDATE `creature_template` SET `ScriptName` = 'boss_aerial_command_unit' WHERE `entry` = 33670;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_assault_bot' WHERE `entry` = 34057;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_magnetic_core' WHERE `entry` = 34068;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_flames' WHERE `entry` IN (34121, 34363);
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_computer' WHERE `entry` = 34143;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_emergency_fire_bot' WHERE `entry` = 34147;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_frost_bomb' WHERE `entry` = 34149;
UPDATE `creature_template` SET `ScriptName` = 'npc_mimiron_proximity_mine' WHERE `entry` = 34362;

-- DO NOT PUSH THIS BUTTON!
UPDATE `gameobject_template` SET `ScriptName` = 'go_mimiron_hardmode_button' WHERE `entry` = 194739;

-- Spell scripts. The old proximity mine script on 63027 is replaced by the Mimiron one.
DELETE FROM `spell_script_names` WHERE `ScriptName` LIKE 'spell\_mimiron\_%' OR (`spell_id` = 63027 AND `ScriptName` = 'spell_ulduar_proximity_mines');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(63801, 'spell_mimiron_bomb_bot'),
(64619, 'spell_mimiron_clear_fires'),
(65192, 'spell_mimiron_clear_fires'),
(65224, 'spell_mimiron_clear_fires'),
(65354, 'spell_mimiron_clear_fires'),
(64463, 'spell_mimiron_despawn_assault_bots'),
(64618, 'spell_mimiron_fire_search'),
(64436, 'spell_mimiron_magnetic_core'),
(64444, 'spell_mimiron_magnetic_core_summon'),
(63667, 'spell_mimiron_napalm_shell'),
(63274, 'spell_mimiron_p3wx2_laser_barrage'),
(64542, 'spell_mimiron_plasma_blast'),
(63009, 'spell_mimiron_proximity_explosion'),
(66351, 'spell_mimiron_proximity_explosion'),
(63027, 'spell_mimiron_proximity_mines'),
(65346, 'spell_mimiron_proximity_trigger'),
(63382, 'spell_mimiron_rapid_burst'),
(64402, 'spell_mimiron_rocket_strike'),
(65034, 'spell_mimiron_rocket_strike'),
(63041, 'spell_mimiron_rocket_strike_damage'),
(63681, 'spell_mimiron_rocket_strike_target_select'),
(64383, 'spell_mimiron_self_repair'),
(63414, 'spell_mimiron_spinning_up'),
(64426, 'spell_mimiron_summon_assault_bot'),
(64425, 'spell_mimiron_summon_assault_bot_target'),
(64621, 'spell_mimiron_summon_fire_bot'),
(64620, 'spell_mimiron_summon_fire_bot_target'),
(64562, 'spell_mimiron_summon_flames_spread'),
(64623, 'spell_mimiron_summon_frost_bomb_target'),
(64398, 'spell_mimiron_summon_junk_bot'),
(63820, 'spell_mimiron_summon_junk_bot_target'),
(63339, 'spell_mimiron_weld');

-- Achievement criteria: Firefighter and Set Up Us the Bomb (mine, rocket, bomb bot), 10 and 25 player.
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10450, 10463, 10543, 10544, 10545, 10546, 10547, 10548);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10450, 11, 0, 0, 'achievement_firefighter'),
(10463, 11, 0, 0, 'achievement_firefighter'),
(10543, 11, 0, 0, 'achievement_setup_mine'),
(10544, 11, 0, 0, 'achievement_setup_rocket'),
(10545, 11, 0, 0, 'achievement_setup_boom'),
(10546, 11, 0, 0, 'achievement_setup_mine'),
(10547, 11, 0, 0, 'achievement_setup_rocket'),
(10548, 11, 0, 0, 'achievement_setup_boom');

-- Spell implicit target conditions (source type 13, SourceGroup is the effect mask).
-- Rocket Strike effect 2 names the Emergency Fire Bot as 34147, the reference has the Winter Jormungar 34137 there.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (63414, 64444, 64618, 64623, 63041, 64436);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 63414, 0, 0, 31, 0, 3, 33576, 0, 0, 0, 0, '', 'Spinning Up EFFECT_0 can only hit NPC_DB_TARGET'),
(13, 1, 64444, 0, 0, 31, 0, 3, 33670, 0, 0, 0, 0, '', 'Magnetic Core EFFECT_0 can only hit NPC_AERIAL_COMMAND_UNIT'),
(13, 1, 64618, 0, 0, 31, 0, 3, 34363, 0, 0, 0, 0, '', 'Fire Search EFFECT_0 can only hit NPC_FLAME'),
(13, 1, 64618, 0, 1, 31, 0, 3, 34121, 0, 0, 0, 0, '', 'Fire Search EFFECT_0 can only hit NPC_FLAME_SPREAD'),
(13, 1, 64623, 0, 0, 31, 0, 3, 34363, 0, 0, 0, 0, '', 'Frost Bomb EFFECT_0 can only hit NPC_FLAME'),
(13, 1, 64623, 0, 1, 31, 0, 3, 34121, 0, 0, 0, 0, '', 'Frost Bomb EFFECT_0 can only hit NPC_FLAME_SPREAD'),
(13, 2, 63041, 0, 0, 31, 0, 3, 34057, 0, 0, 0, 0, '', 'Rocket Strike EFFECT_1 can only hit NPC_ASSAULT_BOT'),
(13, 2, 63414, 0, 1, 31, 0, 3, 33432, 0, 0, 0, 0, '', 'Spinning Up EFFECT_1 can only hit NPC_LEVIATHAN_MKII'),
(13, 2, 64436, 0, 0, 31, 0, 3, 33670, 0, 0, 0, 0, '', 'Magnetic Core EFFECT_1 can only hit NPC_AERIAL_COMMAND_UNIT'),
(13, 4, 63041, 0, 1, 31, 0, 3, 34147, 0, 0, 0, 0, '', 'Rocket Strike EFFECT_2 can only hit NPC_EMERGENCY_FIRE_BOT'),
(13, 4, 63041, 0, 2, 31, 0, 3, 33855, 0, 0, 0, 0, '', 'Rocket Strike EFFECT_2 can only hit NPC_JUNK_BOT'),
(13, 4, 63041, 0, 3, 31, 0, 3, 33836, 0, 0, 0, 0, '', 'Rocket Strike EFFECT_2 can only hit NPC_BOMB_BOT');

-- Linked spells.
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (64567, 64570, 64626, 64627);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(64567, 64563, 1, 'Summon Flames Initial - Cast SPELL_SUMMON_FLAMES_INITIAL on hit targets'),
(64570, -64561, 1, 'Flame Suppressant - Remove SPELL_FLAMES_PERIODIC_TRIGGER from targets'),
(64626, -64624, 0, 'Explosion - Remove SPELL_FROST_BOMB_LINKED from caster'),
(64626, -64561, 1, 'Explosion - Remove SPELL_FLAMES_PERIODIC_TRIGGER from targets'),
(64627, -64561, 1, 'Frost Bomb - Remove SPELL_FLAMES_PERIODIC_TRIGGER from target');
