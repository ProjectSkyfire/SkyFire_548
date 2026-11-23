-- The Shattered Halls: the heroic executioner event. Kargath calls the executioner when the party leaves
-- Nethekurse's chamber (area trigger 4524); the executioner kills a prisoner after 55, then 10 and 15
-- minutes, and every living prisoner is a loot tier of his table. Data and scripts from TrinityCore 3.3.5.

-- The executioner has a script now instead of a SmartAI that only cast Cleave.
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_shattered_executioner' WHERE `entry` = 17301 AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `entryorguid` = 17301 AND `source_type` = 0;

DELETE FROM `areatrigger_scripts` WHERE `entry` = 4524;
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(4524, 'at_nethekurse_exit');

-- Kargath's Executioner (countdown aura) and its removal.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (39288, 39289, 39290, 39291);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(39288, 'spell_kargath_executioner'),
(39289, 'spell_kargath_executioner'),
(39290, 'spell_kargath_executioner'),
(39291, 'spell_remove_kargath_executioner');

-- Kargath's calls for the executioner (the third line of 3.3.5, the leash, is not used).
DELETE FROM `creature_text` WHERE `entry` = 16808 AND `groupid` IN (3, 4);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(16808, 3, 0, 'The Alliance dares to intrude this far into my fortress?  Bring out the Honor Hold prisoners and call for the executioner!  They''ll pay with their lives for this trespass!', 14, 0, 100, 0, 0, 0, 'Warchief Kargath Bladefist - Call executioner (Alliance)'),
(16808, 4, 0, 'Thrall''s false Horde dares to intrude this far into my fortress?  Bring out the Thrallmar prisoners and call for the executioner!  They''ll pay with their lives for this trespass!', 14, 0, 100, 0, 0, 0, 'Warchief Kargath Bladefist - Call executioner (Horde)');

-- The quest "Imprisoned in the Citadel" (Randy Whizzlesprocket / Drisella, taken in by Captain Alina /
-- Captain Boneshatter), the same in TrinityCore 3.3.5 and Legends of Azeroth 5.4.8. Randy is Alliance only;
-- the instance turns him into Drisella for a Horde party. Spawn only in heroic; position from AzerothCore
-- (TrinityCore has an unsniffed row 30 yards lower).
DELETE FROM `creature_queststarter` WHERE `id` IN (17288, 17294) AND `quest` IN (9524, 9525);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(17288, 9524),
(17294, 9525);
DELETE FROM `creature_questender` WHERE `id` IN (17290, 17296) AND `quest` IN (9524, 9525);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(17290, 9524),
(17296, 9525);

DELETE FROM `creature` WHERE `guid` = 8319160;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8319160, 17288, 540, 4, 0, 0, 0, 0, 120.88, 252.78, -14.57, 0.82, 7200, 0, 0, 0, 0, 0, 0, 0, 0);

-- Heroic executioner loot by hard mode (TrinityCore 3.3.5): loot mode 8 only with all three prisoners alive
-- (Badge of Justice and 2 to 3 primals), 12 with at most one executed (reference table 24726), 14 with at most
-- two executed (the axe for the quest and potions). Replaces the flat table SkyFire had.
DELETE FROM `creature_loot_template` WHERE `entry` = 20585;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(20585, 29434, 100, 8, 0, 1, 1),
(20585, 21884, 0, 8, 1, 2, 3),
(20585, 21885, 0, 8, 1, 2, 3),
(20585, 22451, 0, 8, 1, 2, 3),
(20585, 22452, 0, 8, 1, 2, 3),
(20585, 24726, 100, 12, 0, -24726, 1),
(20585, 31716, -100, 14, 0, 1, 1),
(20585, 22829, 0, 14, 2, 2, 8),
(20585, 22832, 0, 14, 2, 2, 8);
