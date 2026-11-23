-- Uldaman (map 70): data for the Cataclysm entrance camp and the Stonevault Pillagers.
-- High Examiner Tae'thelan Bloodwatcher was a level 3 placeholder, the Stonevault Pillager had no AI or gold and the
-- Reliquary Digger had other unit flags. Idempotent: absolute UPDATEs, DELETE before INSERT.

UPDATE `creature_template` SET `minlevel`=88, `maxlevel`=88, `exp`=3, `mechanic_immune_mask`=617299967 WHERE `entry`=46236;
UPDATE `creature_template` SET `unit_flags`=163840 WHERE `entry`=46231;
UPDATE `creature_template` SET `AIName`='SmartAI', `mingold`=454, `maxgold`=454 WHERE `entry`=24830 AND `ScriptName`='';

DELETE FROM `creature_template_addon` WHERE `entry`=46236;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(46236, 0, 0, 33554432, 0, 0, '31635');

DELETE FROM `creature_text` WHERE `entry`=24830;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24830, 0, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Stonevault Pillager');

DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=24830;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(24830, 0, 0, 0, 2, 0, 100, 4, 0, 30, 120000, 120000, 0, 11, 8599, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Pillager - Between 0-30% Health - Cast ''Enrage'''),
(24830, 0, 1, 0, 2, 0, 100, 4, 0, 30, 120000, 120000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Pillager - Between 0-30% Health - Say Line 0'),
(24830, 0, 2, 0, 2, 0, 100, 5, 0, 15, 0, 0, 0, 25, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Stonevault Pillager - Between 0-15% Health - Flee For Assist');
