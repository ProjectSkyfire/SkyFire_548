-- Mage spell scripts
DELETE FROM `spell_script_names` WHERE `spell_id` = 1463 AND `ScriptName` = 'spell_mage_mana_shield';

DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
'spell_mage_pyroblast_clearcasting', 'spell_mage_combustion', 'spell_mage_inferno_blast', 'spell_mage_inferno_blast_spread',
'spell_mage_flamestrike', 'spell_mage_pyromaniac', 'spell_mage_fingers_of_frost', 'spell_mage_fingers_of_frost_proc',
'spell_mage_icicles_periodic', 'spell_mage_mastery_icicles', 'spell_mage_ice_lance', 'spell_mage_deep_freeze',
'spell_mage_frostbolt', 'spell_mage_icy_veins_cooldown', 'spell_mage_frostfire_bolt', 'spell_mage_blizzard',
'spell_mage_frozen_orb_damage_trigger', 'spell_mage_frozen_orb_damage', 'spell_mage_frost_bomb_aura', 'spell_mage_frost_bomb',
'spell_mage_arcane_missiles_proc', 'spell_mage_arcane_missiles_proc_visual', 'spell_mage_arcane_missiles', 'spell_mage_arcane_barrage',
'spell_mage_arcane_charge', 'spell_mage_arcane_explosion', 'spell_mage_evocation', 'spell_mage_incanters_ward', 'spell_mage_incanters_ward_cooldown',
'spell_mage_rune_of_power', 'spell_mage_nether_tempest_selector', 'spell_mage_nether_tempest', 'spell_mage_invisibility',
'spell_mage_greater_invisibility', 'spell_mage_greater_invisibility_damage_reduction', 'spell_mage_temporal_shield', 'spell_mage_cauterize',
'spell_mage_flameglow', 'spell_mage_glyph_of_illusion', 'spell_mage_glyph_of_conjure_familiar', 'spell_mage_glyph_of_unbound_elemental',
'spell_mage_glyph_of_rapid_displacement', 'spell_mage_glyph_of_rapid_teleportation', 'spell_mage_glyph_of_armors', 'spell_mage_polymorph',
'spell_mage_dragons_breath', 'spell_mage_slow', 'spell_mage_t15_2p_bonus', 'spell_mage_t16_2p_bonus', 'spell_mage_t16_4p_bonus',
'spell_mage_glyph_of_slow', 'spell_mage_counterspell', 'spell_mage_alter_time', 'spell_mage_alter_time_cast', 'spell_mage_alter_time_overridden',
'spell_mage_ring_of_frost_overridden', 'spell_mage_glyph_of_icy_veins', 'spell_mage_glyph_of_icy_veins_damage', 'spell_mage_glyph_of_spellsteal',
'spell_mage_glyph_of_momentum', 'spell_mage_glyph_of_momentum_blink');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
-- Fire
(44448, 'spell_mage_pyroblast_clearcasting'),
(11129, 'spell_mage_combustion'),
(108853, 'spell_mage_inferno_blast'),
(118280, 'spell_mage_inferno_blast_spread'),
(2120, 'spell_mage_flamestrike'),
(44457, 'spell_mage_pyromaniac'),
(112948, 'spell_mage_pyromaniac'),
(114923, 'spell_mage_pyromaniac'),
-- Frost
(44544, 'spell_mage_fingers_of_frost'),
(116, 'spell_mage_fingers_of_frost_proc'),
(42208, 'spell_mage_fingers_of_frost_proc'),
(44614, 'spell_mage_fingers_of_frost_proc'),
(148023, 'spell_mage_icicles_periodic'),
(76613, 'spell_mage_mastery_icicles'),
(30455, 'spell_mage_ice_lance'),
(44572, 'spell_mage_deep_freeze'),
(116, 'spell_mage_frostbolt'),
(131079, 'spell_mage_frostbolt'),
(12472, 'spell_mage_icy_veins_cooldown'),
(131078, 'spell_mage_icy_veins_cooldown'),
(44614, 'spell_mage_frostfire_bolt'),
(42208, 'spell_mage_blizzard'),
(84718, 'spell_mage_frozen_orb_damage_trigger'),
(84721, 'spell_mage_frozen_orb_damage'),
(112948, 'spell_mage_frost_bomb_aura'),
(113092, 'spell_mage_frost_bomb'),
-- Arcane
(79684, 'spell_mage_arcane_missiles_proc'),
(79683, 'spell_mage_arcane_missiles_proc_visual'),
(5143, 'spell_mage_arcane_missiles'),
(44425, 'spell_mage_arcane_barrage'),
(36032, 'spell_mage_arcane_charge'),
(1449, 'spell_mage_arcane_explosion'),
(12051, 'spell_mage_evocation'),
-- Talents
(1463, 'spell_mage_incanters_ward'),
(118859, 'spell_mage_incanters_ward_cooldown'),
(116011, 'spell_mage_rune_of_power'),
(114924, 'spell_mage_nether_tempest_selector'),
(114923, 'spell_mage_nether_tempest'),
(32612, 'spell_mage_invisibility'),
(110960, 'spell_mage_greater_invisibility'),
(113862, 'spell_mage_greater_invisibility_damage_reduction'),
(115610, 'spell_mage_temporal_shield'),
(86949, 'spell_mage_cauterize'),
(140468, 'spell_mage_flameglow'),
-- Glyphs
(63092, 'spell_mage_glyph_of_illusion'),
(126748, 'spell_mage_glyph_of_conjure_familiar'),
(146976, 'spell_mage_glyph_of_unbound_elemental'),
(1953, 'spell_mage_glyph_of_rapid_displacement'),
(119415, 'spell_mage_glyph_of_rapid_displacement'),
(1953, 'spell_mage_glyph_of_momentum'),
(119415, 'spell_mage_glyph_of_momentum_blink'),
(3561, 'spell_mage_glyph_of_rapid_teleportation'),
(3562, 'spell_mage_glyph_of_rapid_teleportation'),
(3563, 'spell_mage_glyph_of_rapid_teleportation'),
(3565, 'spell_mage_glyph_of_rapid_teleportation'),
(3566, 'spell_mage_glyph_of_rapid_teleportation'),
(3567, 'spell_mage_glyph_of_rapid_teleportation'),
(32271, 'spell_mage_glyph_of_rapid_teleportation'),
(33690, 'spell_mage_glyph_of_rapid_teleportation'),
(35715, 'spell_mage_glyph_of_rapid_teleportation'),
(49358, 'spell_mage_glyph_of_rapid_teleportation'),
(49359, 'spell_mage_glyph_of_rapid_teleportation'),
(53140, 'spell_mage_glyph_of_rapid_teleportation'),
(88342, 'spell_mage_glyph_of_rapid_teleportation'),
(88344, 'spell_mage_glyph_of_rapid_teleportation'),
(121847, 'spell_mage_glyph_of_rapid_teleportation'),
(121848, 'spell_mage_glyph_of_rapid_teleportation'),
(121849, 'spell_mage_glyph_of_rapid_teleportation'),
(121850, 'spell_mage_glyph_of_rapid_teleportation'),
(121851, 'spell_mage_glyph_of_rapid_teleportation'),
(121852, 'spell_mage_glyph_of_rapid_teleportation'),
(121853, 'spell_mage_glyph_of_rapid_teleportation'),
(121854, 'spell_mage_glyph_of_rapid_teleportation'),
(121855, 'spell_mage_glyph_of_rapid_teleportation'),
(121856, 'spell_mage_glyph_of_rapid_teleportation'),
(121857, 'spell_mage_glyph_of_rapid_teleportation'),
(121858, 'spell_mage_glyph_of_rapid_teleportation'),
(121859, 'spell_mage_glyph_of_rapid_teleportation'),
(121860, 'spell_mage_glyph_of_rapid_teleportation'),
(121861, 'spell_mage_glyph_of_rapid_teleportation'),
(121862, 'spell_mage_glyph_of_rapid_teleportation'),
(128836, 'spell_mage_glyph_of_rapid_teleportation'),
(128838, 'spell_mage_glyph_of_rapid_teleportation'),
(98397, 'spell_mage_glyph_of_armors'),
(118, 'spell_mage_polymorph'),
(31661, 'spell_mage_dragons_breath'),
(31589, 'spell_mage_slow'),
-- Set bonuses
(110909, 'spell_mage_t15_2p_bonus'),
(48108, 'spell_mage_t16_2p_bonus'),
(57761, 'spell_mage_t16_2p_bonus'),
(145257, 'spell_mage_t16_4p_bonus'),
(2139, 'spell_mage_counterspell'),
-- Alter Time
(108978, 'spell_mage_alter_time_cast'),
(110909, 'spell_mage_alter_time'),
(127140, 'spell_mage_alter_time_overridden'),
-- Ring of Frost (Presence of Mind)
(140376, 'spell_mage_ring_of_frost_overridden'),
-- Glyph of Slow / Spellsteal / Icy Veins
(30451, 'spell_mage_glyph_of_slow'),
(30449, 'spell_mage_glyph_of_spellsteal'),
(116, 'spell_mage_glyph_of_icy_veins'),
(30455, 'spell_mage_glyph_of_icy_veins'),
(44614, 'spell_mage_glyph_of_icy_veins'),
(31707, 'spell_mage_glyph_of_icy_veins'),
(131079, 'spell_mage_glyph_of_icy_veins_damage'),
(131080, 'spell_mage_glyph_of_icy_veins_damage'),
(131081, 'spell_mage_glyph_of_icy_veins_damage'),
(131581, 'spell_mage_glyph_of_icy_veins_damage');

-- Mastery: Icicles stores Frostbolt / Frostfire Bolt hits; T16 2P (Arcane) triggers from Arcane Missiles
DELETE FROM `spell_proc_event` WHERE `entry` IN (76613, 145251);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(76613, 0, 3, 32, 4096, 0, 96, 0, 0, 0, 0, 0),
(145251, 0, 3, 2048, 0, 0, 0, 4096, 1, 0, 0, 0);

-- Incanter's Ward grants Incanter's Absorption while it is off cooldown
DELETE FROM `spell_learn_spell` WHERE `entry` = 1463 AND `SpellID` = 118858;
INSERT INTO `spell_learn_spell` (`entry`, `SpellID`, `Active`) VALUES
(1463, 118858, 1);

DELETE FROM `spell_linked_spell` WHERE (`spell_trigger`, `spell_effect`, `type`) IN ((-114003, -116257, 0), (45438, 41425, 0), (-70748, -70747, 0));
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-114003, -116257, 0, 'Invocation - Remove Invoker''s Energy'),
(45438, 41425, 0, 'Ice Block - Hypothermia'),
(-70748, -70747, 0, 'Item - Mage T10 4P Bonus - Remove buff');

-- Dalaran Brilliance does not stack with Arcane Brilliance
DELETE FROM `spell_group` WHERE `id` = 1086 AND `spell_id` = 61316;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1086, 61316);

DELETE FROM `spell_areatrigger_template` WHERE `Entry` = 304;
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(304, 0, 0, 8, 6, 6, 'sat_mage_rune_of_power');

UPDATE `creature_template` SET `ScriptName` = 'npc_pet_mage_frozen_orb' WHERE `entry` = 45322;
UPDATE `creature_template` SET `ScriptName` = 'npc_pet_mage_past_self' WHERE `entry` = 58542;
