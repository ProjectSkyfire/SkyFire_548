-- Quest 13560 "An Ocean Not So Deep": decoy bot vehicle, kill credit, return teleport, console lock.
-- Foolhardy Adventurer (34207) vehicle from Decoy Bot Control Console (195006).
-- Objective: kill-credit bunny 32852 x50 via Greymist KillCredit1 (do NOT also KilledMonsterCredit in C++).

DELETE FROM `spell_script_names` WHERE `spell_id` = 64802 AND `ScriptName` = 'spell_q13560_murloc_bomb_control_end';
DELETE FROM `spell_target_position` WHERE `id` = 64802;

UPDATE `creature_template` SET `spell1` = 62684 WHERE `entry` = 34207 AND `spell1` = 0;
UPDATE `creature_template` SET `ScriptName` = 'npc_foolhardy_adventurer' WHERE `entry` = 34207 AND `ScriptName` <> 'npc_foolhardy_adventurer';

-- One credit per Greymist death via kill-credit bunny (quest 13560 objective ObjectId).
UPDATE `creature_template` SET `KillCredit1` = 32852, `KillCredit2` = 0 WHERE `entry` IN (33262, 33277);

-- groupid 0 = taunt, groupid 1 = imminent (Talk(0) / Talk(1) in script).
DELETE FROM `creature_text` WHERE `entry` = 34207;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34207, 0, 0, 'Murloc pests, I have come to steal your treasure! Attempt to stop me! Quickly!', 14, 0, 100, 0, 0, 0, 'Foolhardy Adventurer - depth charge taunt'),
(34207, 1, 0, 'Detonation imminent.', 14, 0, 100, 0, 0, 0, 'Foolhardy Adventurer - depth charge imminent');

-- Remove broken type-10 dummy objectives (e.g. sniff gap-fill id 1356000) that block completion after 50 kills.
-- Keep / restore only the real kill-credit objective.
DELETE FROM `quest_objective` WHERE `questId` = 13560 AND (`id` <> 256718 OR `type` = 10);
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`)
SELECT 13560, 256718, 0, 0, 32852, 50, 0, 'Scavenging Greymist Murlocs'
WHERE NOT EXISTS (SELECT 1 FROM `quest_objective` WHERE `id` = 256718);

-- Console usable only while quest 13560 is incomplete.
UPDATE `gameobject_template` SET `data1` = 13560 WHERE `entry` = 195006;

-- Spell 64924 is cast by a world trigger; ConditionTarget 1 = player spell target.
-- CONDITION_SOURCE_TYPE_SPELL=17, CONDITION_QUESTSTATE=47, state mask 8 = incomplete.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 64924 AND `ConditionTypeOrReference` = 47 AND `ConditionValue1` = 13560;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 64924, 0, 0, 47, 1, 13560, 8, 0, 0, 0, 0, '', 'Decoy Bot Control Console: require quest 13560 incomplete (player target)');
