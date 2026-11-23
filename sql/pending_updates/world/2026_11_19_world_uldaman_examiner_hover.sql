-- High Examiner Tae'thelan Bloodwatcher in Uldaman carried the hover flag in his template and spawn addon rows, so he
-- floated above the floor. The 4.3.4 sniffs have no hover flag for this entry.
-- Ported from the Cataclysm Preservation Project 4.3.4.
UPDATE `creature_template_addon` SET `bytes1` = 0 WHERE `entry` = 46236;
UPDATE `creature_addon` SET `bytes1` = 0 WHERE `guid` = 9252121;
