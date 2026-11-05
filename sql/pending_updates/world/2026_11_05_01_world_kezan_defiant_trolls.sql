-- Defiant Trolls take the Goblin All-In-1-Der Belt Shocker by spellclick, and only during Good Help is Hard to Find
-- (14069). They had a questgiver flag with no quests, and no texts for the line they say when shocked.
UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 34830;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 34830 AND `SourceEntry` = 66306;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 34830, 66306, 0, 0, 9, 0, 14069, 0, 0, 0, 0, 0, '', 'Defiant Troll - Belt Shocker only during Good Help is Hard to Find');

DELETE FROM `creature_text` WHERE `entry` = 34830;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34830, 0, 0, 'Don''t tase me, mon!', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 1, 'I report you to HR!', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 2, 'I''m going. I''m going!', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 3, 'Oops, break''s over.', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 4, 'Sorry, mon. It won''t happen again.', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 5, 'What I doin'' wrong? Don''t I get a lunch and two breaks a day, mon?', 12, 0, 100, 0, 0, 0, 'Defiant Troll'),
(34830, 0, 6, 'Work was bettah in da Undermine!', 12, 0, 100, 0, 0, 0, 'Defiant Troll');
