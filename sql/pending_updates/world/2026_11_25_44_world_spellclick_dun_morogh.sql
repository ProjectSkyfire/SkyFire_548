-- A Fowl Shortage (29352) asks for 6 chickens. Clicking a Dun Morogh Chicken (53568, 69 spawns) with the quest taken
-- casts spell 99486 and spell 99487 (Pickup Chicken), which creates the chicken item (69982) for the player. The chickens
-- had no click flag, no click rows and no condition. As in Legends of Azeroth 5.4.8, without its script that removes the
-- chicken after the click.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (53568);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (53568);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(53568, 99486, 1, 0),
(53568, 99487, 2, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 53568 AND `SourceEntry` = 99486;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 53568 AND `SourceEntry` = 99487;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 53568, 99486, 0, 0, 9, 0, 29352, 0, 0, 0, 0, 0, '', 'Dun Morogh Chicken - only with A Fowl Shortage'),
(18, 53568, 99487, 0, 0, 9, 0, 29352, 0, 0, 0, 0, 0, '', 'Dun Morogh Chicken - only with A Fowl Shortage');
