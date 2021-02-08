DELETE FROM `gameobject_template` WHERE `entry`=211363;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(211363, 22, 7537, 'Refreshment Table', '', '', '', 0, 0, 1.2, 0, 0, 0, 0, 0, 0, 116136, 50, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 18414);

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_conjure_refreshment_table';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(43987, 'spell_mage_conjure_refreshment_table');
