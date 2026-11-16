-- The Arcatraz: bind the scripts of Dalliah the Doomsayer and Wrath-Scryer Soccothrates, which had none,
-- and add their lines (TrinityCore 3.3.5 creature_text, the groups the scripts use). Their deaths open the
-- two Containment Core Security Fields, so the instance could not be passed without them.
UPDATE `creature_template` SET `ScriptName` = 'boss_dalliah_the_doomsayer' WHERE `entry` = 20885 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'boss_wrath_scryer_soccothrates' WHERE `entry` = 20886 AND `ScriptName` = '';

DELETE FROM `creature_text` WHERE `entry` IN (20885, 20886);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(20885, 0, 0, 'Don''t worry about me; kill that worthless dullard instead!', 14, 0, 100, 1, 0, 11085, 'Dalliah the Doomsayer - Aggro Soccothrates First'),
(20885, 1, 0, 'It is unwise to anger me!', 14, 0, 100, 0, 0, 11086, 'Dalliah the Doomsayer - Aggro'),
(20885, 2, 0, 'Completely ineffective.  Just like someone else I know.', 14, 0, 100, 0, 0, 11087, 'Dalliah the Doomsayer - Kill'),
(20885, 2, 1, 'You chose the wrong opponent.', 14, 0, 100, 0, 0, 11088, 'Dalliah the Doomsayer - Kill'),
(20885, 3, 0, 'Reap the whirlwind!', 14, 0, 100, 0, 0, 11089, 'Dalliah the Doomsayer - Cast Whirlwind'),
(20885, 3, 1, 'I''ll cut you to pieces!', 14, 0, 100, 0, 0, 11090, 'Dalliah the Doomsayer - Cast Whirlwind'),
(20885, 4, 0, 'That is much better.', 14, 0, 100, 0, 0, 11091, 'Dalliah the Doomsayer - Cast Heal'),
(20885, 4, 1, 'Ahh... just what I needed.', 14, 0, 100, 0, 0, 11092, 'Dalliah the Doomsayer - Cast Heal'),
(20885, 5, 0, 'Now I''m really angry.', 14, 0, 100, 0, 0, 11093, 'Dalliah the Doomsayer - Death'),
(20885, 6, 0, 'More than you can handle, scryer?', 14, 0, 100, 1, 0, 11094, 'Dalliah the Doomsayer - Soccothrates at 25%'),
(20885, 6, 1, 'I suppose I''ll end up fighting them all myself.', 14, 0, 100, 1, 0, 11095, 'Dalliah the Doomsayer - Soccothrates at 25%'),
(20885, 6, 2, 'I''ve grown used to cleaning up your messes.', 14, 0, 100, 1, 0, 11096, 'Dalliah the Doomsayer - Soccothrates at 25%'),
(20885, 7, 0, 'Congratulations.  I''ve wanted to do that for years.', 14, 0, 100, 66, 0, 11097, 'Dalliah the Doomsayer - Soccothratess dies'),
(20886, 0, 0, 'Have you come to kill Dalliah?  Can I watch?', 14, 0, 100, 1, 0, 11237, 'Wrath-Scryer Soccothrates - Aggro Dalliah First'),
(20886, 1, 0, 'At last, a target for my frustrations!', 14, 0, 100, 0, 0, 11238, 'Wrath-Scryer Soccothrates - Aggro'),
(20886, 2, 0, 'Yes, that was quite satisfying.', 14, 0, 100, 0, 0, 11239, 'Wrath-Scryer Soccothrates - Kill'),
(20886, 3, 0, 'On guard!', 14, 0, 100, 0, 0, 11241, 'Wrath-Scryer Soccothrates - Knock Away'),
(20886, 3, 1, 'Defend yourself, for all the good it will do....', 14, 0, 100, 0, 0, 11242, 'Wrath-Scryer Soccothrates - Knock Away'),
(20886, 4, 0, 'Knew this was... the only way out.', 14, 0, 100, 0, 0, 11243, 'Wrath-Scryer Soccothrates - Death'),
(20886, 5, 0, 'Having problems, Dalliah?  How nice.', 14, 0, 100, 1, 0, 11244, 'Wrath-Scryer Soccothrates - Dalliah at 25%'),
(20886, 5, 1, 'This may be the end for you, Dalliah.  What a shame that would be.', 14, 0, 100, 1, 0, 11245, 'Wrath-Scryer Soccothrates - Dalliah at 25%'),
(20886, 5, 2, 'I suggest a new strategy: you draw their attacks while I gather reinforcements.', 14, 0, 100, 1, 0, 11246, 'Wrath-Scryer Soccothrates - Dalliah at 25%'),
(20886, 6, 0, 'Finally!  Well done!', 14, 0, 100, 66, 0, 11247, 'Wrath-Scryer Soccothrates - Dalliah dies');
