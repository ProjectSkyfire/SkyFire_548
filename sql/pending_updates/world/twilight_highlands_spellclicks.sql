-- Twilight Highlands: spell clicks of the quest vehicles and objects. The world has click rows for 12 of the zone's creatures and
-- the 5.4.8 data has them for 27 more: the gryphons and drakes of the flight quests, the Krazz cannons, the Vermillion eggs, the
-- Salvageable Shredder, the Eye of Twilight, the cannon powder barrels, the altars and the Dragonmaw drums. Without them a click
-- does nothing, so the quests that need the ride or the click (credit by the vehicle or by the spell) cannot be finished. The
-- click flag and the vehicle id of the creatures that lacked them are added. Based on Legends of Azeroth 5.4.8.

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (42603,45435,45651,45682,46100,46706,47186,47241,47274,47316,47422,48116,48197,48476,49143,49680,49683,50426,50473,50599,50610,50635,50636,50638,50643,50655,52025);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42603, 90102, 1, 0),
(45435, 46990, 1, 0),
(45651, 85491, 1, 0),
(45682, 85491, 1, 0),
(46100, 65764, 1, 0),
(46706, 116554, 0, 0),
(47186, 46598, 1, 0),
(47241, 87513, 1, 0),
(47274, 88237, 1, 0),
(47316, 69641, 1, 0),
(47422, 88463, 1, 0),
(48116, 88237, 1, 0),
(48197, 46598, 1, 0),
(48476, 46598, 1, 0),
(49143, 91587, 2, 0),
(49680, 92793, 1, 0),
(49683, 46598, 1, 0),
(50426, 93523, 1, 0),
(50473, 46598, 1, 0),
(50599, 46598, 1, 0),
(50610, 93785, 1, 0),
(50635, 94271, 1, 0),
(50636, 94272, 1, 0),
(50638, 94275, 1, 0),
(50643, 94269, 1, 0),
(50655, 93800, 1, 0),
(52025, 46598, 1, 0);

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (45435,45651,45682,46100,46706,47241,47274,47316,47422,48116,48197,49143,49680,50426,50599,50610,50655,52025);
UPDATE `creature_template` SET `VehicleId` = 1221 WHERE `entry` = 46706;
UPDATE `creature_template` SET `VehicleId` = 1275 WHERE `entry` = 47422;
UPDATE `creature_template` SET `VehicleId` = 1518 WHERE `entry` = 50599;
UPDATE `creature_template` SET `VehicleId` = 1571 WHERE `entry` = 52025;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` IN (45435, 49143);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 45435, 46990, 0, 1, 9, 0, 27376, 0, 0, 0, 0, 0, '', 'Show the click while quest 27376 is taken'),
(18, 49143, 91587, 0, 0, 9, 0, 28597, 0, 0, 0, 0, 0, '', 'Show the click while quest 28597 is taken');
