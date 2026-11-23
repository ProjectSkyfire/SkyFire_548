-- Latent Demons of the Land (28312, 28444) asks for 18 Fallen Flamekin, which the player picks up from the bodies of
-- Flamekin Spitters, Torchers and Ragers. On death each of them summons a Fallen Flamekin (spell 90645) and despawns; the
-- Fallen Flamekin can be clicked while the quest is taken (spell 90650), and clicking it adds the item (63423) and removes
-- the body. The Fallen Flamekin was a level 1 placeholder without a spell click, flag or script, and the three Flamekin
-- did nothing on death, so the quest items could not be collected.
-- Rows, spell click, condition, template values and aura are as in Legends of Azeroth 5.4.8. The death rows are added
-- behind the existing rows of the three Flamekin. They only run when the killer has one of the two quests taken
-- (conditions of source type 22 on the death rows, evaluated on the killer), so other players keep the loot of the body,
-- and the original body is removed after 30 seconds instead of at once so the quest player can still loot it. A kill by
-- a pet or by another group member does not count as the killer having the quest.

UPDATE `creature_template` SET `minlevel` = 50, `maxlevel` = 51, `npcflag` = 16777216, `unit_flags` = 33536, `baseattacktime` = 2000, `rangeattacktime` = 2000, `AIName` = 'SmartAI' WHERE `entry` = 48649 AND `ScriptName` = '';

DELETE FROM `creature_template_addon` WHERE `entry` = 48649;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(48649, 0, 0, 0, 1, 0, '29266 90650');

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 48649;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(48649, 90650, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 48649;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 48649, 90650, 0, 0, 9, 0, 28312, 0, 0, 0, 0, 0, '', 'Spellclick allow if player have quest 28312'),
(18, 48649, 90650, 0, 1, 9, 0, 28444, 0, 0, 0, 0, 0, '', 'Spellclick allow if player have quest 28444');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 48649;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(48649, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 56, 63423, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fallen Flamekin - On Spellclick - Add Item Fallen Flamekin'),
(48649, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fallen Flamekin - Linked To Id 0 - Despawn');

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 9776 AND `id` IN (8, 9)) OR (`entryorguid` = 9778 AND `id` IN (10, 11)) OR (`entryorguid` = 9779 AND `id` IN (2, 3)));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(9776, 0, 8, 9, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 90645, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Spitter - On Just Died - Cast Summon Fallen Flamekin'),
(9776, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Spitter - Linked To Death - Despawn After 30 Seconds'),
(9778, 0, 10, 11, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 90645, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Torcher - On Just Died - Cast Summon Fallen Flamekin'),
(9778, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Torcher - Linked To Death - Despawn After 30 Seconds'),
(9779, 0, 2, 3, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 90645, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Rager - On Just Died - Cast Summon Fallen Flamekin'),
(9779, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 30000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Flamekin Rager - Linked To Death - Despawn After 30 Seconds');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND ((`SourceEntry` = 9776 AND `SourceGroup` = 9) OR (`SourceEntry` = 9778 AND `SourceGroup` = 11) OR (`SourceEntry` = 9779 AND `SourceGroup` = 3));
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 9, 9776, 0, 0, 9, 0, 28312, 0, 0, 0, 0, 0, '', 'Flamekin Spitter - Summon Fallen Flamekin only if the killer has quest 28312'),
(22, 9, 9776, 0, 1, 9, 0, 28444, 0, 0, 0, 0, 0, '', 'Flamekin Spitter - Summon Fallen Flamekin only if the killer has quest 28444'),
(22, 11, 9778, 0, 0, 9, 0, 28312, 0, 0, 0, 0, 0, '', 'Flamekin Torcher - Summon Fallen Flamekin only if the killer has quest 28312'),
(22, 11, 9778, 0, 1, 9, 0, 28444, 0, 0, 0, 0, 0, '', 'Flamekin Torcher - Summon Fallen Flamekin only if the killer has quest 28444'),
(22, 3, 9779, 0, 0, 9, 0, 28312, 0, 0, 0, 0, 0, '', 'Flamekin Rager - Summon Fallen Flamekin only if the killer has quest 28312'),
(22, 3, 9779, 0, 1, 9, 0, 28444, 0, 0, 0, 0, 0, '', 'Flamekin Rager - Summon Fallen Flamekin only if the killer has quest 28444');
