-- Mount Hyjal quests that are finished by clicking a creature: Save the Wee Animals (25385, Panicked Bunny 39997 and
-- Terrified Squirrel 39998, spell 74618 creates the rescued animal item), Oh, Deer! (25392, Injured Fawn 39999, spell
-- 74664) and The Bears Up There (25462) with Those Bears Up There (29161, Hyjal Bear Cub 40240 and 52688, spell 101673).
-- The scripts that react to the click (credit, despawn, escort) were already here; the click flag, the click rows and
-- their quest conditions were missing, so the click did nothing. As in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (39997, 39998, 39999, 40240, 52688);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (39997, 39998, 39999, 40240, 52688);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(39997, 74618, 3, 0),
(39998, 74618, 3, 0),
(39999, 74664, 1, 0),
(40240, 101673, 1, 0),
(52688, 101673, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 39997 AND `SourceEntry` = 74618;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 39998 AND `SourceEntry` = 74618;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 39999 AND `SourceEntry` = 74664;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 40240 AND `SourceEntry` = 101673;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 52688 AND `SourceEntry` = 101673;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 39997, 74618, 0, 0, 9, 0, 25385, 0, 0, 0, 0, 0, '', 'Panicked Bunny - only with Save the Wee Animals'),
(18, 39998, 74618, 0, 0, 9, 0, 25385, 0, 0, 0, 0, 0, '', 'Terrified Squirrel - only with Save the Wee Animals'),
(18, 39999, 74664, 0, 0, 9, 0, 25392, 0, 0, 0, 0, 0, '', 'Injured Fawn - only with Oh, Deer!'),
(18, 40240, 101673, 0, 0, 9, 0, 25462, 0, 0, 0, 0, 0, '', 'Hyjal Bear Cub - only with The Bears Up There'),
(18, 40240, 101673, 0, 1, 9, 0, 29161, 0, 0, 0, 0, 0, '', 'Hyjal Bear Cub - only with Those Bears Up There'),
(18, 52688, 101673, 0, 0, 9, 0, 25462, 0, 0, 0, 0, 0, '', 'Hyjal Bear Cub - only with The Bears Up There'),
(18, 52688, 101673, 0, 1, 9, 0, 29161, 0, 0, 0, 0, 0, '', 'Hyjal Bear Cub - only with Those Bears Up There');
