-- Feeling Crabby? (26177) asks for 6 canal crabs. Clicking a Canal Crab (42339) with the quest taken casts spell 78975
-- on the player, which creates the Canal Crab item (57175). The crabs had no click flag, no click row and no condition.
-- As in Legends of Azeroth 5.4.8, without its script that removes the crab after the click.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (42339);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (42339);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42339, 78975, 3, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 42339 AND `SourceEntry` = 78975;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 42339, 78975, 0, 0, 9, 0, 26177, 0, 0, 0, 0, 0, '', 'Canal Crab - only with Feeling Crabby?');
