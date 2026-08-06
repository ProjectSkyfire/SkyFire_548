-- Wild Mushroom: place, Bloom, Detonate, Fungal Growth, and related NPC/proc setup.
DELETE FROM `spell_script_names` WHERE `ScriptName` IN (
    'spell_dru_wild_mushroom',
    'spell_dru_wild_mushroom_overheal',
    'spell_dru_wild_mushroom_bloom',
    'spell_dru_wild_mushroom_heal',
    'spell_dru_wild_mushroom_detonate'
);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(88747, 'spell_dru_wild_mushroom'),
(88751, 'spell_dru_wild_mushroom_detonate'),
(102791, 'spell_dru_wild_mushroom_bloom'),
(102792, 'spell_dru_wild_mushroom_heal'),
(138611, 'spell_dru_wild_mushroom_overheal'),
(145205, 'spell_dru_wild_mushroom'),
(147349, 'spell_dru_wild_mushroom');

-- Wild Mushroom NPC: rooted/immobile, scripted AI, LoA level baseline
-- (script also sets caster level on summon). unit_flags: PVP_ATTACKABLE|RENAME|IMMUNE_TO_NPC|DISABLE_MOVE
UPDATE `creature_template` SET
    `minlevel` = 85,
    `maxlevel` = 85,
    `unit_flags` = 540,
    `ScriptName` = 'npc_wild_mushroom'
WHERE `entry` = 47649;

-- Fungal Growth II: not selectable; applies slow aura/visual via script.
UPDATE `creature_template` SET
    `unit_flags` = 33554456,
    `ScriptName` = 'npc_fungal_growth'
WHERE `entry` = 43484;

-- Only Rejuvenation overhealing feeds the mushroom accumulator.
-- SpellFamilyMask0 0x10 = Rejuvenation; procFlags = periodic + positive magic heals.
DELETE FROM `spell_proc_event` WHERE `entry` = 138611;
INSERT INTO `spell_proc_event`
(`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`)
VALUES
(138611, 0, 7, 16, 0, 0, 0, 0x00044000, 0, 0, 0, 0);
