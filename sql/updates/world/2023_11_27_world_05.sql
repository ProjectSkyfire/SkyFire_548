UPDATE `creature_template` SET `ScriptName`='npc_aysa_chamber_of_whispers' WHERE `entry`=55744;

DELETE FROM `areatrigger_scripts` WHERE `entry`=7041;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(7041, 'at_chamber_of_whispers_entrance');
