DELETE FROM `creature_addon` WHERE `guid` IN (225863, 225864, 225865, 225866);
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(225863, 0, 0, 0, 0, 0, '60921'),
(225864, 0, 0, 0, 0, 0, '60921'),
(225865, 0, 0, 0, 0, 0, '60921'),
(225866, 0, 0, 0, 0, 0, '60921');

DELETE FROM `spell_area` WHERE `spell`=60922 AND `area=`5829;
INSERT INTO `spell_area` (`spell`, `area`, `quest_end`, `autocast`, `quest_end_status`) VALUES
(60922, 5829, 29786, 1, 66);

UPDATE `creature` SET `phaseId`=524 WHERE `id`=64507;
