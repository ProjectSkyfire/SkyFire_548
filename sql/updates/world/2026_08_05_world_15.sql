-- Fix spell_script_names bindings that disagree with MoP 5.4.8 SpellEffect.dbc.
-- Startup was logging "did not match dbc effect data" / Validate() failures for these.

-- Glyph of Polymorph: script expects EFFECT_0 DUMMY (glyph 56375 only), not Polymorph forms.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mage_glyph_of_polymorph'
  AND `spell_id` IN (118, 28271, 28272, 61025, 61305, 61721, 61780);
INSERT IGNORE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(56375, 'spell_mage_glyph_of_polymorph');

-- Glyph of Ice Block: Ice Block 45438 is stun/immunity; glyph dummy is 115723.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mage_glyph_of_ice_block';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115723, 'spell_mage_glyph_of_ice_block');

-- Glyph of Innervate: dummy is on glyph 54832, not Innervate 29166.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_glyph_of_innervate';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(54832, 'spell_dru_glyph_of_innervate');

-- Sword and Board: talent aura 46953 (DUMMY), not Devastate 20243.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_sword_and_board';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(46953, 'spell_warr_sword_and_board');

-- Fortifying Brew: cast spell 115203 is DUMMY; 120954 is the resulting aura.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_monk_fortifying_brew';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(115203, 'spell_monk_fortifying_brew');

-- Zen Pilgrimage: teleport save is on 126892; 126895 is the return (separate script).
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_monk_zen_pilgrimage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(126892, 'spell_monk_zen_pilgrimage');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_monk_zen_pilgrimage_return';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(126895, 'spell_monk_zen_pilgrimage_return');

-- Ring of Frost: MoP periodic aura is 136511 (triggered by cast 113724).
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mage_ring_of_frost';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(136511, 'spell_mage_ring_of_frost');

-- Halion exit portals: 74805 in MoP DBC is a transform aura, not SUMMON_OBJECT_WILD.
-- WotLK used a custom spell_dbc override; until that is restored, unbind to silence the error.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_halion_summon_exit_portals';
