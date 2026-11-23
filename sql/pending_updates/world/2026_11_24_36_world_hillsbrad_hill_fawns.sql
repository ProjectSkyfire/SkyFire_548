-- A Fighting Chance (28495): Duskingdawn's Wand (90679) is used on Hill Fawns, ten of them count as saved. The wand could
-- hit anything and the fawns gave no credit, so the quest could not be completed. Added as in Legends of Azeroth 5.4.8: the
-- wand can only hit a Hill Fawn (48187), and the fawn credits the player once, blesses itself, turns hostile and
-- despawns after twenty seconds. Legends of Azeroth stops a second hit with a condition on the script phase (type 101),
-- which does not exist here; the spellhit row is not repeatable instead, so a fawn credits once per spawn. The existing
-- rows of the fawn (Blessing at 40 percent, Gore, Fawnplode on death) keep ids 0 to 2, the new rows follow them.

DELETE FROM `smart_scripts` WHERE `entryorguid` = 48187 AND `source_type` = 0 AND `id` IN (3, 4, 5, 6, 7, 8, 9, 10);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48187, 0, 3, 4, 11, 0, 100, 0, 0, 0, 0, 0, 0, 2, 1999, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - On Respawn - Set Faction 1999'),
(48187, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 0 - Set Phase 0 '),
(48187, 0, 5, 6, 8, 0, 100, 1, 90679, 0, 20000, 20000, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - On SpellHit Duskingdawn''s Blessing - Set Phase 1'),
(48187, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 48684, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 2 - Quest Credit'),
(48187, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 90685, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 3 - Cast Duskingdawn''s Blessing'),
(48187, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 4 - Set Faction 14'),
(48187, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 5 - Set React State Defensive'),
(48187, 0, 10, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 20000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hill Fawn - Linked To Id 6 - Despawn after 20 sec');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 90679;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 90679, 0, 0, 31, 0, 3, 48187, 0, 0, 0, 0, '', 'Duskingdawn''s Wand can only hit Hill Fawns');
