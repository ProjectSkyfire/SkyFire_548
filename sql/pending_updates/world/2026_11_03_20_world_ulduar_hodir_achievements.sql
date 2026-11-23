-- Ulduar, Hodir: the criteria of Staying Buffed All Winter, Getting Cold in Here, I Have the Coolest Friends,
-- Cheese the Freeze and I Could Say That This Cache Was Rare (10 and 25 player) use the script checks of TrinityCore
-- 3.3.5 in place of an instance script check the Ulduar script does not answer.
-- TrinityCore also has a difficulty row (data type 12) for each; this core has no such data type, and the
-- criteria carry the difficulty themselves.
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10223, 10229, 10238, 10239, 10240, 10241, 10247, 10248, 10258, 10259, 10260, 10261, 10452, 10458) AND `type` IN (11, 18);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10223, 11, 0, 0, 'achievement_staying_buffed_all_winter'),
(10229, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10238, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10239, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10240, 11, 0, 0, 'achievement_staying_buffed_all_winter'),
(10241, 11, 0, 0, 'achievement_staying_buffed_all_winter'),
(10247, 11, 0, 0, 'achievement_getting_cold_in_here'),
(10248, 11, 0, 0, 'achievement_getting_cold_in_here'),
(10258, 11, 0, 0, 'achievement_i_have_the_coolest_friends'),
(10259, 11, 0, 0, 'achievement_cheese_the_freeze'),
(10260, 11, 0, 0, 'achievement_i_have_the_coolest_friends'),
(10261, 11, 0, 0, 'achievement_cheese_the_freeze'),
(10452, 11, 0, 0, 'achievement_this_cache_was_rare'),
(10458, 11, 0, 0, 'achievement_this_cache_was_rare');
