-- Wrathion spawn
SET @CGUID := 200000;
DELETE FROM `creature` WHERE `id`=64616;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`) VALUES
(@CGUID-0, 64616, 870, 1, 817.122, -162.944, 415.223, 5.03566, 300);

-- Wrathion creature_template
REPLACE INTO `creature_template` (`entry`, `modelid1`, `NAME`, `subname`, `gossip_menu_id`, `minlevel`, `maxlevel`, `EXP`, `faction_a`, `faction_h`, `npcflag`, `scale`, `npc_rank`, `mindmg`, `maxdmg`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `inhabittype`, `health_mod`, `mana_mod`, `TYPE`, `WDBVerified`) VALUES
(64616, 47214, 'Wrathion', 'The Black Prince', 14936, 90, 90, 4, 2598, 2598, 2, 1, 1, 11839, 17339, 45299, 2, 2000, 2000, 1, 33280, 3, 6, 6, 2, 18414);

-- Wrathion creature_model_info
DELETE FROM `creature_model_info` WHERE `modelid`=47214;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES
(47214, 1, 1.5, 0, 0);

-- Wrathion creature_queststarter
DELETE FROM `creature_queststarter` WHERE `id`=64616;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES 
(64616, 31454),
(64616, 31468),
(64616, 31473),
(64616, 31481),
(64616, 31482),
(64616, 31483),
(64616, 32373),
(64616, 32388),
(64616, 32389),
(64616, 32390),
(64616, 32427),
(64616, 32430),
(64616, 32431),
(64616, 32432),
(64616, 32457),
(64616, 32474),
(64616, 32476),
(64616, 32590);

-- Wrathion creature_questender
DELETE FROM `creature_questender` WHERE `id`=64616;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(64616, 31454),
(64616, 31468),
(64616, 31473),
(64616, 31481),
(64616, 31483),
(64616, 31488),
(64616, 31489),
(64616, 32373),
(64616, 32388),
(64616, 32389),
(64616, 32427),
(64616, 32430),
(64616, 32431),
(64616, 32474),
(64616, 32476);
