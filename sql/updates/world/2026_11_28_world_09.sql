-- Draconic Mending: wounded dragons, defending their menders, and Ruby Seeds.
-- The original introduction credit spell requires a personal summoner.
-- Use proximity credit on the permanent Lirastrasza instead.
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_vermillion_lirastrasza_intro' WHERE `entry`=45708;
DELETE FROM `smart_scripts` WHERE `entryorguid`=45708 AND `source_type`=0;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=45708 AND `SourceId`=0;

-- Menders and attacking drakes belong to their wounded dragon encounter.
-- Remove only the newly restored ambient menders; retain their recorded neighbours.
DELETE FROM `waypoint_data` WHERE `id` IN
 (SELECT `a`.`path_id` FROM `creature_addon` `a` JOIN `creature` `c` ON `c`.`guid`=`a`.`guid`
  WHERE `c`.`guid` BETWEEN 1108000 AND 1108095 AND `c`.`id`=45746 AND `a`.`path_id`<>0);
DELETE FROM `creature_addon` WHERE `guid` IN
 (SELECT `guid` FROM `creature` WHERE `guid` BETWEEN 1108000 AND 1108095 AND `id`=45746);
DELETE FROM `creature` WHERE `guid` BETWEEN 1108000 AND 1108095 AND `id`=45746;

UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_vermillion_wounded_dragon', `InhabitType`=5
 WHERE `entry` IN (45560,45857,45866);
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_vermillion_mender', `InhabitType`=1
 WHERE `entry`=45746;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_vermillion_wyrmkiller', `InhabitType`=1
 WHERE `entry`=45748;
UPDATE `creature` SET `MovementType`=0, `spawndist`=0
 WHERE `guid` BETWEEN 1108000 AND 1108095 AND `id` IN (45560,45857,45866);

INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
 (85389,'spell_vermillion_ruby_seeds')
ON DUPLICATE KEY UPDATE `ScriptName`=VALUES(`ScriptName`);

-- Draconic Mending's native channel selects the nearest wounded dragon.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceGroup`=1 AND `SourceEntry`=85340;
INSERT INTO `conditions`
 (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,
  `ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,
  `NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
 (13,1,85340,0,0,31,0,3,45560,0,0,0,0,'','Draconic Mending - Acridostrasz'),
 (13,1,85340,0,1,31,0,3,45857,0,0,0,0,'','Draconic Mending - Celastrasza'),
 (13,1,85340,0,2,31,0,3,45866,0,0,0,0,'','Draconic Mending - Novrastrasz');
