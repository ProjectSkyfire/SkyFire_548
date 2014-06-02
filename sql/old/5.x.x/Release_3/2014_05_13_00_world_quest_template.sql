-- Updates for quests 28757, 28762, 28763, 28764, 28765, 28766, 28767.
-- Beating them back.

UPDATE quest_template SET RequiredRaces = 2098253 WHERE Id IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
UPDATE quest_template SET RequiredClasses = 128 WHERE Id = 28757;
UPDATE quest_template SET RequiredClasses = 2 WHERE Id = 28762;
UPDATE quest_template SET RequiredClasses = 16 WHERE Id = 28763;
UPDATE quest_template SET RequiredClasses = 8 WHERE Id = 28764;
UPDATE quest_template SET RequiredClasses = 256 WHERE Id = 28765;
UPDATE quest_template SET RequiredClasses = 1 WHERE Id = 28766;
UPDATE quest_template SET RequiredClasses = 4 WHERE Id = 28767;

DELETE FROM creature_queststarter WHERE id = 197 AND quest IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
INSERT INTO `creature_queststarter` (`id` , `quest`) VALUES
(197, 28757),
(197, 28762),
(197, 28763),
(197, 28764),
(197, 28765),
(197, 28766),
(197, 28767);

DELETE FROM creature_questender WHERE id = 197 AND quest IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
INSERT INTO `creature_questender` (`id` , `quest`) VALUES
(197, 28757),
(197, 28762),
(197, 28763),
(197, 28764),
(197, 28765),
(197, 28766),
(197, 28767);

DELETE FROM creature_queststarter WHERE id = 49871;
DELETE FROM creature_questender WHERE id = 49871;

UPDATE quest_template SET OfferRewardText = 'You\'ve bought us a little time, $N, but we\'ve got even bigger problems to deal with now.' WHERE Id IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
UPDATE quest_template SET CompletedText = 'Return to Marshal McBride at Northshire Abbey in Elwynn Forest.' WHERE Id IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
UPDATE quest_template SET RequestItemsText = 'Have you beaten back the Blackrock battle worgs?' WHERE Id IN(28757, 28762, 28763, 28764, 28765, 28766, 28767);
