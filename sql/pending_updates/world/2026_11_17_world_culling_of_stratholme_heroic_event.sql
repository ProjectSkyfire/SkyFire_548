-- The Culling of Stratholme heroic: Chromie's three warnings about the Guardian of Time and the channel of the
-- Infinite Corruptor on the Guardian (the spell may only hit the Guardian of Time).
-- Safe to run twice: every row is deleted by its key before it is inserted.
DELETE FROM `creature_text` WHERE `entry`=26527 AND `groupid` IN (0, 1, 2);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(26527, 0, 0, 'Oh, no! Adventurers, something awful has happened! A colleague of mine has been captured by the Infinite Dragonflight, and they''re doing something horrible to him! Keeping Arthas is still your highest priority, but if you act fast you could help save a Guardian of Time!', 15, 0, 100, 0, 0, 0, 'Chromie - Guardian of Time captured'),
(26527, 1, 0, 'Adventurers, you must hurry! The Guardian of Time cannot last for much longer!', 15, 0, 100, 0, 0, 0, 'Chromie - Guardian of Time hurry'),
(26527, 2, 0, 'I can barely sense the Guardian of Time! His timeline is fading quickly!', 15, 0, 100, 0, 0, 0, 'Chromie - Guardian of Time fading');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=60422;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 60422, 0, 0, 31, 0, 3, 32281, 0, 0, 0, 0, '', 'Corruption of Time can only hit the Guardian of Time');
