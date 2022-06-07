UPDATE `instance_template` SET `script`='instance_stormstout_brewery' WHERE `map`=961;

DELETE FROM `areatrigger_scripts` WHERE `entry`=8355;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(8355, 'at_ssb_banana_bar');

UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=35, `faction_H`=35 WHERE `entry`=59704;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=35, `faction_H`=35 WHERE `entry`=59824;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=35, `faction_H`=35 WHERE `entry`=59822;
-- Fiery Trickster
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=190, `faction_H`=190, `InhabitType`=4 WHERE `entry`=56867;
-- Sodden Hozen Brawler
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=86, `maxlevel`=86, `faction_A`=16, `faction_H`=16 WHERE `entry`=59605;
-- Inflamed Hozen Brawler
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=86, `maxlevel`=86, `faction_A`=16, `faction_H`=16 WHERE `entry`=56924;
-- Habanero Brew
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `faction_A`=190, `faction_H`=190 WHERE `entry`=56731;
-- Sleepy Hozen Brawler
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=86, `maxlevel`=86, `faction_A`=2136, `faction_H`=2136 WHERE `entry`=56863;
-- Drunken Hozen Brawler
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=86, `maxlevel`=86, `faction_A`=16, `faction_H`=16 WHERE `entry`=56862;
-- Hozen Party Animal
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=85, `maxlevel`=85, `faction_A`=2136, `faction_H`=2136 WHERE `entry`=56927;
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=85, `maxlevel`=85, `faction_A`=2136, `faction_H`=2136 WHERE `entry`=59684;
-- Hozen Bouncer
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=87, `maxlevel`=87, `faction_A`=190, `faction_H`=190 WHERE `entry`=56849;
-- Hozen Clinger
UPDATE `creature_template` SET `ScriptName`='npc_banana_hozen', `minlevel`=85, `maxlevel`=85, `faction_A`=2028, `faction_H`=2028 WHERE `entry`=60276;
-- Hopper
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=16, `faction_H`=16 WHERE `entry`=59464;
-- Hopling
UPDATE `creature_template` SET `minlevel`=90, `maxlevel`=90, `faction_A`=16, `faction_H`=16 WHERE `entry`=60208;
-- Nibbler
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=35, `faction_H`=35 WHERE `entry`=65392;
-- Bopper
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=16, `faction_H`=16 WHERE `entry`=59551;
-- Ancestral Brewmaster
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=35, `faction_H`=35 WHERE `entry`=59075;
