UPDATE `creature_template` SET `ScriptName`='boss_chi_ji', `WDBVerified`='15585' WHERE `entry`=71952;
UPDATE `creature_template` SET `ScriptName`='boss_galleon' WHERE `entry`=62346;
UPDATE `creature_template` SET `ScriptName`='npc_galleon' WHERE `entry`=62351;

-- Model Fix
DELETE FROM `creature_model_info` WHERE `modelid`=47227;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES (47227, 1.5, 25, 2, 0);
-- Nalak World Boss
DELETE FROM `creature_template` WHERE `entry`=69099;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (69099, 0, 0, 0, 0, 0, 47227, 0, 0, 0, 'Nalak', 'The Storm Lord', NULL, 0, 93, 93, 4, 0, 14, 14, 2, 1.14286, 1.14286, 1, 3, 120000, 160000, 0, 150, 2, 2000, 2000, 1, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2097260, 0, 69099, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 4, 700, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 786382847, 0, 'boss_nalak', 17614);

-- Ordos Database Text
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=0 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 0, 0, 'You will take my place on the eternal brazier.', 14, 0, 100, 0, 0, 38971, 'ORDOS - SAY_AGGRO');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=1 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 1, 0, 'The eternal fire will never be extinguished.', 14, 0, 100, 0, 0, 38972, 'ORDOS - SAY_DEATH');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=2 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 2, 0, 'Up in smoke.', 14, 0, 100, 0, 0, 38977, 'ORDOS - SAY_SLAY');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=3 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 3, 0, 'Your flesh will melt.', 14, 0, 100, 0, 0, 38978, 'ORDOS - SAY_ANCIENT_FLAME');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=4 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 4, 0, 'Your pain will be endless.', 14, 0, 100, 0, 0, 38979, 'ORDOS - SAY_ETERNAL_AGONY');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=5 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 5, 0, 'You will feel but a fraction of my agony.', 14, 0, 100, 0, 0, 38980, 'ORDOS - SAY_POOL_OF_FIRE');
DELETE FROM `creature_text` WHERE `entry`=72057 AND `groupid`=6 AND `id`=0;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (72057, 6, 0, 'Burn!', 14, 0, 100, 0, 0, 38981, 'ORDOS - SAY_BURNING_SOUL');
