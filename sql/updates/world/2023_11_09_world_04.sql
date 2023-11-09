UPDATE `quest_template` SET `SourceItemId`=0 WHERE `Id`=29408;
UPDATE `gameobject_template` SET `unkInt32`=0 WHERE `entry`=210986;

DELETE FROM `smart_scripts` WHERE `entryorguid`=53566 AND `id`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_param1`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(53566, 9, 19, 29408, 11, 114611, 7, 'Master Shang Xi: On quest accept 29408 - cast spell 114611');
