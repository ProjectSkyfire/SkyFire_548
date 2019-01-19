-- Quest Special Flags Errors
-- Add SpecialFlags Repeatable to Daily Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|4096 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Repeatable Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|8192 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Weekly Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|32768 AND SpecialFlags=0;
