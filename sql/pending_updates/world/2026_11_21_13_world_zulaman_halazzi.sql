-- Halazzi's Spirit of the Lynx fights for him in the lynx phase and the two totems are scripted. The lynx was a
-- level 73 creature of a friendly faction, the water totem a level 1 friend with no script.
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 24143 AND `minlevel` = 73;
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70, `ScriptName` = 'npc_halazzi_lightning_totem' WHERE `entry` = 24224 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `faction_A` = 1890, `faction_H` = 1890, `ScriptName` = 'npc_halazzi_water_totem' WHERE `entry` = 52755 AND `ScriptName` = '';
