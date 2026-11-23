-- The Blackrock Cudgel of Discipline (item 63390, spell 89562) is used on Blackrock soldiers, Firegut ogres, Smolderthorn
-- trolls and goblin mercenaries for "I Am the Law and I Am the Lash" (28278, 28439) and on Blackrock Sergeants for
-- "Abuse of Power" (28279, 28440). The spell had no target limit, so it could be used on any creature, and the
-- Sergeants did not give their credit (48210), so the second quest could not be finished.
-- The 16 target conditions, the speech and the spell hit lines are as in Legends of Azeroth 5.4.8; the Sergeant also gives
-- the credit on death there. The source's rows on the Smolderthorn Assassin and the Firegut Flamespeaker that credit the
-- Cudgel Credit (48195) by hand are left out: the spell's own effect (89568, triggered by the cudgel) already credits the
-- user, so the hand-made credit would count each use twice. The Sergeant's Unmask on being hit is left out too, as the
-- source casts it on the Sergeant itself. The existing rows are kept; the new ones follow them.

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 89562;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 89562, 0, 0, 31, 1, 3, 7035, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 1, 31, 1, 3, 48121, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 2, 31, 1, 3, 7033, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 3, 31, 1, 3, 7034, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 4, 31, 1, 3, 48120, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 5, 31, 1, 3, 48414, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 6, 31, 1, 3, 48432, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 7, 31, 1, 3, 48413, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 8, 31, 1, 3, 48201, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 9, 31, 1, 3, 7027, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 10, 31, 1, 3, 7025, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 11, 31, 1, 3, 7026, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 12, 31, 1, 3, 7028, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 13, 31, 1, 3, 47782, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 14, 31, 1, 3, 48119, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier'),
(17, 0, 89562, 0, 15, 31, 1, 3, 48118, 0, 0, 0, 0, '', 'Blackrock Cudgel of Discipline - Target is a Blackrock, Firegut, Smolderthorn or goblin soldier');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 48119 AND `id` = 2) OR (`entryorguid` = 48121 AND `id` = 4) OR (`entryorguid` = 48201 AND `id` IN (3, 4)));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48119, 0, 2, 0, 8, 0, 100, 0, 89562, 0, 30000, 30000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Soldier - On SpellHit Cudgel - Say Line 0'),
(48121, 0, 4, 0, 8, 0, 100, 0, 89562, 0, 30000, 30000, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Soldier - On SpellHit Cudgel - Say Line 1'),
(48201, 0, 3, 0, 8, 0, 100, 1, 89562, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Sergeant - On SpellHit Cudgel - Say Line 0'),
(48201, 0, 4, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 48210, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Blackrock Sergeant - On Death - Quest Credit Sergeant Cudgel');

DELETE FROM `creature_text` WHERE `entry` IN (48119, 48201) OR (`entry` = 48121 AND `groupid` = 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48119, 0, 0, 'Hey, wait a minute...', 12, 0, 100, 0, 0, 0, ''),
(48119, 0, 1, 'I don''t recognize you...', 12, 0, 100, 0, 0, 0, ''),
(48119, 0, 2, 'Hmm... are you new around here...?', 12, 0, 100, 0, 0, 0, ''),
(48119, 0, 3, 'You''re not one of us, are you...?', 12, 0, 100, 0, 0, 0, ''),
(48201, 0, 0, 'What do you think you''re doing, soldier!?', 12, 0, 100, 0, 0, 0, ''),
(48201, 0, 1, 'I''ll slice you in two!  Wait a minute... who are you?', 12, 0, 100, 0, 0, 0, ''),
(48201, 0, 2, 'Stop that, I''m no soldier!  Hmm... are you?', 12, 0, 100, 0, 0, 0, ''),
(48201, 0, 3, 'Is that a mask you''re wearing, soldier?', 12, 0, 100, 0, 0, 0, ''),
(48121, 1, 0, 'You''re not one of us, are you...?', 12, 0, 100, 0, 0, 0, '');
