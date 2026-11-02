-- Quest 13527: Decomposing Thistle Bear (32975) is collected via spellclick, not corpse loot.
-- Spell 62113 "Force Foul Bear Carcass Sample" creates item 44911.
-- Also restore missing OfferRewardText (completion dialogue was blank).
UPDATE `creature_template` SET
    `npcflag` = (`npcflag` | 16777216),
    `lootid` = 0,
    `dynamicflags` = 32
WHERE `entry` = 32975;

DELETE FROM `creature_loot_template` WHERE `entry` = 32975;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 32975;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(32975, 62113, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 32975 AND `SourceEntry` = 62113;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
(18, 32975, 62113, 0, 0, 9, 0, 13527, 0, 0, 0, 0, 0, '', 'Decomposing Thistle Bear spellclick requires quest 13527 taken'),
(18, 32975, 62113, 0, 0, 2, 0, 44911, 1, 0, 1, 0, 0, '', 'Decomposing Thistle Bear spellclick hidden if sample already held');

UPDATE `quest_template` SET
    `OfferRewardText` = 'This definitely looks like the stuff they were eating. Let me take a closer peek...'
WHERE `Id` = 13527;
