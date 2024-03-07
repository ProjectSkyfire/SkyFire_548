DELETE FROM `creature_text` WHERE `entry`=448;

INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(448, 0, 0, 'Hogger is eating! Stop him!', 41, 0, 100, 0, 0, 0, 'Hogger WARNING_EATING');
(448, 1, 0, 'Yipe! Help $n!', 14, 0, 100, 0, 0, 0, 'Hogger YELL_YIPE');
(448, 2, 0, 'Grrr...', 12, 0, 100, 0, 0, 0, 'Hogger SAY_GRR');
(448, 3, 0, 'Nooooo...', 12, 0, 100, 0, 0, 0, 'Hogger SAY_NO');
(448, 4, 0, 'No hurt $n!', 14, 0, 100, 0, 0, 0, 'Hogger YELL_NO_HURT');
