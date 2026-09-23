-- The Slumbering Ancients (13895): require Soggoth and Kronn (13893) before accept
UPDATE `quest_template` SET `PrevQuestId`=13893 WHERE `Id`=13895;
