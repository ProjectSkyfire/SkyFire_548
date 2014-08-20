-- Quest Special Flags Errors
-- Add SpecialFlags Repeatable to Daily Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|4096 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Repeatable Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|8192 AND SpecialFlags=0;
-- Add SpecialFlags Repeatable to Weekly Quests
UPDATE quest_template SET SpecialFlags=1 WHERE flags=flags|32768 AND SpecialFlags=0;

-- Spells Not Exists Anymore
UPDATE quest_template SET RewardSpellCast=0 WHERE RewardSpellCast IN (50574, 87477, 77462, 123076, 68316, 62095, 64689, 130087, 83854, 119486, 113342, 73663, 81040, 90372, 127688, 142290, 139948, 139855, 139583, 135036, 134716, 134304, 133438, 138844, 131410, 131400, 131398, 130241, 131432);
