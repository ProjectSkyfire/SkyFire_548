-- Proving Pit (Echo Isles): the Darkspear Jailor walks to the cage and opens it, then the Captive Spitescale Scout walks
-- into the pit and attacks. Players can no longer open the cage themselves, which left the scout stuck and immune.
UPDATE `creature_template` SET `ScriptName` = 'npc_captive_spitescale_scout' WHERE `entry` = 38142;
UPDATE `gameobject_template` SET `faction` = 114, `flags` = 2 WHERE `entry` = 201968;

DELETE FROM `creature_text` WHERE `entry` IN (38142, 39062);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38142, 0, 0, 'I sshal ssslaughter you, Darksspear runt!', 12, 0, 100, 0, 0, 0, 'Captive Spitescale Scout - Released'),
(39062, 0, 0, 'Get in the pit and show us your stuff, $G boy:girl;.', 12, 0, 100, 0, 0, 0, 'Darkspear Jailor - Opens the cage');
