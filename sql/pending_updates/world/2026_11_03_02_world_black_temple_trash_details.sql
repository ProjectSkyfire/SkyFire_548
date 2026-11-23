-- Black Temple details: nine Ashtongue Stalkers were never spawned; the Supremus Volcano could be attacked (faction 14
-- instead of 35); the Black Temple Trigger had no texts, so the door emotes after High Warlord Naj'entus and the Den of
-- Mortal Delights were silent; the Illidari Defilers lacked their Fiery Aura; and the Illidan Music Controller the
-- Illidan scripts look for did not exist.
DELETE FROM `creature` WHERE `guid` BETWEEN 8314000 AND 8314008;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8314000, 23374, 564, 16, 0, 0, 21442, 0, 656.298, 387.99, 112.773, 2.47837, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314001, 23374, 564, 16, 0, 0, 21442, 0, 707.532, 352.477, 125.265, 1.32645, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314002, 23374, 564, 16, 0, 0, 21442, 0, 769.792, 370.588, 112.841, 0.612888, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314003, 23374, 564, 16, 0, 0, 21442, 0, 702.75, 352.389, 125.266, 1.97222, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314004, 23374, 564, 16, 0, 0, 21442, 0, 666.901, 290.353, 125.256, 3.1765, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314005, 23374, 564, 16, 0, 0, 21442, 0, 740.37, 286.297, 125.269, 5.11381, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314006, 23374, 564, 16, 0, 0, 21442, 0, 669.151, 286.562, 125.257, 4.24115, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314007, 23374, 564, 16, 0, 0, 21442, 0, 742.599, 290.401, 125.268, 0.0523599, 3600, 0, 0, 1, 0, 0, 0, 0, 0),
(8314008, 23374, 564, 16, 0, 0, 21442, 0, 631.986, 251.528, 112.749, 2.80998, 3600, 0, 0, 1, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 23085;
DELETE FROM `creature_text` WHERE `entry` = 22984;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(22984, 0, 0, 'You hear a loud rumble of metal grinding on stone...', 16, 0, 100, 0, 0, 0, 'Black Temple Trigger - High Warlord Naj''entus dies'),
(22984, 1, 0, 'The door to The Den of Mortal Delights has opened.', 16, 0, 100, 0, 0, 0, 'Black Temple Trigger - the four bosses are dead');
UPDATE `creature_template_addon` SET `auras` = '39839' WHERE `entry` = 22853;
DELETE FROM `gameobject_template` WHERE `entry` = 185966;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(185966, 5, 3972, 'Illidan Music Controller', '', '', '', 0, 0, 1, 0, 0, 1, 1, 0, 0, -1, '', '', 12340);
DELETE FROM `gameobject` WHERE `guid` = 911300;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(911300, 185966, 564, 16, 0, 0, 706.406, 302.182, 353.744, 0.157961, 0, 0, 0.588741, 0.808322, 7200, 0, 1);
