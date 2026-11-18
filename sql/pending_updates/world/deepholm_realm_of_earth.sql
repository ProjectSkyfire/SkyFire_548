-- Deepholm, Realm of Earth: the flight from the Maelstrom into Deepholm. Thrall's quest sends the player to the wyvern on the
-- Maelstrom; the click needed spell 84071, which summons a mount that never flew, so nobody got into Deepholm by quest. The
-- wyvern click now casts the intro taxi (spell 84101) while quest 27123 is complete, the taxi wyvern 45024 flies the
-- Maelstrom path with Aggra, the intro teleport (84073) sends the rider on and the taxi starts again over Deepholm
-- (npc_deepholm_wyvern_ride, npc_maelstrom_aggra, spell_deepholm_intro_teleport). Aggra's lines are the 5.4.8 lines.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `ScriptName` = 'npc_deepholm_wyvern_ride' WHERE `entry` = 45024;
UPDATE `creature_template` SET `ScriptName` = 'npc_maelstrom_aggra', `npcflag` = 0 WHERE `entry` = 45006;
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` = 45005;
DELETE FROM `spell_script_names` WHERE `spell_id` = 84073;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (84073, 'spell_deepholm_intro_teleport');
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 45005;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES (45005, 84101, 3, 0);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 45005 AND `SourceEntry` = 84101;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 45005, 84101, 0, 0, 28, 0, 27123, 0, 0, 0, 0, 0, '', 'Show the wyvern click if quest 27123 is complete');
DELETE FROM `creature_text` WHERE `entry` IN (45006, 45027);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45006, 0, 0, 'I hope you have a strong stomach.  Travelling through the breach is not for the weak.', 12, 0, 100, 0, 0, 22606, ''),
(45006, 1, 0, 'We''re at the center of the Maelstrom, the rift between Deepholm and Azeroth.  Watch as it threatens to tear your world apart!', 12, 0, 100, 0, 0, 22607, ''),
(45006, 2, 0, 'Let us hope Thrall and the others can contain it while you''re inside.', 12, 0, 100, 0, 0, 22608, ''),
(45006, 3, 0, 'This is it!  Brace yourself!', 14, 0, 100, 0, 0, 22609, ''),
(45006, 4, 0, 'You made it!  Good!', 12, 0, 100, 4, 0, 22610, ''),
(45006, 5, 0, 'This is the Temple of Earth.  It''s the most important structure in Deepholm.', 12, 0, 100, 0, 0, 22611, ''),
(45006, 6, 0, 'There was once a Titan-made stone inside.  It was known as the World Pillar.  The name was literal.  It bore the weight of all the elemental and magnetic forces in Deepholm.', 12, 0, 100, 0, 0, 22612, ''),
(45006, 7, 0, 'It was shattered when Deathwing broke through the dome above and created the rift between the two worlds.  All of Deepholm is going to collapse into Azeroth if we do not put the World Pillar back together.', 12, 0, 100, 0, 0, 22613, ''),
(45006, 8, 0, 'The first fragment was going to be delivered to us by a Horde gunship known as the Storm''s Fury.  It never made it here.', 12, 0, 100, 0, 0, 22614, ''),
(45006, 9, 0, 'We''ve taken control of the temple and we''re containing as much of the damage as we can.  Go inside and speak to Maruut; he''s in charge.', 12, 0, 100, 0, 0, 22615, ''),
(45006, 10, 0, 'We are all counting on you.', 12, 0, 100, 0, 0, 22616, ''),
(45027, 0, 0, 'You made it!  Good!', 12, 0, 100, 4, 0, 22610, 'Aggra to Player'),
(45027, 1, 0, 'This is the Temple of Earth.  It''s the most important structure in Deepholm.', 12, 0, 100, 0, 0, 22611, 'Aggra to Player'),
(45027, 2, 0, 'There was once a Titan-made stone inside.  It was known as the World Pillar.  The name was literal.  It bore the weight of all the elemental and magnetic forces in Deepholm.', 12, 0, 100, 0, 0, 22612, 'Aggra to Player'),
(45027, 3, 0, 'It was shattered when Deathwing broke through the dome above and created the rift between the two worlds.  All of Deepholm is going to collapse into Azeroth if we do not put the World Pillar back together.', 12, 0, 100, 0, 0, 22613, 'Aggra to Player'),
(45027, 4, 0, 'The first fragment was going to be delivered to us by a Horde gunship known as the Storm''s Fury.  It never made it here.', 12, 0, 100, 0, 0, 22614, 'Aggra to Player'),
(45027, 5, 0, 'We''ve taken control of the temple and we''re containing as much of the damage as we can.  Go inside and speak to Maruut; he''s in charge.', 12, 0, 100, 0, 0, 22615, 'Aggra to Player'),
(45027, 6, 0, 'We are all counting on you.', 12, 0, 100, 0, 0, 22616, 'Aggra to Player');
