-- Blasted Lands: the Loramus Thalipedes chain ("The Altar of Storms", "Time is Short", "Enhancing the Stone", the three
-- amulet quests, "You Are Rakh'likh, Demon" and the Alliance copies 26162 to 26171) could not be finished: Loramus, the
-- body, Razelikh, Loramus the Defiled, Chief Prospector Hondo and Elijah Dawnflight were not spawned, the Spirit of Grol
-- (the Amulet of Grol) was a friendly level 1 placeholder and Salt-Flop did not hand out the Amulet of Sevine. Dreadmaul
-- Cache 203204 (Loramus' Head) had no spawn and the three caches had no loot. Based on Legends of Azeroth 5.4.8, the
-- positions match the Wowhead MoP Classic spawn maps.

DELETE FROM `creature` WHERE `guid` BETWEEN 8316000 AND 8316007;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8316000, 7783, 0, 1, 0, 0, 0, 0, -11457, -2635.34, 4.0353, 1.6406, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316001, 7783, 0, 1, 0, 0, 0, 1, -11220.2, -3475.42, 9.8133, 4.6159, 180, 0, 0, 0, 0, 0, 0, 0, 0),
(8316002, 41173, 0, 1, 0, 0, 0, 0, -10642.4, -3114.52, 24.3005, -1.1329, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8316003, 41193, 0, 1, 0, 0, 0, 0, -11277.2, -2547.68, 107.563, 3.1081, 60, 0, 0, 0, 0, 0, 0, 0, 0),
(8316004, 41280, 0, 1, 0, 0, 0, 0, -11234.5, -2842.73, 158.034, 2.1991, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8316005, 41292, 0, 1, 0, 0, 0, 0, -11239, -2838.34, 158.053, 1.2704, 60, 3, 0, 0, 0, 1, 0, 0, 0),
(8316006, 42348, 0, 1, 0, 0, 0, 0, -12271.1, -2919.79, 97.5372, 1.4486, 300, 0, 0, 0, 0, 0, 0, 0, 0),
(8316007, 41267, 0, 1, 0, 0, 0, 0, -11445.2, -3800.71, -8.9307, 2.1642, 300, 3, 0, 0, 0, 1, 0, 0, 0);

-- Loramus offers quests and a greeting menu.
UPDATE `creature_template` SET `npcflag`=3, `gossip_menu_id`=11487 WHERE `entry`=7783;

-- Spirit of Grol: a hostile level 56 spirit that drops the Amulet of Grol for quests 25699 and 26169.
UPDATE `creature_template` SET `minlevel`=56, `maxlevel`=56, `faction_A`=45, `faction_H`=45, `unit_flags`=16448, `lootid`=41267,
 `mindmg`=100, `maxdmg`=132, `attackpower`=242, `dmg_multiplier`=2.4, `baseattacktime`=2000 WHERE `entry`=41267;
DELETE FROM `creature_loot_template` WHERE `entry`=41267;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(41267, 10753, -100, 1, 0, 1, 1);

-- Dreadmaul Caches: Loramus' Head, Torso and Legs for "Not Just Any Body".
UPDATE `gameobject_template` SET `Data1`=`entry` WHERE `entry` IN (203204, 203205, 203206);
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (203204, 203205, 203206);
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(203204, 55829, -100, 1, 0, 1, 1),
(203205, 55836, -100, 1, 0, 1, 1),
(203206, 55837, -100, 1, 0, 1, 1);
DELETE FROM `gameobject` WHERE `guid`=1068200;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(1068200, 203204, 0, 1, 0, 0, -11241, -2905.02, 10.7674, 2.16421, 0, 0, 0.882947, 0.469473, 300, 100, 1);

-- Salt-Flop gives the Amulet of Sevine for "The Amulet of Sevine" (25698, 26168) when asked for it.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=41265;
DELETE FROM `smart_scripts` WHERE `entryorguid`=41265 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(41265, 0, 0, 0, 62, 0, 100, 0, 11479, 0, 0, 0, 0, 56, 10754, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Salt-Flop - On Gossip Option Select - Add Item Amulet of Sevine');

-- The gossip options of the amulet quests only show to players who took the quest. Bloodmage Lynnore used the menu
-- 820, so her ritual option never showed; the 5.4.8 menus 11473 and 11613 have the single "Amulet Ritual" option.
UPDATE `creature_template` SET `gossip_menu_id`=11473 WHERE `entry`=7506;
DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (11473, 11613);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(11473, 0, 0, 'I would like to start the Amulet Ritual, Lynnore.', 41390, 1, 1, 0, 0, 0, 0, NULL, 0, 0),
(11613, 0, 0, 'I would like to start the Amulet Ritual, Kasim.', 42216, 1, 1, 0, 0, 0, 0, NULL, 0, 0);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup` IN (11479, 11473, 11613);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 11479, 0, 0, 0, 9, 0, 25698, 0, 0, 0, 0, '', 'Salt-Flop - Show option if the player has quest 25698'),
(15, 11479, 0, 0, 1, 9, 0, 26168, 0, 0, 0, 0, '', 'Salt-Flop - Show option if the player has quest 26168'),
(15, 11473, 0, 0, 0, 9, 0, 25700, 0, 0, 0, 0, '', 'Bloodmage Lynnore - Show the amulet ritual option if the player has quest 25700'),
(15, 11613, 0, 0, 0, 9, 0, 26170, 0, 0, 0, 0, '', 'Kasim Sharim - Show the amulet ritual option if the player has quest 26170');

-- The ritual: the NPC says two lines after the option is picked.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (7506, 42298);
DELETE FROM `creature_text` WHERE `entry` IN (7506, 42298);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7506, 0, 0, 'With the power within the amulets of Razelikh...', 12, 0, 100, 0, 0, 0, 'Bloodmage Lynnore - Amulet ritual'),
(7506, 1, 0, '... I bind you to his lair!', 12, 0, 100, 0, 0, 0, 'Bloodmage Lynnore - Amulet ritual'),
(42298, 0, 0, 'With the power within the amulets of Razelikh...', 12, 0, 100, 0, 0, 0, 'Kasim Sharim - Amulet ritual'),
(42298, 1, 0, '... I bind you to his lair!', 12, 0, 100, 0, 0, 0, 'Kasim Sharim - Amulet ritual');
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (7506, 42298) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(7506, 0, 0, 1, 62, 0, 100, 0, 11473, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Gossip Option Select - Set Data'),
(7506, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Gossip Option Select - Close Gossip'),
(7506, 0, 2, 0, 38, 0, 100, 0, 1, 0, 10000, 10000, 0, 1, 0, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Data Set - Say Line 0'),
(7506, 0, 3, 0, 52, 0, 100, 0, 0, 7506, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bloodmage Lynnore - On Text Over - Say Line 1'),
(42298, 0, 0, 1, 62, 0, 100, 0, 11613, 0, 0, 0, 0, 45, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Gossip Option Select - Set Data'),
(42298, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Gossip Option Select - Close Gossip'),
(42298, 0, 2, 0, 38, 0, 100, 0, 1, 0, 10000, 10000, 0, 1, 0, 5000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Data Set - Say Line 0'),
(42298, 0, 3, 0, 52, 0, 100, 0, 0, 42298, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kasim Sharim - On Text Over - Say Line 1');
