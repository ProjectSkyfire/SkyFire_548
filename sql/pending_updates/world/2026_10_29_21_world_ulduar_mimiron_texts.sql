-- Ulduar: the Computer's self destruct announcements of the Mimiron hard mode.
DELETE FROM `creature_text` WHERE `entry` = 34143;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34143, 0, 0, 'Self-destruct sequence initiated.', 14, 0, 100, 0, 0, 15413, 'Computer SAY_SELF_DESTRUCT_INITIATED'),
(34143, 1, 0, 'Self-destruct sequence terminated. Override code A-9-0-5.', 14, 0, 100, 0, 0, 15414, 'Computer SAY_SELF_DESTRUCT_TERMINATED'),
(34143, 2, 0, 'This area will self-destruct in ten minutes.', 14, 0, 100, 0, 0, 15415, 'Computer SAY_SELF_DESTRUCT_10'),
(34143, 3, 0, 'This area will self-destruct in nine minutes.', 14, 0, 100, 0, 0, 15416, 'Computer SAY_SELF_DESTRUCT_9'),
(34143, 4, 0, 'This area will self-destruct in eight minutes.', 14, 0, 100, 0, 0, 15417, 'Computer SAY_SELF_DESTRUCT_8'),
(34143, 5, 0, 'This area will self-destruct in seven minutes.', 14, 0, 100, 0, 0, 15418, 'Computer SAY_SELF_DESTRUCT_7'),
(34143, 6, 0, 'This area will self-destruct in six minutes.', 14, 0, 100, 0, 0, 15419, 'Computer SAY_SELF_DESTRUCT_6'),
(34143, 7, 0, 'This area will self-destruct in five minutes.', 14, 0, 100, 0, 0, 15420, 'Computer SAY_SELF_DESTRUCT_5'),
(34143, 8, 0, 'This area will self-destruct in four minutes.', 14, 0, 100, 0, 0, 15421, 'Computer SAY_SELF_DESTRUCT_4'),
(34143, 9, 0, 'This area will self-destruct in three minutes.', 14, 0, 100, 0, 0, 15422, 'Computer SAY_SELF_DESTRUCT_3'),
(34143, 10, 0, 'This area will self-destruct in two minutes.', 14, 0, 100, 0, 0, 15423, 'Computer SAY_SELF_DESTRUCT_2'),
(34143, 11, 0, 'This area will self-destruct in one minute.', 14, 0, 100, 0, 0, 15424, 'Computer SAY_SELF_DESTRUCT_1'),
(34143, 12, 0, 'Self-destruct sequence finalized. Have a nice day!', 14, 0, 100, 0, 0, 15425, 'Computer SAY_SELF_DESTRUCT_FINALIZED');
