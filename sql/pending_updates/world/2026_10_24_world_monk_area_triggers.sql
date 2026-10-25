-- Spell-created area triggers (SPELL_EFFECT_CREATE_AREATRIGGER / SPELL_AURA_AREA_TRIGGER)
CREATE TABLE IF NOT EXISTS `spell_areatrigger_template` (
  `Entry` int unsigned NOT NULL,
  `Flags` int unsigned NOT NULL DEFAULT '0',
  `CollisionType` int unsigned NOT NULL DEFAULT '0',
  `Radius` float NOT NULL DEFAULT '0',
  `ScaleX` float NOT NULL DEFAULT '0',
  `ScaleY` float NOT NULL DEFAULT '0',
  `ScriptName` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`Entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DELETE FROM `spell_areatrigger_template` WHERE `Entry` IN (228, 321, 336, 339, 373, 383, 718, 796);
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(228, 0, 0, 1, 0, 0, 'sat_monk_healing_sphere'),
(321, 0, 0, 1, 0, 0, 'sat_monk_gift_of_the_serpent'),
(336, 0, 0, 1, 0, 0, 'sat_monk_chi_sphere'),
(339, 0, 0, 0, 0, 0, 'sat_monk_chi_torpedo'),
(373, 0, 0, 1, 1, 1, 'sat_monk_gift_of_the_ox'),
(383, 0, 0, 3, 3, 3, 'sat_monk_afterlife_healing_sphere'),
(718, 0, 1, 4.5, 0, 0, 'sat_monk_ring_of_peace'),
(796, 0, 0, 1.5, 0, 0, 'sat_monk_energy_sphere');

-- Monk spell scripts
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
'spell_monk_ring_of_peace_aura', 'spell_monk_healing_sphere', 'spell_monk_gift_of_the_ox', 'spell_monk_summon_gift_of_the_ox',
'spell_monk_mastery_gift_of_the_serpent', 'spell_monk_gift_of_the_serpent_selector', 'spell_monk_gift_of_the_serpent_summon',
'spell_monk_t15_windwalker_2p_bonus', 'spell_monk_summon_energy_sphere', 'spell_monk_afterlife', 'spell_monk_sanctuary_of_the_ox',
'spell_monk_black_ox_statue_guard_selector', 'spell_monk_soothing_mist_statue', 'spell_monk_soothing_mist_statue_trigger',
'spell_monk_invoke_xuen', 'spell_monk_crackling_tiger_lightning', 'spell_monk_s_e_f', 'spell_monk_s_e_f_aura');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(140023, 'spell_monk_ring_of_peace_aura'),
(115460, 'spell_monk_healing_sphere'),
(124502, 'spell_monk_gift_of_the_ox'),
(124503, 'spell_monk_summon_gift_of_the_ox'),
(124506, 'spell_monk_summon_gift_of_the_ox'),
(117907, 'spell_monk_mastery_gift_of_the_serpent'),
(119036, 'spell_monk_gift_of_the_serpent_selector'),
(119031, 'spell_monk_gift_of_the_serpent_summon'),
(138177, 'spell_monk_t15_windwalker_2p_bonus'),
(138310, 'spell_monk_summon_energy_sphere'),
(140674, 'spell_monk_summon_energy_sphere'),
(116092, 'spell_monk_afterlife'),
(126119, 'spell_monk_sanctuary_of_the_ox'),
(118605, 'spell_monk_black_ox_statue_guard_selector'),
(125950, 'spell_monk_soothing_mist_statue'),
(125962, 'spell_monk_soothing_mist_statue_trigger'),
(123904, 'spell_monk_invoke_xuen'),
(123999, 'spell_monk_crackling_tiger_lightning'),
(137639, 'spell_monk_s_e_f'),
(137639, 'spell_monk_s_e_f_aura');

-- Ring of Peace procs on any hit result, Storm, Earth, and Fire only on hits
DELETE FROM `spell_proc_event` WHERE `entry` IN (140023, 137639);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(140023, 0, 0, 0, 0, 0, 0, 0, 65536, 0, 0, 0),
(137639, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0);

-- Statues and Xuen
UPDATE `creature_template` SET `ScriptName` = 'npc_black_ox_statue' WHERE `entry` = 61146;
UPDATE `creature_template` SET `spell1` = 0 WHERE `entry` = 60849;
UPDATE `creature_template` SET `ScriptName` = 'npc_monk_xuen' WHERE `entry` = 63508;

-- Storm, Earth, and Fire spirits
DELETE FROM `creature_model_info` WHERE `modelid` = 47726;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES
(47726, 0.5, 1, 2, 0);

DELETE FROM `creature_template` WHERE `entry` IN (69680, 69791, 69792);
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `speed_walk`, `speed_run`, `scale`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`, `type_flags`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RegenHealth`, `ScriptName`) VALUES
(69680, 11686, 'Storm Spirit', 90, 90, 0, 35, 35, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 7, 0, 3, 0.095, 1, 1, 1, 'npc_monk_s_e_f_spirit'),
(69791, 11686, 'Fire Spirit', 90, 90, 4, 7, 7, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 0, 7, 16777216, 3, 0.3, 1, 1, 1, 'npc_monk_s_e_f_spirit'),
(69792, 47726, 'Earth Spirit', 90, 90, 4, 14, 14, 1, 1.14286, 1, 1, 2000, 2000, 1, 0, 2048, 7, 16778240, 3, 0.3, 1, 1, 1, 'npc_monk_s_e_f_spirit');
