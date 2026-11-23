-- Stormwind Stockade (map 34): boss credit order, money drops and prisoner flags

-- instance_encounters: Hogger and Lord Overheat had each other's credit creature
UPDATE `instance_encounters` SET `creditEntry`=46254 WHERE `entry`=1144 AND `creditType`=0;
UPDATE `instance_encounters` SET `creditEntry`=46264 WHERE `entry`=1145 AND `creditType`=0;

-- creature_template: the Stockade creatures dropped no money
UPDATE `creature_template` SET `mingold`=194, `maxgold`=194 WHERE `entry` IN (46248,46250,46260,46262,46375);
UPDATE `creature_template` SET `mingold`=195, `maxgold`=195 WHERE `entry` IN (46249,46251,46252,46261,46263,46379,46381,46382);
UPDATE `creature_template` SET `mingold`=301, `maxgold`=301 WHERE `entry`=46254;
UPDATE `creature_template` SET `mingold`=171, `maxgold`=171 WHERE `entry`=46264;
UPDATE `creature_template` SET `mingold`=188, `maxgold`=188 WHERE `entry`=46383;

-- creature_template: Imprisoned Thug and Imprisoned Thief were set to look dead (feign death flags)
UPDATE `creature_template` SET `unit_flags`=768, `unit_flags2`=2048, `dynamicflags`=0 WHERE `entry` IN (46299,46300);

-- creature_loot_template: Randolph Moloch's quest item drop
DELETE FROM `creature_loot_template` WHERE `entry`=46383 AND `item`=71638;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(46383, 71638, 1.9451, 1, 0, 1, 1);
