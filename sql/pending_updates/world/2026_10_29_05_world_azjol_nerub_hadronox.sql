-- Azjol-Nerub: Hadronox encounter (TrinityCore 3.3.5 data)

-- Script bindings: the crusher packs and the Anub'ar sent against Hadronox
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_crusher' WHERE `entry` = 28922 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_crusher_champion' WHERE `entry` = 29117 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_crusher_crypt_fiend' WHERE `entry` = 29118 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_crusher_necromancer' WHERE `entry` = 29119 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_champion' WHERE `entry` IN (29062, 29096) AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_crypt_fiend' WHERE `entry` IN (29063, 29097) AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anub_ar_necromancer' WHERE `entry` IN (29064, 29098) AND `ScriptName` = '' AND `AIName` = '';

-- Hadronox (normal and heroic) is immune to the same mechanics as in TrinityCore
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853239 WHERE `entry` IN (28921, 31611);

-- Hadronox starts at the bottom of the tunnel; the stray Rend aura is not part of the encounter
UPDATE `creature` SET `position_x` = 515.585, `position_y` = 544.201, `position_z` = 673.627, `orientation` = 5.74029 WHERE `guid` = 127401;
UPDATE `creature_addon` SET `auras` = NULL WHERE `guid` = 127401;

-- The first crusher pack is summoned by Hadronox now (creature_summon_groups), drop the static copy
DELETE FROM `creature` WHERE `guid` IN (127402, 127379, 127380);
DELETE FROM `creature_addon` WHERE `guid` IN (127402, 127379, 127380);

DELETE FROM `creature_summon_groups` WHERE `summonerId` = 28921 AND `summonerType` = 0 AND `groupId` IN (1, 2, 3);
INSERT INTO `creature_summon_groups` (`summonerId`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `summonType`, `summonTime`) VALUES
(28921, 0, 1, 28922, 529.691, 547.126, 731.916, 4.79965, 6, 45000),
(28921, 0, 1, 29117, 539.208, 549.754, 732.867, 4.55531, 6, 30000),
(28921, 0, 1, 29118, 520.391, 548.789, 732.012, 5.0091, 6, 30000),
(28921, 0, 2, 28922, 493.477, 603.344, 760.563, 5.44024, 6, 45000),
(28921, 0, 2, 29117, 490.442, 604.335, 763.182, 5.6256, 6, 30000),
(28921, 0, 2, 29119, 488.825, 609.282, 767.588, 5.59029, 6, 30000),
(28921, 0, 3, 28922, 566.979, 602.571, 759.642, 3.88597, 6, 45000),
(28921, 0, 3, 29118, 569.348, 604.999, 763.214, 4.17983, 6, 30000),
(28921, 0, 3, 29119, 572.474, 607.411, 767.178, 3.94417, 6, 30000);

-- World triggers at the web doors keep sending Anub'ar down the tunnel until Hadronox webs the doors
UPDATE `creature` SET `position_x` = 480.033, `position_y` = 615.589, `position_z` = 773.353, `orientation` = 4.10152 WHERE `guid` = 127377;
UPDATE `creature_addon` SET `auras` = '53035 53036 53037' WHERE `guid` IN (127376, 127377, 127378);

-- Web Front Doors / Web Side Door hit the world triggers
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (53177, 53185);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 53177, 0, 0, 31, 0, 3, 23472, 0, 0, 0, 0, '', 'Web Front Doors - Target World Trigger (Large AOI, Not Immune PC/NPC)'),
(13, 1, 53185, 0, 0, 31, 0, 3, 23472, 0, 0, 0, 0, '', 'Web Side Door - Target World Trigger (Large AOI, Not Immune PC/NPC)');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (53030, 59417, 53035, 53036, 53037, 53177, 53185) AND `ScriptName` IN ('spell_hadronox_leeching_poison', 'spell_hadronox_periodic_summon', 'spell_hadronox_web_doors');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53030, 'spell_hadronox_leeching_poison'),
(59417, 'spell_hadronox_leeching_poison'),
(53035, 'spell_hadronox_periodic_summon'),
(53036, 'spell_hadronox_periodic_summon'),
(53037, 'spell_hadronox_periodic_summon'),
(53177, 'spell_hadronox_web_doors'),
(53185, 'spell_hadronox_web_doors');

-- Hadronox Denied (heroic only: the script checks the difficulty, this core has no criteria data type for it)
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 4244 AND `type` = 11;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(4244, 11, 0, 0, 'achievement_hadronox_denied');

DELETE FROM `creature_text` WHERE `entry` IN (28921, 28922);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(28921, 1, 0, '%s moves up the tunnel!', 41, 0, 100, 0, 0, 0, 'Hadronox - Moves up the tunnel'),
(28922, 1, 0, 'The gate has been breached! Quickly, divert forces to deal with these invaders!', 14, 0, 100, 0, 0, 13941, 'Anub\'ar Crusher - Aggro'),
(28922, 2, 0, '%s goes into a frenzy!', 41, 0, 100, 0, 0, 0, 'Anub\'ar Crusher - Frenzy');
