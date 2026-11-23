-- To the Surface (27674): the gnome who finished the Survivors chain asks Torben Zapblast (46293) to send them to the surface, and
-- his script (gossip option 0 of menu 12104) credits the objective and casts Teleport to Surface (86278). The option was stored
-- with the id 1 of the older data, so the script, which waits for option 0, never ran and the new gnome could not leave the
-- survivors' camp. The option now has the id 0 and, as in Legends of Azeroth 5.4.8, is shown only to a player who has the quest.

DELETE FROM `gossip_menu_option` WHERE `MenuID` = 12104;

INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(12104, 0, 0, 'Send me to the surface, Torben.', 46360, 1, 1, 0, 0, 0, 0, NULL, 0, 15595);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 12104;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12104, 0, 0, 0, 9, 0, 27674, 0, 0, 0, 0, 0, '', 'Torben Zapblast - Show option if the player has To the Surface');
