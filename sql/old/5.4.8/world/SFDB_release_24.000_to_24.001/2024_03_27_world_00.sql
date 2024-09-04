UPDATE `creature_template` SET `ScriptName`='npc_zuni_bridge' WHERE `entry`=38930;

DELETE FROM `areatrigger_scripts` WHERE `entry`=5777;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(5777, 'at_darkspear_isle');
