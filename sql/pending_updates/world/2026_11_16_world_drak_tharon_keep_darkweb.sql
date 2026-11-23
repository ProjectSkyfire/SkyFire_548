-- Drak'Tharon Keep: SmartAI for the Darkweb Recluse, Hatchling and Victim
-- Ported from TrinityCore 3.3.5. The corpse delay row of the Darkweb Victim is left out: SmartAI has no such action.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (26625,26674,27909) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (26625,26674,27909);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(26625, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - On Aggro - Set Event Phase 1'),
(26625, 0, 1, 0, 0, 1, 100, 0, 4000, 8000, 20000, 30000, 0, 11, 49704, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - IC - Cast ''Encasing Webs'' (Phase 1)'),
(26625, 0, 2, 0, 0, 1, 100, 0, 2000, 4000, 8000, 10000, 0, 11, 49708, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - IC - Cast ''Poison Spit'' (Phase 1)'),
(26625, 0, 3, 4, 2, 0, 100, 1, 0, 20, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - Health Between 0-20% - Set Event Phase 0'),
(26625, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - Health Between 0-20% - Say Line 0'),
(26625, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 92, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - Health Between 0-20% - Stop Current Cast'),
(26625, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - Health Between 0-20% - Set ReactState Passive'),
(26625, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 69, 1, 0, 0, 0, 0, 0, 19, 26675, 100, 0, 0, 0, 0, 0, 'Darkweb Recluse - Health Between 0-20% - Move To Nearby NPC ''Spider Summon Target'''),
(26625, 0, 8, 9, 34, 0, 100, 0, 0, 1, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 26675, 5, 0, 0, 0, 0, 0, 'Darkweb Recluse - On MovementInform PointID 1 - Set Data 1 1 To Nearby NPC ''Spider Summon Target'''),
(26625, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - On MovementInform PointID 1 - Set Event Phase 1'),
(26625, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - On MovementInform PointID 1 - Set ReactState Aggressive'),
(26625, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Recluse - On MovementInform PointID 1 - Attack Target'),
(26674, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 100, 0, 0, 0, 0, 0, 0, 'Darkweb Hatchling - On Spawn - Attack Closest Player'),
(27909, 0, 1, 0, 37, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Victim - On AI Initialize - Set Reactstate Passive'),
(27909, 0, 2, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Victim - On Spawn - Set Event Phase 1'),
(27909, 0, 3, 0, 38, 0, 100, 0, 1, 1, 0, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Victim - On Data Set 1 1 - Set Event Phase 2'),
(27909, 0, 4, 0, 6, 1, 100, 0, 0, 0, 0, 0, 0, 11, 49960, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Victim - On Death - Cast ''Summon Random Drakkari'' (Phase 1)'),
(27909, 0, 5, 0, 6, 2, 100, 0, 0, 0, 0, 0, 0, 11, 49952, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkweb Victim - On Death - Cast ''Summon Kurzel'' (Phase 2)');

INSERT IGNORE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(26625, 0, 0, '%s flees back to its den....', 16, 0, 100, 0, 0, 0, 'Darkweb Recluse');

DELETE FROM `spell_script_names` WHERE `spell_id` = 49960;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(49960, 'spell_summon_random_drakkari');
