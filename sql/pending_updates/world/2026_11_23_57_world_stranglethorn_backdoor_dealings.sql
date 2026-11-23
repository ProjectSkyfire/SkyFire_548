-- Backdoor Dealings (26550, 26809) and Eliminate the Outcast (26551, 26810): the player lights the incense at the Totem of
-- Hir'eek (menu 11711, spell 80789 gives the "Bat Totem Credit" and calls Wings of Hir'eek, a flying vehicle), the bat
-- carries the player along a 46 point path to Zanzil's hideout, where Zanzil the Outcast and the Shade of the Hexxer are
-- summoned for a short conversation and the "Spy on Zanzil" credit is given; the bat then drops the player. The totem
-- had no script, the bat was no vehicle (VehicleId 0), had no spell click and no script or path, Zanzil and the Shade had
-- no speech, and the area trigger 6063 at the hideout was bound to SmartAI with no rows, so no credit was ever given.
-- Everything is as in Legends of Azeroth 5.4.8 (rows, path, speech, vehicle id, speed): the source names the bat's AI
-- SmartVehicleAI, which this core does not have, so SmartAI handles the passenger events. The source gives the credit of
-- the area trigger (43245, the objective of Eliminate the Outcast) to anyone who walks in; that is kept. The totem's
-- second option is shown only while one of the two quests is taken (the source has no condition). The bat flies, so its
-- movement type is set to flying; the source has no such column.

UPDATE `creature_template` SET `npcflag` = 16777216, `VehicleId` = 940, `speed_run` = 2.28572, `InhabitType` = 4, `AIName` = 'SmartAI' WHERE `entry` = 43241 AND `ScriptName` = '';
UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` = 204361 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 43241;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(43241, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 11711 AND `SourceEntry` = 1;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11711, 1, 0, 0, 9, 0, 26550, 0, 0, 0, 0, 0, '', 'Totem of Hir''eek - incense only with Backdoor Dealings'),
(15, 11711, 1, 0, 1, 9, 0, 26809, 0, 0, 0, 0, 0, '', 'Totem of Hir''eek - incense only with Backdoor Dealings');

DELETE FROM `waypoints` WHERE `entry` = 43241;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(43241, 1, -13344.1, -27.0611, 33.3311, 'Wings of Hir''eek'),
(43241, 2, -13347.1, -22.7933, 40.1211, 'Wings of Hir''eek'),
(43241, 3, -13355.1, -13.8323, 43.3679, 'Wings of Hir''eek'),
(43241, 4, -13363.4, -6.60924, 45.5293, 'Wings of Hir''eek'),
(43241, 5, -13372.9, 1.25707, 49.3975, 'Wings of Hir''eek'),
(43241, 6, -13386.8, 11.7761, 51.1131, 'Wings of Hir''eek'),
(43241, 7, -13393.7, 16.9891, 52.4641, 'Wings of Hir''eek'),
(43241, 8, -13401.6, 26.4615, 54.5607, 'Wings of Hir''eek'),
(43241, 9, -13408.9, 33.9207, 57.2804, 'Wings of Hir''eek'),
(43241, 10, -13418.8, 42.8742, 57.7546, 'Wings of Hir''eek'),
(43241, 11, -13430.2, 51.4853, 58.4707, 'Wings of Hir''eek'),
(43241, 12, -13442.8, 56.5091, 53.5414, 'Wings of Hir''eek'),
(43241, 13, -13454.6, 56.9981, 51.3627, 'Wings of Hir''eek'),
(43241, 14, -13465.4, 44.3686, 45.8021, 'Wings of Hir''eek'),
(43241, 15, -13474.8, 31.9472, 43.2841, 'Wings of Hir''eek'),
(43241, 16, -13483.6, 17.7391, 42.8541, 'Wings of Hir''eek'),
(43241, 17, -13491.3, 5.21701, 42.0973, 'Wings of Hir''eek'),
(43241, 18, -13500.8, -11.7596, 43.3771, 'Wings of Hir''eek'),
(43241, 19, -13508.8, -27.4521, 44.4429, 'Wings of Hir''eek'),
(43241, 20, -13516.9, -46.8464, 44.1882, 'Wings of Hir''eek'),
(43241, 21, -13522.1, -61.2218, 46.1518, 'Wings of Hir''eek'),
(43241, 22, -13527.8, -77.3737, 46.2121, 'Wings of Hir''eek'),
(43241, 23, -13534.5, -95.2624, 46.3382, 'Wings of Hir''eek'),
(43241, 24, -13538.3, -106.453, 47.3824, 'Wings of Hir''eek'),
(43241, 25, -13543.4, -122.961, 49.2351, 'Wings of Hir''eek'),
(43241, 26, -13549.9, -139.809, 49.1644, 'Wings of Hir''eek'),
(43241, 27, -13554.1, -155.358, 48.0754, 'Wings of Hir''eek'),
(43241, 28, -13559.1, -172.977, 46.3798, 'Wings of Hir''eek'),
(43241, 29, -13565.2, -190.436, 41.3662, 'Wings of Hir''eek'),
(43241, 30, -13572.3, -205.376, 38.0821, 'Wings of Hir''eek'),
(43241, 31, -13581.9, -221.893, 34.2933, 'Wings of Hir''eek'),
(43241, 32, -13589.1, -233.811, 31.3914, 'Wings of Hir''eek'),
(43241, 33, -13598.1, -246.583, 27.0274, 'Wings of Hir''eek'),
(43241, 34, -13608.1, -255.659, 26.5399, 'Wings of Hir''eek'),
(43241, 35, -13621.9, -264.163, 25.9037, 'Wings of Hir''eek'),
(43241, 36, -13635.2, -275.189, 24.0415, 'Wings of Hir''eek'),
(43241, 37, -13648.1, -283.056, 23.3659, 'Wings of Hir''eek'),
(43241, 38, -13661.2, -293.681, 21.6189, 'Wings of Hir''eek'),
(43241, 39, -13662.5, -305.415, 20.4973, 'Wings of Hir''eek'),
(43241, 40, -13663.7, -313.951, 20.6791, 'Wings of Hir''eek'),
(43241, 41, -13662.2, -321.682, 20.6601, 'Wings of Hir''eek'),
(43241, 42, -13660.1, -328.554, 19.4834, 'Wings of Hir''eek'),
(43241, 43, -13655.1, -332.192, 17.9341, 'Wings of Hir''eek'),
(43241, 44, -13649.7, -331.231, 17.6472, 'Wings of Hir''eek'),
(43241, 45, -13645.7, -327.479, 15.7351, 'Wings of Hir''eek'),
(43241, 46, -13648.4, -319.521, 14.3109, 'Wings of Hir''eek');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 43241 AND `id` IN (0, 1, 2, 3, 4, 5);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4324100 AND `id` IN (0, 1);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 4324101 AND `id` IN (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 204361 AND `id` IN (0, 1, 2);
DELETE FROM `smart_scripts` WHERE `source_type` = 2 AND `entryorguid` = 6063 AND `id` IN (0);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(43241, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On Summon - Store Target'),
(43241, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 4324100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On Link - Run Script'),
(43241, 0, 2, 0, 27, 0, 100, 0, 0, 0, 0, 0, 0, 53, 1, 43241, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On Passenger Boarded - Start WP'),
(43241, 0, 3, 4, 28, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On Passenger Removed - Remove Spellclick'),
(43241, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On Link - Despawn'),
(43241, 0, 5, 0, 40, 0, 100, 0, 46, 0, 0, 0, 0, 80, 4324101, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - On WP Reached 46 - Run Script'),
(4324100, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Set Immune'),
(4324100, 9, 1, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 86, 46598, 0, 12, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Cross Cast Ride Vehicle Hardcoded'),
(4324101, 9, 0, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 43245, 3, 60000, 0, 0, 0, 8, 0, 0, 0, -13658.8, -306.533, 8.17537, 3.24094, 'Wings of Hir''eek - Script - Summon Zanzil the Outcast'),
(4324101, 9, 1, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 12, 43246, 3, 60000, 0, 0, 0, 8, 0, 0, 0, -13664.6, -307.128, 8.17537, 0.06793, 'Wings of Hir''eek - Script - Summon Shade of the Hexxer'),
(4324101, 9, 2, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 43245, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 3, 0, 0, 0, 100, 0, 4000, 4000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 43245, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 4, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 19, 43246, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 5, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 19, 43246, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 6, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 43245, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 7, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 19, 43246, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 8, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 43246, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 9, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 19, 43245, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 10, 0, 0, 0, 100, 0, 6000, 6000, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 19, 43245, 50, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Say Lines'),
(4324101, 9, 11, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 33, 43245, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Killcredit'),
(4324101, 9, 12, 0, 0, 0, 100, 0, 1, 1, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wings of Hir''eek - Script - Remove Ride Vehicle Hardcoded'),
(204361, 1, 0, 2, 62, 0, 100, 0, 11711, 0, 0, 0, 0, 85, 80789, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Totem of Hir''eek - Gossip Select - Cast Wings of Hir''eek'),
(204361, 1, 1, 2, 62, 0, 100, 0, 11711, 1, 0, 0, 0, 85, 80789, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Totem of Hir''eek - Gossip Select - Cast Wings of Hir''eek'),
(204361, 1, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Totem of Hir''eek - Gossip Select - Close Gossip'),
(6063, 2, 0, 0, 46, 0, 100, 0, 6063, 0, 0, 0, 0, 33, 43245, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger 6063 - On Trigger - Killcredit');

DELETE FROM `creature_text` WHERE `entry` IN (43245, 43246);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43245, 0, 0, 'Jin''do the Hexxer!  It''s been a while since I''ve seen that mask.', 12, 0, 100, 1, 0, 0, ''),
(43245, 1, 0, 'What brings you out to my part of Stranglethorn?  I''m not accustomed to taking visitors from the esteemed Zul''Gurub.', 12, 0, 100, 1, 0, 0, ''),
(43245, 2, 0, 'What a shame... but not a concern for an old exile like Zanzil.', 12, 0, 100, 1, 0, 0, ''),
(43245, 3, 0, 'Friend?  You were among those who spit and threw stones, Jin''do.  But let''s let bygones be bygones, eh?', 12, 0, 100, 1, 0, 0, ''),
(43245, 4, 0, 'So your bat and your snake need Zanzil''s help?  Very well.  I''ll see you inside Zul''Gurub... old friend.', 12, 0, 100, 1, 0, 0, ''),
(43246, 0, 0, 'You know well enough about why I''m here, Zanzil.', 12, 0, 100, 1, 0, 0, ''),
(43246, 1, 0, 'The Gurubashi have fallen on hard times.  I live, and Mandokir has returned... but the rest of us are gone.', 12, 0, 100, 1, 0, 0, ''),
(43246, 2, 0, 'Well, Zanzil, I''d like to make it your concern.  I''ve recovered the bodies of High Priestess Jeklik and High Priest Venoxis.', 12, 0, 100, 1, 0, 0, ''),
(43246, 3, 0, 'I remember what you can do with those elixirs of yours.  Can your elixirs help out an old friend?', 12, 0, 100, 1, 0, 0, '');
