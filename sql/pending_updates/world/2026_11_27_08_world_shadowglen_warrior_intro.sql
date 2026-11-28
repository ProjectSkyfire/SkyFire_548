-- Correct the Shadowglen warrior introduction without removing the dwarf quest.
-- Simple Sigil is the night elf warrior introduction; Simple Rune is not.
DELETE FROM `creature_queststarter` WHERE `id` IN (2077,2079) AND `quest` = 3106;
UPDATE `quest_template` SET `NextQuestId` = 0, `NextQuestIdChain` = 0
WHERE `Id` = 3106 AND `NextQuestId` = 26945 AND `NextQuestIdChain` = 26945;
UPDATE `quest_template` SET `NextQuestId` = 26945, `NextQuestIdChain` = 26945
WHERE `Id` = 3116;
UPDATE `quest_template` SET `PrevQuestId` = 3116
WHERE `Id` = 26945;
