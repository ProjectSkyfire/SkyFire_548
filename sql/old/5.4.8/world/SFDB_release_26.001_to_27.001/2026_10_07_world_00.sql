-- Restore Unbagwa's summon event for Stranglethorn Fever (26597).
-- Reference: https://www.wowhead.com/mop-classic/quest=26598/the-heart-of-mokk
-- The Heart of Mokk is an immediate turn-in that starts the event. Require
-- the parent quest to be active and allow another attempt if the event fails.
UPDATE `quest_template`
SET `PrevQuestId` = -26597, `SpecialFlags` = `SpecialFlags` | 1
WHERE `Id` = 26598;

-- Each attempt consumes one Gorilla Fang. Use the quest-based fallback
-- objective ID convention for this missing objective (not a sniffed ID).
DELETE FROM `quest_objective` WHERE `questId` = 26598;
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`) VALUES
(26598, 2659800, 0, 1, 2799, 1, 0, '');

DELETE FROM `creature_queststarter` WHERE `id` = 1449 AND `quest` = 26598;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (1449, 26598);

-- Keep the existing summon sequence, but trigger it from the current quest
-- rather than the retired pre-Cataclysm quest (349).
UPDATE `smart_scripts`
SET `event_param1` = 26598,
    `comment` = 'Witch Doctor Unbagwa - On Quest The Heart of Mokk Rewarded - Start Action List'
WHERE `entryorguid` = 1449 AND `source_type` = 0 AND `id` = 0
    AND `event_type` = 20 AND `action_type` = 80 AND `action_param1` = 144900;
