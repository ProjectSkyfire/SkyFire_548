-- Stone Cold (14165) and Refleshification (14194): the Mountainfoot Miners (35257, 32 spawns) can be clicked while the
-- quest is taken: spell 67032 (Lugging a Goblin) for Stone Cold, spell 67362 (Refleshification) for Refleshification.
-- The miner script reacting to both spells was already here; the click flag, the click rows and their conditions were
-- missing. As in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (35257);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (35257);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(35257, 67032, 1, 0),
(35257, 67362, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 35257 AND `SourceEntry` = 67032;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 35257 AND `SourceEntry` = 67362;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 35257, 67032, 0, 0, 9, 0, 14165, 0, 0, 0, 0, 0, '', 'Mountainfoot Miner - only with Stone Cold'),
(18, 35257, 67362, 0, 0, 9, 0, 14194, 0, 0, 0, 0, 0, '', 'Mountainfoot Miner - only with Refleshification');
