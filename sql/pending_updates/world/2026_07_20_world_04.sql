DELETE FROM `gameobject_loot_template` WHERE `entry`=41305;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(41305, 78959, -100), -- Serpent Egg
(41305, 79067, -100); -- Stolen Egg

-- Egg Collection
UPDATE `quest_template` SET `RequestItemsText`='The saurok keep the stolen serpent eggs in their camp.' WHERE `Id`=30137;
UPDATE `quest_template` SET `OfferRewardText`='Oh, wonderful!\r\n\r\nThese eggs look...well...they look a little on the small side. Are you sure you couldn\'t find any better ones?\r\n\r\nOh well, I suppose with the current situation, we can\'t be so picky.' WHERE `Id`=30137;

-- Preservation
UPDATE `quest_template` SET `OfferRewardText`='Will those nasty Slitherscale saurok never give up?' WHERE `Id`=30159;
UPDATE `quest_template` SET `EndText`='Well done, $n. As soon as Suchi told me she sent you to recover the stolen eggs, I knew you would accomplish the task.\r\n\r\nOur instructors will make sure to return these eggs to a safer place.' WHERE `Id`=30159;
