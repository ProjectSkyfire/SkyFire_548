-- Scarlet Halls (MoP dungeon revamp): replace placeholder creature templates (level 1, faction 35) with the 5.4.8
-- values from Legends of Azeroth. Only columns that differ are changed; each UPDATE is guarded by the current value
-- of every column it changes, so the file is safe to run twice. ScriptName and AIName are never touched.
-- Column mapping (SkyFire <- LoA): faction_A and faction_H <- faction, npc_rank <- rank, baseattacktime <- BaseAttackTime,
-- rangeattacktime <- RangeAttackTime, dmg_multiplier <- the normal-mode DamageModifier. flags_extra is masked with
-- SkyFire's allowed bits. Heroic and challenge scaling are left out (no table or code for them here).

-- 54030 Invisible Stalker
UPDATE `creature_template` SET `baseattacktime` = 2000, `rangeattacktime` = 2000, `flags_extra` = 128 WHERE `entry` = 54030 AND `baseattacktime` = 0 AND `rangeattacktime` = 0 AND `flags_extra` = 0;
-- 58632 Armsmaster Harlan
UPDATE `creature_template` SET `minlevel` = 31, `maxlevel` = 31, `faction_A` = 67, `faction_H` = 67, `baseattacktime` = 2500, `VehicleId` = 2307, `mechanic_immune_mask` = 769638399, `flags_extra` = 1, `dmg_multiplier` = 9 WHERE `entry` = 58632 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000 AND `VehicleId` = 0 AND `mechanic_immune_mask` = 0 AND `flags_extra` = 0 AND ABS(`dmg_multiplier` - 2) < 0.0001;
-- 58674 Angry Hound
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 168, `faction_H` = 168, `baseattacktime` = 1500, `unit_class` = 4, `VehicleId` = 2022 WHERE `entry` = 58674 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000 AND `unit_class` = 1 AND `VehicleId` = 0;
-- 58676 Scarlet Defender
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67 WHERE `entry` = 58676 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 58683 Scarlet Myrmidon
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `baseattacktime` = 1700 WHERE `entry` = 58683 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000;
-- 58684 Scarlet Scourge Hewer
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `baseattacktime` = 2500 WHERE `entry` = 58684 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000;
-- 58685 Scarlet Evangelist
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `unit_class` = 8 WHERE `entry` = 58685 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `unit_class` = 1;
-- 58701 Stake
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_flags` = 33554432 WHERE `entry` = 58701 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_flags` = 0;
-- 58725 Scarlet Cannons
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `flags_extra` = 128 WHERE `entry` = 58725 AND `minlevel` = 1 AND `maxlevel` = 1 AND `flags_extra` = 0;
-- 58726 Scarlet Cannon
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `faction_A` = 67, `faction_H` = 67, `flags_extra` = 128 WHERE `entry` = 58726 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `flags_extra` = 0;
-- 58727 Chained Dog Target
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 86, `faction_A` = 168, `faction_H` = 168, `family` = 0, `type` = 10, `flags_extra` = 128, `dmg_multiplier` = 3 WHERE `entry` = 58727 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `family` = 52 AND `type` = 1 AND `flags_extra` = 0 AND ABS(`dmg_multiplier` - 2) < 0.0001;
-- 58756 Scarlet Evoker
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `unit_class` = 8 WHERE `entry` = 58756 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `unit_class` = 1;
-- 58876 Starving Hound
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 16, `faction_H` = 16, `baseattacktime` = 1000 WHERE `entry` = 58876 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000;
-- 58898 Vigilant Watchman
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 58898 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 58998 Scarlet Defender
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 58998 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59150 Flameweaver Koegler
UPDATE `creature_template` SET `minlevel` = 31, `maxlevel` = 31, `faction_A` = 14, `faction_H` = 14, `baseattacktime` = 1500, `unit_class` = 2, `mechanic_immune_mask` = 769638399, `flags_extra` = 1, `dmg_multiplier` = 9 WHERE `entry` = 59150 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000 AND `unit_class` = 1 AND `mechanic_immune_mask` = 0 AND `flags_extra` = 0 AND ABS(`dmg_multiplier` - 2) < 0.0001;
-- 59155 Book Case
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 86, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 59155 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59163 Reinforced Archery Target
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_flags` = 537166592, `VehicleId` = 2085 WHERE `entry` = 59163 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_flags` = 0 AND `VehicleId` = 0;
-- 59175 Master Archer
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 2102, `faction_H` = 2102 WHERE `entry` = 59175 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59191 Commander Lindon
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 2102, `faction_H` = 2102, `dmg_multiplier` = 5 WHERE `entry` = 59191 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND ABS(`dmg_multiplier` - 2) < 0.0001;
-- 59198 Dragon Breath Target
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 86, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 59198 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59209 Books
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 86, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 59209 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59240 Scarlet Hall Guardian
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67 WHERE `entry` = 59240 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59241 Scarlet Treasurer
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67 WHERE `entry` = 59241 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59299 Scarlet Guardian
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 59299 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59302 Sergeant Verdone
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 59302 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35;
-- 59303 Houndmaster Braun
UPDATE `creature_template` SET `minlevel` = 31, `maxlevel` = 31, `faction_A` = 16, `faction_H` = 16, `mechanic_immune_mask` = 769638399, `flags_extra` = 1, `dmg_multiplier` = 9 WHERE `entry` = 59303 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `mechanic_immune_mask` = 0 AND `flags_extra` = 0 AND ABS(`dmg_multiplier` - 2) < 0.0001;
-- 59309 Obedient Hound
UPDATE `creature_template` SET `minlevel` = 30, `maxlevel` = 30, `faction_A` = 16, `faction_H` = 16, `baseattacktime` = 1000 WHERE `entry` = 59309 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `baseattacktime` = 2000;
-- 59372 Scarlet Scholar
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `unit_class` = 2 WHERE `entry` = 59372 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `unit_class` = 1;
-- 59373 Scarlet Pupil
UPDATE `creature_template` SET `minlevel` = 29, `maxlevel` = 29, `faction_A` = 67, `faction_H` = 67, `unit_class` = 2 WHERE `entry` = 59373 AND `minlevel` = 1 AND `maxlevel` = 1 AND `faction_A` = 35 AND `faction_H` = 35 AND `unit_class` = 1;
-- 59683 Exploding Shot Stalker
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `unit_flags` = 33685504, `flags_extra` = 0 WHERE `entry` = 59683 AND `faction_A` = 35 AND `faction_H` = 35 AND `unit_flags` = 0 AND `flags_extra` = 128;
-- 60908 Archery Target Base
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_flags` = 33554432 WHERE `entry` = 60908 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_flags` = 0;
-- 64738 Hooded Crusader
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_class` = 2 WHERE `entry` = 64738 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_class` = 1;
-- 65379 Bucket of Meaty Dog Food
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90 WHERE `entry` = 65379 AND `minlevel` = 1 AND `maxlevel` = 1;
-- 65546 Sekrit
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_class` = 4 WHERE `entry` = 65546 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_class` = 1;
-- 65547 Sekret
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `unit_class` = 4 WHERE `entry` = 65547 AND `minlevel` = 1 AND `maxlevel` = 1 AND `unit_class` = 1;
-- 66091 Referee
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `flags_extra` = 128 WHERE `entry` = 66091 AND `minlevel` = 1 AND `maxlevel` = 1 AND `flags_extra` = 0;
