-- The Truth of the Grave (24961): Lilian Voss (38910) uses the gossip menu 11133 and its only option, "You're not
-- hideous, Lilian... you're one of us. Here, look in this mirror", showed to every player. As in Legends of Azeroth 5.4.8
-- (which has the same option on menu 11132) it is shown only to players who have the quest.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11133 AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11133, 0, 0, 0, 9, 0, 24961, 0, 0, 0, 0, 0, '', 'Show the gossip option if the player has The Truth of the Grave');
