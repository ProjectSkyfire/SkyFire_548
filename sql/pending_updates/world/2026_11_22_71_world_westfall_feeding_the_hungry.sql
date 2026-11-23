-- Feeding the Hungry and the Hopeless (26271) cannot be finished. The Westfall Stew (42617) summoned by the quest
-- item has no script, so the homeless nearby never walk to it, never eat and never give the quest credit (42617).
-- The stew script and the thanks the homeless say are as in the Cataclysm Preservation Project 4.3.4; the homeless
-- keep their script (npc_transient), which now handles the stew too, so only the stew gets a script name here.

UPDATE `creature_template` SET `ScriptName` = 'npc_westfall_westfall_stew' WHERE `entry` = 42617;

DELETE FROM `creature_text` WHERE `entry` IN (42383, 42384, 42386, 42391) AND `groupid` = 17;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42383, 17, 0, 'Thank you, kind and gentle stranger.', 12, 7, 100, 1, 0, 0, 'Transient to Westfall Stew'),
(42383, 17, 1, 'Westfall stew? I''ll never forget this moment!', 12, 7, 100, 5, 0, 0, 'Transient to Westfall Stew'),
(42383, 17, 2, 'I''d know that smell anywhere! Salma''s famous stew!', 12, 7, 100, 5, 0, 0, 'Transient to Westfall Stew'),
(42383, 17, 3, 'Maybe... maybe life is worth living.', 12, 7, 100, 5, 0, 0, 'Transient to Westfall Stew'),
(42383, 17, 4, 'Bless you, friend.', 12, 7, 100, 1, 0, 0, 'Transient to Westfall Stew'),
(42384, 17, 0, 'Thank you, kind and gentle stranger.', 12, 7, 100, 1, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42384, 17, 1, 'Westfall stew? I''ll never forget this moment!', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42384, 17, 2, 'I''d know that smell anywhere! Salma''s famous stew!', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42384, 17, 3, 'Maybe... maybe life is worth living.', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42384, 17, 4, 'Bless you, friend.', 12, 7, 100, 1, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42386, 17, 0, 'Thank you, kind and gentle stranger.', 12, 7, 100, 1, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42386, 17, 1, 'Westfall stew? I''ll never forget this moment!', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42386, 17, 2, 'I''d know that smell anywhere! Salma''s famous stew!', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42386, 17, 3, 'Maybe... maybe life is worth living.', 12, 7, 100, 5, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42386, 17, 4, 'Bless you, friend.', 12, 7, 100, 1, 0, 0, 'Homeless Stormwind Citizen to Westfall Stew'),
(42391, 17, 0, 'Thank you, kind and gentle stranger.', 12, 7, 100, 1, 0, 0, 'West Plains Drifter to Westfall Stew'),
(42391, 17, 1, 'Westfall stew? I''ll never forget this moment!', 12, 7, 100, 5, 0, 0, 'West Plains Drifter to Westfall Stew'),
(42391, 17, 2, 'I''d know that smell anywhere! Salma''s famous stew!', 12, 7, 100, 5, 0, 0, 'West Plains Drifter to Westfall Stew'),
(42391, 17, 3, 'Maybe... maybe life is worth living.', 12, 7, 100, 5, 0, 0, 'West Plains Drifter to Westfall Stew'),
(42391, 17, 4, 'Bless you, friend.', 12, 7, 100, 1, 0, 0, 'West Plains Drifter to Westfall Stew');
