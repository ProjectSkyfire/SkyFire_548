-- Azjol-Nerub: Krik'thir the Gatewatcher, the three Watcher packs (TrinityCore 3.3.5 data)

-- Script bindings
UPDATE `creature_template` SET `ScriptName` = 'npc_skittering_swarmer' WHERE `entry` = 28735 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_gatewatcher_web_wrap' WHERE `entry` = 28619 AND `ScriptName` = '' AND `AIName` = '';

-- Krik'thir (normal and heroic) is immune to the same mechanics as in TrinityCore
UPDATE `creature_template` SET `mechanic_immune_mask` = 617298807 WHERE `entry` IN (28684, 31612);

-- The three Watcher packs are summoned by Krik'thir (creature_summon_groups) instead of standing as static spawns
DELETE FROM `creature` WHERE `guid` IN (127225, 127226, 127227, 127228, 127231, 127232, 127233, 127234, 127236);
DELETE FROM `creature_addon` WHERE `guid` IN (127225, 127226, 127227, 127228, 127231, 127232, 127233, 127234, 127236);

DELETE FROM `creature_summon_groups` WHERE `summonerId` = 28684 AND `summonerType` = 0 AND `groupId` IN (1, 2, 3);
INSERT INTO `creature_summon_groups` (`summonerId`, `summonerType`, `groupId`, `entry`, `position_x`, `position_y`, `position_z`, `orientation`, `summonType`, `summonTime`) VALUES
(28684, 0, 1, 28729, 511.81, 666.493, 776.278, 0.977384, 6, 45000),
(28684, 0, 1, 28732, 506.516, 664.38, 776.976, 0.925025, 6, 30000),
(28684, 0, 1, 28733, 506.178, 669.927, 776.306, 0.855211, 6, 30000),
(28684, 0, 2, 28730, 526.664, 663.605, 775.805, 1.23918, 6, 45000),
(28684, 0, 2, 28732, 531.031, 658.173, 776.24, 1.3439, 6, 30000),
(28684, 0, 2, 28734, 521.815, 659.471, 776.313, 1.18682, 6, 30000),
(28684, 0, 3, 28731, 543.826, 665.123, 776.245, 1.55334, 6, 45000),
(28684, 0, 3, 28734, 550.046, 668.131, 776.279, 1.65806, 6, 30000),
(28684, 0, 3, 28733, 549.693, 662.007, 776.993, 1.64061, 6, 30000);

-- Subboss Aggro Trigger picks a Watcher; the swarm spells summon at the world triggers around the room
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (52343, 52438, 52449);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 52343, 0, 0, 31, 0, 3, 28729, 0, 0, 0, 0, '', 'Krik\'Thir Subboss Aggro Trigger - Target Watcher Narjil'),
(13, 1, 52343, 0, 1, 31, 0, 3, 28730, 0, 0, 0, 0, '', 'Krik\'Thir Subboss Aggro Trigger - Target Watcher Gashra'),
(13, 1, 52343, 0, 2, 31, 0, 3, 28731, 0, 0, 0, 0, '', 'Krik\'Thir Subboss Aggro Trigger - Target Watcher Silthik'),
(13, 1, 52438, 0, 0, 31, 0, 3, 22515, 0, 0, 0, 0, '', 'Krik\'thir swarm (52438) - Target World Trigger'),
(13, 1, 52449, 0, 0, 31, 0, 3, 22515, 0, 0, 0, 0, '', 'Krik\'thir swarm (52449) - Target World Trigger');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (52343, 52536, 52086) AND `ScriptName` IN ('spell_gatewatcher_subboss_trigger', 'spell_anub_ar_skirmisher_fixate', 'spell_gatewatcher_web_wrap');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(52343, 'spell_gatewatcher_subboss_trigger'),
(52536, 'spell_anub_ar_skirmisher_fixate'),
(52086, 'spell_gatewatcher_web_wrap');
