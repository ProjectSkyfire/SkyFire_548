-- Quests 13523 (Power Over the Tides) and 13565 (Twice Removed):
-- spell summon/credit handled in spell_darkshore_corpse_soothe (62518, 64306, 70719).

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (32890, 32937, 33206, 34009);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(32890, 0, 0, 0, 8, 0, 100, 0, 62517, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Enraged Tidal Spirit - On Spellhit Elune''s Presence - Set Event Phase 1'),
(33206, 0, 0, 0, 8, 0, 100, 0, 64306, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkscale Scout - On Spellhit Call Withered Ent - Set Event Phase 1'),
(32937, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 84, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tranquil Tidal Spirit - On Respawn - Say Line 0'),
(32937, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tranquil Tidal Spirit - Linked - Start Random Movement'),
(32937, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 8000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tranquil Tidal Spirit - Linked - Despawn In 8 Sec'),
(32937, 0, 3, 0, 1, 0, 100, 0, 2000, 3000, 2000, 3000, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Tranquil Tidal Spirit - OOC - Random Move'),
(34009, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Withered Ent - On Respawn - Start Random Movement'),
(34009, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 8000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Withered Ent - Linked - Despawn In 8 Sec'),
(34009, 0, 2, 0, 1, 0, 100, 0, 2000, 3000, 2000, 3000, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Withered Ent - OOC - Random Move');

UPDATE `creature_template`
SET `AIName` = 'SmartAI', `ScriptName` = ''
WHERE `entry` IN (32890, 32937, 33206, 34009)
  AND (`AIName` IS NULL OR `AIName` = '' OR `AIName` <> 'SmartAI')
  AND (`ScriptName` IS NULL OR `ScriptName` = '');

UPDATE `creature_template`
SET `minlevel` = 14, `maxlevel` = 14
WHERE `entry` = 32937
  AND (`minlevel` <> 14 OR `maxlevel` <> 14);

UPDATE `creature_template`
SET `minlevel` = 12, `maxlevel` = 13
WHERE `entry` = 34009
  AND (`minlevel` <> 12 OR `maxlevel` <> 13);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` IN (62517, 64306);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 62517, 0, 0, 31, 1, 3, 32890, 0, 0, 0, 0, '', 'Elune''s Presence requires dead Enraged Tidal Spirit'),
(17, 0, 62517, 0, 0, 36, 1, 0, 0, 0, 1, 0, 0, '', 'Elune''s Presence requires dead target'),
(17, 0, 64306, 0, 0, 31, 1, 3, 33206, 0, 0, 0, 0, '', 'Call Withered Ent requires dead Darkscale Scout'),
(17, 0, 64306, 0, 0, 36, 1, 0, 0, 0, 1, 0, 0, '', 'Call Withered Ent requires dead target');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (62517, 62518, 64306, 70719);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(62517, 'spell_darkshore_corpse_soothe'),
(62518, 'spell_darkshore_corpse_soothe'),
(64306, 'spell_darkshore_corpse_soothe'),
(70719, 'spell_darkshore_corpse_soothe');

DELETE FROM `creature_text` WHERE `entry` = 32937;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32937, 0, 0, 'Return to the ocean... return to the tides.', 12, 0, 33, 0, 0, 0, 'Tranquil Tidal Spirit'),
(32937, 0, 1, 'The tides do as Elune commands.', 12, 0, 33, 0, 0, 0, 'Tranquil Tidal Spirit'),
(32937, 0, 2, 'We thank you mortal.  The ocean''s will is to be free.', 12, 0, 34, 0, 0, 0, 'Tranquil Tidal Spirit');
