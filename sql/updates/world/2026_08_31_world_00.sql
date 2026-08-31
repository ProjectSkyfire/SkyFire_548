-- Northshire: restore McBride quest order after Lions for Lambs.
-- Correct order: Beating Them Back! -> Lions for Lambs -> class letter ->
-- class trainer quest -> Join the Battle!
-- Lions for Lambs was chaining straight into Join the Battle!, which also
-- treated Lions as a previous quest and skipped the letter/trainer steps.

-- Lions for Lambs -> class letter (auto-offer from McBride)
UPDATE `quest_template` SET `NextQuestId`=3104, `NextQuestIdChain`=3104 WHERE `Id`=28769; -- Mage -> Glyphic Letter
UPDATE `quest_template` SET `NextQuestId`=3101, `NextQuestIdChain`=3101 WHERE `Id`=28770; -- Paladin -> Consecrated Letter
UPDATE `quest_template` SET `NextQuestId`=3103, `NextQuestIdChain`=3103 WHERE `Id`=28771; -- Priest -> Hallowed Letter
UPDATE `quest_template` SET `NextQuestId`=3102, `NextQuestIdChain`=3102 WHERE `Id`=28772; -- Rogue -> Encrypted Letter
UPDATE `quest_template` SET `NextQuestId`=3105, `NextQuestIdChain`=3105 WHERE `Id`=28773; -- Warlock -> Tainted Letter
UPDATE `quest_template` SET `NextQuestId`=3100, `NextQuestIdChain`=3100 WHERE `Id`=28774; -- Warrior -> Simple Letter
UPDATE `quest_template` SET `NextQuestId`=26910, `NextQuestIdChain`=26910 WHERE `Id`=28759; -- Hunter -> Etched Letter
UPDATE `quest_template` SET `NextQuestId`=31141, `NextQuestIdChain`=31141 WHERE `Id`=31140; -- Monk -> Calligraphed Letter

-- Class letters -> trainer quests (auto-offer at the class trainer on letter turn-in)
UPDATE `quest_template` SET `NextQuestIdChain`=26913 WHERE `Id`=3100; -- Simple Letter -> Charging into Battle
UPDATE `quest_template` SET `NextQuestIdChain`=26918 WHERE `Id`=3101; -- Consecrated Letter -> The Power of the Light
UPDATE `quest_template` SET `NextQuestIdChain`=26915 WHERE `Id`=3102; -- Encrypted Letter -> The Deepest Cut
UPDATE `quest_template` SET `NextQuestIdChain`=26919 WHERE `Id`=3103; -- Hallowed Letter -> Learning the Word
UPDATE `quest_template` SET `NextQuestIdChain`=26916 WHERE `Id`=3104; -- Glyphic Letter -> Mastering the Arcane
UPDATE `quest_template` SET `NextQuestId`=26914, `NextQuestIdChain`=26914 WHERE `Id`=3105; -- Tainted Letter -> Corruption (was wrongly 26916)
UPDATE `quest_template` SET `NextQuestIdChain`=26917, `RequiredClasses`=4 WHERE `Id`=26910; -- Etched Letter -> The Hunter's Path
UPDATE `quest_template` SET `NextQuestIdChain`=31142 WHERE `Id`=31141; -- Calligraphed Letter -> Palm of the Tiger

-- Class trainer quests -> Join the Battle! (auto-offer at trainer)
UPDATE `quest_template` SET `NextQuestIdChain`=28789 WHERE `Id`=26913; -- Warrior
UPDATE `quest_template` SET `NextQuestIdChain`=28785 WHERE `Id`=26918; -- Paladin
UPDATE `quest_template` SET `NextQuestIdChain`=28787 WHERE `Id`=26915; -- Rogue
UPDATE `quest_template` SET `NextQuestIdChain`=28786 WHERE `Id`=26919; -- Priest
UPDATE `quest_template` SET `NextQuestIdChain`=28784 WHERE `Id`=26916; -- Mage
UPDATE `quest_template` SET `NextQuestIdChain`=28788 WHERE `Id`=26914; -- Warlock
UPDATE `quest_template` SET `NextQuestIdChain`=28780 WHERE `Id`=26917; -- Hunter
UPDATE `quest_template` SET `NextQuestIdChain`=31143 WHERE `Id`=31142; -- Monk
