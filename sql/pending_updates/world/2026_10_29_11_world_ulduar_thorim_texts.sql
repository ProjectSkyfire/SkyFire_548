-- Ulduar: Thorim, Runic Colossus, Ancient Rune Giant and Sif texts. The Thorim groups are renumbered to match the script.
DELETE FROM `creature_text` WHERE `entry` IN (32865, 32872, 32873, 33196);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32865, 0, 0, 'Interlopers! You mortals who dare to interfere with my sport will pay.... Wait--you...', 14, 0, 100, 0, 0, 15733, 'Thorim SAY_AGGRO_1'),
(32865, 1, 0, 'I remember you... in the mountains.... But you... what is this? Where am--', 14, 0, 100, 0, 0, 15734, 'Thorim SAY_AGGRO_2'),
(32865, 2, 0, 'Do not hold back! Destroy them!', 14, 0, 100, 0, 0, 15736, 'Thorim SAY_SPECIAL'),
(32865, 3, 0, 'Impertinent whelps, you dare challenge me atop my pedestal? I will crush you myself!', 14, 0, 100, 0, 0, 15738, 'Thorim SAY_JUMPDOWN'),
(32865, 4, 0, 'Can''t you at least put up a fight?', 14, 0, 100, 0, 0, 15739, 'Thorim SAY_SLAY_1'),
(32865, 4, 1, 'Pathetic.', 14, 0, 100, 0, 0, 15740, 'Thorim SAY_SLAY_2'),
(32865, 5, 0, 'My patience has reached its limit!', 14, 0, 100, 0, 0, 15741, 'Thorim SAY_BERSERK'),
(32865, 6, 0, 'Failures!  Weaklings!', 14, 0, 100, 0, 0, 15742, 'Thorim SAY_WIPE'),
(32865, 7, 0, 'Stay your arms! I yield!', 14, 0, 100, 0, 0, 15743, 'Thorim SAY_DEATH'),
(32865, 8, 0, 'I feel as though I am awakening from a nightmare, but the shadows in this place yet linger.', 14, 0, 100, 0, 0, 15744, 'Thorim SAY_END_NORMAL_1'),
(32865, 9, 0, 'Sif... was Sif here? Impossible--she died at my brother''s hand. A dark nightmare indeed....', 14, 0, 100, 0, 0, 15745, 'Thorim SAY_END_NORMAL_2'),
(32865, 10, 0, 'I need time to reflect.... I will aid your cause if you should require it. I owe you at least that much. Farewell.', 14, 0, 100, 0, 0, 15746, 'Thorim SAY_END_NORMAL_3'),
(32865, 11, 0, 'You! Fiend, you are not my beloved! Begone!', 14, 0, 100, 0, 0, 15747, 'Thorim SAY_END_HARD_1'),
(32865, 12, 0, 'Behold the hand behind all the evil that has befallen Ulduar, left my kingdom in ruins, corrupted my brother, and slain my wife.', 14, 0, 100, 0, 0, 15748, 'Thorim SAY_END_HARD_2'),
(32865, 13, 0, 'And now it falls to you, champions, to avenge us all. The task before you is great, but I will lend you my aid as I am able. You must prevail.', 14, 0, 100, 0, 0, 15749, 'Thorim SAY_END_HARD_3'),
(32872, 0, 0, '%s surrounds itself with a crackling Runic Barrier!', 41, 0, 100, 0, 0, 0, 'Runic Colossus EMOTE_RUNIC_BARRIER'),
(32873, 0, 0, '%s fortifies nearby allies with runic might!', 41, 0, 100, 0, 0, 0, 'Ancient Rune Giant EMOTE_RUNIC_MIGHT'),
(33196, 0, 0, 'Thorim, my lord!  Why else would these invaders have come into your sanctum but to slay you?  They must be stopped!', 14, 0, 100, 0, 0, 15668, 'Sif SAY_SIF_START'),
(33196, 1, 0, 'These pathetic mortals are harmless, beneath my station.  Dispose of them!', 14, 0, 100, 0, 0, 15669, 'Sif SAY_SIF_DESPAWN'),
(33196, 2, 0, 'Impossible!  Lord Thorim, I will bring your foes a frigid death!', 14, 0, 100, 0, 0, 15670, 'Sif SAY_SIF_EVENT');
