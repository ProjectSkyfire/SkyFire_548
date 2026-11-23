-- Twilight Highlands, "Devoured" 27377 and "The Worldbreaker" 27378: the dream of Iso'rath. The first file of the dream (iso_rath_dream_script) carried the
-- player through with a wait of 30 and of 20 seconds. This one puts the events of the client data in the waits.
--
-- Devoured. Yevaa finishes the summoning while the player holds back Iso'rath's defenses. The client data has them as four auras that Yevaa
-- puts on herself: "Summon Blood of Iso'rath Aura 1" to 4 (89679 to 89682) summon a Blood of Iso'rath (48243, 48244, 48245, 48247, each lives ten
-- seconds) every 10, 10, 10 and 5 seconds, started at once and by the timer auras 89686 to 89688 after 20, 55 and 73 seconds. The summon spells need a
-- creature to land beside that no data names, so the script summons the same creatures at the same times with Smart AI, and the wave ends at 90
-- seconds, after the last timer. "Iso'rath Death Prevention Aura" (89444) keeps the player alive: the blow that would kill does no damage and
-- starts the fake death, which credits the quest and, nine seconds later, carries the player to the Maelstrom. A player who lives to the end gets the
-- fake death from Yevaa. The Blood of Iso'rath that stand on paths in the stomach (26 spawns) stay.
--
-- The Worldbreaker. The dream of the Maelstrom has Thrall (texts of the data, one per step), Deathwing (the summon 89613 and the target of Deathwing
-- Teleport 89651 at 928.3, 826.5, 52.9), and Muln, Aggra and Nobundo at the targets of their teleport spells (89647, 89649, 89650), which are the
-- places where they fall to him. Deathwing casts Fireburst (89580), the three fall at 10, 20 and 38 seconds with the lines of Thrall, "Maelstrom Death
-- Prevention Aura" (89655) keeps the player alive and Thrall casts the "Maelstrom Fake Death Aura" at 54 seconds, which credits the quest and carries
-- the player back. The data has no damage for Deathwing and the arena is far from where the player stands, so the player does not fight him: the part
-- of this quest that is still a scene is the fight itself. Thrall hears the "Thrall Post Death Ping" (89473) that the teleport casts and greets the player.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (47991, 48051, 48005);

-- The creatures of the dream had level 1 and no faction. The values of the 4.3.4 data; Deathwing cannot be attacked, he is a part of the scene
UPDATE `creature_template` SET `minlevel` = 88, `maxlevel` = 88, `exp` = 3, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33554434 WHERE `entry` = 48005;
UPDATE `creature_template` SET `minlevel` = 88, `maxlevel` = 88, `exp` = 3, `faction_A` = 2167, `faction_H` = 2167 WHERE `entry` IN (48076, 48079, 48084);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (89444, 89655);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(89444, 'spell_twilight_dream_death_prevention'),
(89655, 'spell_twilight_dream_death_prevention');

-- The ping that the teleport to the Maelstrom casts finds Thrall
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 1 AND `SourceEntry` = 89473;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 89473, 0, 0, 31, 0, 3, 48051, 0, 0, 0, 0, '', 'Thrall Post Death Ping - Target Thrall');

DELETE FROM `creature_text` WHERE `entry` = 48051;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(48051, 0, 0, 'I bring you back from the brink of death, $Gbrother:sister;. We are in need of your aid!', 12, 0, 100, 1, 0, 0, 'Thrall to Player'),
(48051, 1, 0, 'NOBUNDO! NOOO!', 12, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 2, 0, 'You will die this day, Earthwarder! $n will see to it!', 14, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 3, 0, 'Aggra... this can''t be... this has gone too far...', 12, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 4, 0, 'Do something, you useless $r. My allies are giving their lives for you!', 12, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 5, 0, 'Muln falls! And the Earthen Ring dies with him.', 12, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 6, 0, 'You''ve failed us all, $n. Deathwing has won. The world is doomed.', 12, 0, 100, 0, 0, 0, 'Thrall to Player'),
(48051, 7, 0, 'We''re surrounded! Its defenses are too great. $n! Wake up!', 12, 0, 100, 0, 0, 0, 'Thrall to Player');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (47991, 48051, 48005);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (4799100, 4805100);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47991, 0, 0, 0, 19, 0, 100, 0, 27377, 0, 0, 0, 0, 80, 4799100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Earthcaller Yevaa - On Quest Accept (Devoured) - Run Script'),
(4799100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 89444, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Cast Iso''rath Death Prevention Aura on Invoker'),
(4799100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 512, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Set IMMUNE_TO_NPC, the Blood of Iso''rath leave her alone'),
(4799100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 3, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 4, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 6, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 8, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 10, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 11, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 12, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 12, 48245, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 3, every 10 s from 55 s)'),
(4799100, 9, 13, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 14, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 15, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 12, 48245, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 3, every 10 s from 55 s)'),
(4799100, 9, 16, 0, 0, 0, 100, 0, 5000, 5000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 17, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 18, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 48247, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 4, every 5 s from 73 s)'),
(4799100, 9, 19, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 12, 48245, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 3, every 10 s from 55 s)'),
(4799100, 9, 20, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 48247, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 4, every 5 s from 73 s)'),
(4799100, 9, 21, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 12, 48243, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, -12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Summon Blood of Iso''rath Aura 1, every 10 s)'),
(4799100, 9, 22, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48244, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 2, every 10 s from 20 s)'),
(4799100, 9, 23, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 48247, 3, 10000, 1, 0, 0, 1, 0, 0, 0, -12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 4, every 5 s from 73 s)'),
(4799100, 9, 24, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 12, 48245, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 12.0, 0.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 3, every 10 s from 55 s)'),
(4799100, 9, 25, 0, 0, 0, 100, 0, 3000, 3000, 0, 0, 0, 12, 48247, 3, 10000, 1, 0, 0, 1, 0, 0, 0, 0.0, 12.0, 0.0, 0.0, 'Script - Summon Blood of Iso''rath (Aura 4, every 5 s from 73 s)'),
(4799100, 9, 26, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 89444, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Remove Iso''rath Death Prevention Aura from Invoker'),
(4799100, 9, 27, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 89458, 34, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Cast Iso''rath Fake Death Aura on Invoker (the summoning is finished)'),
(4799100, 9, 28, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 512, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Remove IMMUNE_TO_NPC'),
(48051, 0, 0, 0, 19, 0, 100, 0, 27378, 0, 0, 0, 0, 80, 4805100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thrall - On Quest Accept (The Worldbreaker) - Run Script'),
(48051, 0, 1, 0, 8, 0, 100, 0, 89473, 0, 5000, 5000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Thrall - On Spell Hit (Thrall Post Death Ping) - Say Text 0'),
(4805100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 89655, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Cast Maelstrom Death Prevention Aura on Invoker'),
(4805100, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48005, 3, 60000, 0, 0, 0, 8, 0, 0, 0, 928.285, 826.519, 52.9494, 1.9337, 'Script - Summon Deathwing at the target of Deathwing Teleport 89651'),
(4805100, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48084, 3, 60000, 0, 0, 0, 8, 0, 0, 0, 721.634, 822.92, 129.037, 0.0174, 'Script - Summon Muln Earthfury at the target of Muln Teleport 89647'),
(4805100, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48076, 3, 60000, 0, 0, 0, 8, 0, 0, 0, 1089.45, 776.464, 72.7129, 2.8405, 'Script - Summon Aggra at the target of Aggra Teleport 89649'),
(4805100, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 12, 48079, 3, 60000, 0, 0, 0, 8, 0, 0, 0, 822.359, 653.179, -8.43734, 1.0223, 'Script - Summon Farseer Nobundo at the target of Nobundo Teleport 89650'),
(4805100, 9, 5, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 2 (You will die this day, Earthwarder!)'),
(4805100, 9, 6, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 1 (NOBUNDO! NOOO!)'),
(4805100, 9, 7, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 9, 48079, 0, 800, 0, 0, 0, 0, 'Script - Kill Farseer Nobundo'),
(4805100, 9, 8, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 3 (Aggra...)'),
(4805100, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 9, 48076, 0, 800, 0, 0, 0, 0, 'Script - Kill Aggra'),
(4805100, 9, 10, 0, 0, 0, 100, 0, 10000, 10000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 4 (Do something)'),
(4805100, 9, 11, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 5 (Muln falls)'),
(4805100, 9, 12, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 9, 48084, 0, 800, 0, 0, 0, 0, 'Script - Kill Muln Earthfury'),
(4805100, 9, 13, 0, 0, 0, 100, 0, 8000, 8000, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 6 (You have failed us all)'),
(4805100, 9, 14, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Script - Say Text 7 (Wake up!)'),
(4805100, 9, 15, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 28, 89655, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Remove Maelstrom Death Prevention Aura from Invoker'),
(4805100, 9, 16, 0, 0, 0, 100, 0, 100, 100, 0, 0, 0, 11, 89656, 34, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Script - Cast Maelstrom Fake Death Aura on Invoker'),
(4805100, 9, 17, 0, 0, 0, 100, 0, 3900, 3900, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 9, 48005, 0, 800, 0, 0, 0, 0, 'Script - Despawn Deathwing'),
(48005, 0, 0, 0, 1, 0, 100, 0, 8000, 8000, 12000, 15000, 0, 11, 89580, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Deathwing - Out of Combat - Cast Fireburst');
