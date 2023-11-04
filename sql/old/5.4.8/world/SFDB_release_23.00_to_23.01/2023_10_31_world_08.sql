DELETE FROM `spell_loot_template` WHERE `entry`=51771;
INSERT INTO `spell_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(51771, 38631, 100, 1, 0, 1, 1);

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=190584;
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance`=100 WHERE `entry`=24611 AND `item`=38607;

DELETE FROM `conditions` WHERE `SourceGroup`=24611 AND `SourceEntry`=38607;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(4, 24611, 38607, 0, 0, 9, 0, 12619, 0, 0, 0, 0, 0, '', NULL),
(4, 24611, 38607, 0, 0, 2, 0, 38607, 1, 0, 1, 0, 0, '', NULL);

