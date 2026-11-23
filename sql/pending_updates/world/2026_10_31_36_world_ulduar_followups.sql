-- Ulduar: Thorim's Stormhammer for Sif (64767) hit players too, and its script made every unit it hit take Sif's
-- form, which then stayed on the player. Restrict it to Sif (33196), as in TrinityCore 3.3.5.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 64767;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 64767, 0, 0, 31, 0, 3, 33196, 0, 0, 0, 0, '', 'Stormhammer (effect 0) only hits Sif');

-- Ulduar: the Winter Jormungar near Hodir could not be targeted (not selectable, immune to players);
-- TrinityCore 3.3.5 and Legends of Azeroth 5.4.8 both give them no unit flags.
UPDATE `creature_template` SET `unit_flags` = 0 WHERE `entry` = 34137;

-- Ulduar: the teleporters were not selectable (flags 48), so players could not reach the Antechamber or any other
-- unlocked stop; TrinityCore 3.3.5 gives them faction 35 and flags 32.
UPDATE `gameobject_template` SET `faction` = 35, `flags` = 32 WHERE `entry` = 194569;
