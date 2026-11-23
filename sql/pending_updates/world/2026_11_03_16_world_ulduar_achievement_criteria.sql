-- Ulduar: Hot Pocket (10 and 25) and Not-So-Friendly Fire (10 and 25) asked the instance script for a check it never
-- answers, so the criteria could not be earned. TrinityCore has no instance script row for them; the difficulty is
-- part of the criteria in the client data.
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10405, 10406, 10430, 10431) AND `type` = 18;
