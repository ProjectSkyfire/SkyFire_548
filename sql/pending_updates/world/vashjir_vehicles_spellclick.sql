-- Vashj'ir: creatures that are clicked or ridden. Coilshell Sifter, Spiralung, the Dead Zin'jatar Raider and the Merciless
-- One have a spell click in the 5.4.8 data and none here, so the quest items and summons they give never came. Swiftfin
-- Seahorse starts the seahorse mount instruction (86358) instead of a ride on a vehicle that has no seats. Sixteen vehicle
-- templates (Watery Vision, Gnaws, Frenzied Reef Shark, Dominated Great Shark, the Zin'jatar Abductors, the tentacles,
-- Whirlpool, Anchor and the Horde camera bunny) had no vehicle id.
UPDATE `creature_template` SET `VehicleId` = 739 WHERE `entry` = 40370 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 808 WHERE `entry` = 41098 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 818 WHERE `entry` = 41294 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 569 WHERE `entry` IN (41809, 43180) AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 744 WHERE `entry` = 42013 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 855 WHERE `entry` = 42395 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 913 WHERE `entry` = 42678 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 553 WHERE `entry` = 42992 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 556 WHERE `entry` IN (42993, 42994) AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 698 WHERE `entry` IN (42995, 42996, 42997) AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 1023 WHERE `entry` = 44373 AND `VehicleId` = 0;
UPDATE `creature_template` SET `VehicleId` = 904 WHERE `entry` = 44536 AND `VehicleId` = 0;

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (39422, 39745, 39911, 41747);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (39422, 39745, 39911, 41747, 41776);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(39422, 74489, 3, 0),
(39745, 76337, 1, 0),
(39911, 75890, 1, 0),
(41747, 78004, 1, 0),
(41776, 86358, 1, 0);
