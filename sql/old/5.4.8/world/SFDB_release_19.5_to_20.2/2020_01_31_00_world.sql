ALTER TABLE `creature_template`
	CHANGE COLUMN `npcflag` `npcflag` INT NOT NULL DEFAULT '0' AFTER `faction_H`;

UPDATE `creature_template` SET `npcflag`=-2147483648, `ScriptName`='' WHERE `entry`=62943;
