-- Dugiru (20604): restore Exodar cloth donation quests.
-- Gossip menu 8132 / text 10072 already present (no gossip options).
-- Quest 10358 Additional Runecloth requires 20 Runecloth on the repeatable turn-in.

DELETE FROM `creature_queststarter` WHERE `id` = 20604 AND `quest` IN (7792, 7798, 10356, 10357, 10358);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(20604, 7792),  -- A Donation of Wool
(20604, 7798),  -- A Donation of Silk
(20604, 10356), -- A Donation of Mageweave
(20604, 10357), -- A Donation of Runecloth
(20604, 10358); -- Additional Runecloth

DELETE FROM `creature_questender` WHERE `id` = 20604 AND `quest` IN (7792, 7798, 10356, 10357, 10358);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(20604, 7792),
(20604, 7798),
(20604, 10356),
(20604, 10357),
(20604, 10358);

DELETE FROM `quest_objective` WHERE `questId` = 10358;
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`) VALUES
(10358, 1035800, 0, 1, 14047, 20, 0, '');
