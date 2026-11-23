-- Lord Aurius Rivendare takes the place of Baron Rivendare at the end of the dungeon. The template, addon and loot
-- of entry 45412 were already in place but nothing spawned or scripted him, and the quest "Lord Aurius Rivendare"
-- needs his kill.
UPDATE `creature` SET `id` = 45412 WHERE `guid` = 54241 AND `id` IN (10440, 45412);

-- The spawn's addon row replaces the template addon, so the Unholy Aura and the mount go there.
UPDATE `creature_addon` SET `mount` = 10718, `auras` = '17467' WHERE `guid` = 54241;

UPDATE `creature_template` SET `ScriptName` = 'boss_baron_rivendare', `mechanic_immune_mask` = 8388624, `mingold` = 1997, `maxgold` = 1997 WHERE `entry` = 45412 AND `ScriptName` = '';

-- The finder credit follows the boss that stands in the room now.
UPDATE `instance_encounters` SET `creditEntry` = 45412 WHERE `entry` = 484 AND `creditType` = 0;

-- Emotes of Raise Dead and Death Pact.
DELETE FROM `creature_text` WHERE `entry` = 45412 AND `groupid` IN (0, 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45412, 0, 0, '%s raises an undead servant back to life!', 16, 0, 100, 0, 0, 0, 'Lord Aurius Rivendare - Raise Dead'),
(45412, 1, 0, '%s attempts to cast Death Pact on his servants!', 16, 0, 100, 0, 0, 0, 'Lord Aurius Rivendare - Death Pact');

-- Quest drops of "The Flesh Does Not Lie" and "Above and Beyond", as the Baron had them.
DELETE FROM `creature_loot_template` WHERE `entry` = 45412 AND `item` IN (13174, 13251);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45412, 13174, -80, 1, 0, 1, 1),
(45412, 13251, -100, 1, 0, 1, 1);

-- The skeletons of Raise Dead match his level and have no script behind their name.
UPDATE `creature_template` SET `minlevel` = 51, `maxlevel` = 51, `ScriptName` = '' WHERE `entry` = 11197;
