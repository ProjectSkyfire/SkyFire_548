-- Captives at Deatholme (9164): the option to give the restorative draught to Apprentice Varnis, Apothecary Enith and
-- Ranger Vedoran (menus 7185, 7182 and 7177) was shown to players who have the draught but not the quest, and Apothecary
-- Renzithen (menu 7187) still offered a sample to players who already carry one. As in Legends of Azeroth 5.4.8 the
-- captives' option needs the quest and Renzithen's option is hidden for players who have the draught (22628).

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (7177, 7182, 7185, 7187) AND `SourceEntry` = 0 AND `ConditionTypeOrReference` IN (2, 9);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 7177, 0, 0, 0, 2, 0, 22628, 1, 0, 0, 0, 0, '', 'Show the option if the player has the restorative draught'),
(15, 7177, 0, 0, 0, 9, 0, 9164, 0, 0, 0, 0, 0, '', 'Show the option if the player has Captives at Deatholme'),
(15, 7182, 0, 0, 0, 2, 0, 22628, 1, 0, 0, 0, 0, '', 'Show the option if the player has the restorative draught'),
(15, 7182, 0, 0, 0, 9, 0, 9164, 0, 0, 0, 0, 0, '', 'Show the option if the player has Captives at Deatholme'),
(15, 7185, 0, 0, 0, 2, 0, 22628, 1, 0, 0, 0, 0, '', 'Show the option if the player has the restorative draught'),
(15, 7185, 0, 0, 0, 9, 0, 9164, 0, 0, 0, 0, 0, '', 'Show the option if the player has Captives at Deatholme'),
(15, 7187, 0, 0, 0, 2, 0, 22628, 1, 0, 1, 0, 0, '', 'Hide the option if the player has the restorative draught'),
(15, 7187, 0, 0, 0, 9, 0, 9164, 0, 0, 0, 0, 0, '', 'Show the option if the player has Captives at Deatholme');
