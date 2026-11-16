-- Hellfire Peninsula: the starters and enders of Dumphry's Request and the master profession quest (10764), the
-- quest targets Urtrak and Goliathon, and the Thornfang Ravager stats.

-- Citadel attunement and master profession quests: starters and enders
DELETE FROM `creature_queststarter` WHERE (`id` = 21209 AND `quest` = 10763) OR (`id` = 21209 AND `quest` = 10764);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(21209, 10763),
(21209, 10764);
DELETE FROM `creature_questender` WHERE (`id` = 21209 AND `quest` = 10763) OR (`id` = 21209 AND `quest` = 10764);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(21209, 10763),
(21209, 10764);

-- Quest targets Urtrak (Bloody Vengeance) and Goliathon (Natural Remedies) were not spawned
DELETE FROM `creature` WHERE `guid` IN (8317000,8317001);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8317000, 19862, 530, 1, 0, 0, 0, 0, -1192.20000, 2263.30000, 47.04720, 3.647430, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8317001, 19305, 530, 1, 0, 0, 0, 0, 108.76400, 4839.50000, 78.56260, 6.105520, 60, 0, 0, 0, 0, 0, 0, 0, 0);

-- Thornfang Ravager (Keep Thornfang Hill Clear) had no attack speed or damage; use the values of its sibling Quillfang Ravager
UPDATE `creature_template` SET `mindmg` = 181, `maxdmg` = 253, `attackpower` = 268, `baseattacktime` = 2000, `rangeattacktime` = 2000, `minrangedmg` = 147, `maxrangedmg` = 219, `rangedattackpower` = 32 WHERE `entry` = 19349 AND `mindmg` = 0 AND `maxdmg` = 0;
