-- Pit of Saron: the falling icicles of the cavern and Tyrannus' warning at its entrance.
-- Ported from TrinityCore 3.3.5.

UPDATE `creature_template` SET `ScriptName` = 'npc_pit_of_saron_icicle' WHERE `entry` = 36847 AND `ScriptName` = '';

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (5580, 5581);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5580, 'at_pit_cavern_entrance'),
(5581, 'at_pit_cavern_end');

DELETE FROM `creature_text` WHERE `entry` = 36794 AND `groupid` = 3;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(36794, 3, 0, 'Rimefang, trap them within the tunnel! BURY THEM ALIVE!', 14, 0, 100, 0, 0, 16757, 'SAY_TYRANNUS_CAVERN_ENTRANCE');
