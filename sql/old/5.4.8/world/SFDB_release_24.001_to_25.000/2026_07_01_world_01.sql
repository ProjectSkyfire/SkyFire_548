-- =============================================================================
-- DUSKWOOD: quest 26720 A Curse We Cannot Lift (Lurking Worgen)
-- Duskwood chain part 2/3 (apply 2026-07-01_world_00 -> _01 -> _02 in order)
-- =============================================================================
-- Symptom:  Lurking Worgen (43814) shows skull level, no nameplate name, does not
--           pounce/aggro, Harris's Ampule has no effect.
-- Cause:    creature_template.unit_flags = 768 (IMMUNE_TO_PC | IMMUNE_TO_NPC),
--           type_flags = 2147483648 breaks 5.4.8 client creature query (skull/??),
--           no ScriptName / combat script binding.
-- Fix:      Clear immunity and type_flags, wire npc_lurking_potion C++ script,
--           spell script for Harris's Ampule, and hesitate emote text.
-- NPCs:     43814 Lurking Worgen, 43860 kill credit
-- Item:     60206 Harris's Ampule (spell 82058)
-- Spells:   81957 Stunning Pounce, 82058 Harris's Ampule
-- Apply:    SQL then rebuild worldserver (C++ script). .reload creature_template
--            on server; clear client cache once if level/name still wrong.
-- =============================================================================

-- Remove PC/NPC immunity; fix skull display; wire quest script.
UPDATE `creature_template`
SET `unit_flags` = 0,
    `unit_flags2` = 2048,
    `AIName` = '',
    `ScriptName` = 'npc_lurking_potion',
    `spell1` = 0,
    `type_flags` = 0,
    `InhabitType` = 3,
    `Health_mod` = 2
WHERE `entry` = 43814;

DELETE FROM `creature_template_addon` WHERE `entry` = 43814;
INSERT INTO `creature_template_addon`
    (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES
    (43814, 0, 0, 0, 1, 433, NULL);

-- type 41 = CHAT_MSG_RAID_BOSS_EMOTE (yellow center screen); not 12 (say) or 63 (white quest boss emote)
DELETE FROM `creature_text` WHERE `entry` = 43814;
INSERT INTO `creature_text`
    (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`)
VALUES
    (43814, 0, 0, 'The worgen stares and hesitates!', 41, 0, 100, 0, 0, 0, 'Lurking Worgen - quest 26720 yellow boss emote');

-- Harris's Ampule (82058): validation handled in spell_q26720_harris_ampule C++ script.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` IN (13, 17)
  AND `SourceEntry` = 82058;

DELETE FROM `spell_script_names` WHERE `spell_id` = 82058 AND `ScriptName` = 'spell_q26720_harris_ampule';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`)
VALUES
    (82058, 'spell_q26720_harris_ampule');
