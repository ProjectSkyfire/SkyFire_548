-- Darkspear Hold: Vol'jin (38966) summons the Vision of Vol'jin and the Vision of Garrosh Hellscream himself when he
-- shows the vision after the class quests. The permanent copies stood on the brazier behind him all the time.
DELETE FROM `creature` WHERE `guid` IN (142447, 142448) AND `id` IN (38953, 38938);
