UPDATE `creature_template` SET `ScriptName`='npc_jinthala' WHERE `entry`=37951;
UPDATE `creature_template` SET `ScriptName`='npc_zuni' WHERE `entry`=37988;

DELETE FROM `playercreateinfo_spell_cast` WHERE `Spell`=71033;
INSERT INTO `playercreateinfo_spell_cast` (`racemask`, `Spell`, `Note`) VALUES
(128, 71033, 'Troll - Calm of the Novice');

