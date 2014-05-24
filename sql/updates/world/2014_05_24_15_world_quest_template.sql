-- Updates for quests 30039, 30040, 30041, 30042, 30043, 30044, 30045
-- Much to learn

UPDATE quest_template SET RequiredClasses = 512 WHERE Id = 30039; -- monk
UPDATE quest_template SET RequiredClasses = 128 WHERE Id = 30040; -- mage
UPDATE quest_template SET RequiredClasses = 4 WHERE Id = 30041; -- hunter
UPDATE quest_template SET RequiredClasses = 16 WHERE Id = 30042; -- priest
UPDATE quest_template SET RequiredClasses = 8 WHERE Id = 30043; -- rogue
UPDATE quest_template SET RequiredClasses = 64 WHERE Id = 30044; -- shaman
UPDATE quest_template SET RequiredClasses = 1 WHERE Id = 30045; -- warrior
