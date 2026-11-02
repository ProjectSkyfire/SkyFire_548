-- Restore progress text for Leave No Tracks.
UPDATE `quest_template` SET `RequestItemsText`='Have you completed your scouting mission, $N?' WHERE `Id`=13892 AND (`RequestItemsText` IS NULL OR `RequestItemsText`='');
