-- Kezan: the order of Rolling with my Homies, Off to the Bank, The New You and Life of the Party, and the pieces that
-- make each of them completable. Based on Legends of Azeroth 5.4.8 (chain, conditions, spawns, Partygoer scripts, vendor
-- scripts, spell areas); Ported from The Cataclysm Preservation Project 4.3.4 (Life of the Party exclusive group, vendor
-- gossip conditions). Safe to run twice: every block deletes its own keys first or sets absolute values.


-- Chain: Off to the Bank follows Rolling with my Homies; Life of the Party follows Do it Yourself (the other two
-- requirements, The New You and Give Sassy the News, are conditions below). 4.3.4 makes the two Life of the Party
-- versions one exclusive group.
UPDATE `quest_template` SET `PrevQuestId` = 14071 WHERE `Id` IN (26711, 26712);
UPDATE `quest_template` SET `PrevQuestId` = 14070, `ExclusiveGroup` = 14113 WHERE `Id` IN (14113, 14153);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 19 AND `SourceGroup` = 0 AND `SourceEntry` IN (14113, 14153);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 14113, 0, 0, 8, 0, 14109, 0, 0, 0, 0, 0, '', 'Life of the Party - The New You (male) rewarded'),
(19, 0, 14113, 0, 0, 8, 0, 24520, 0, 0, 0, 0, 0, '', 'Life of the Party - Give Sassy the News rewarded'),
(19, 0, 14113, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, '', 'Life of the Party - male only'),
(19, 0, 14153, 0, 0, 8, 0, 14110, 0, 0, 0, 0, 0, '', 'Life of the Party - The New You (female) rewarded'),
(19, 0, 14153, 0, 0, 8, 0, 24520, 0, 0, 0, 0, 0, '', 'Life of the Party - Give Sassy the News rewarded'),
(19, 0, 14153, 0, 0, 20, 0, 1, 0, 0, 0, 0, 0, '', 'Life of the Party - female only');


-- Rolling with my Homies: Izzy, Ace and Gobber (the credit NPCs 34890, 34892 and 34954) give their pickup credit to a
-- player or Hot Rod passengers that come within 40 yards; they had no script. Ace 34892 had no spawn.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (34890, 34892, 34954);
DELETE FROM `creature` WHERE `guid` = 8334000;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8334000, 34892, 648, 1, 0, 0, 0, 0, -8068.13, 1482.03, 9.01469, 3.59538, 120, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (34890, 34892, 34954);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(34890, 0, 0, 0, 10, 0, 100, 0, 1, 40, 0, 0, 0, 33, 34959, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Izzy - Out of Combat Line of Sight - Give Izzy Picked Up Credit'),
(34892, 0, 0, 0, 10, 0, 100, 0, 1, 40, 0, 0, 0, 33, 34957, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ace - Out of Combat Line of Sight - Give Ace Picked Up Credit'),
(34954, 0, 0, 0, 10, 0, 100, 0, 1, 40, 0, 0, 0, 33, 34958, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gobber - Out of Combat Line of Sight - Give Gobber Picked Up Credit');


-- The New You: Gappy Silvertooth, Szabo and Missa Spekkies only had a gossip option and no script, so nobody sold the
-- Shiny Bling, Hip New Outfit and Cool Shades. 5.4.8: the player casts the item spell on the vendor and the gossip
-- closes; the option shows only on The New You and only while the item is missing (4.3.4 conditions, with the item
-- check also on the male branch, where 4.3.4 forgot it). The 5.4.8 option has no follow-up menu, so the old one is cleared
-- (it would reopen the dialog after the script closed it). The option is also given its gossip type and NPC flag, without
-- which it does not show.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (35126, 35128, 35130);
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1, `ActionMenuID` = 0 WHERE `MenuID` IN (10620, 10622, 10624) AND `OptionID` = 0;

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (35126, 35128, 35130);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35126, 0, 0, 1, 62, 0, 100, 0, 10620, 0, 0, 0, 0, 86, 66780, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gappy Silvertooth - On Gossip Option 0 Selected - Player Casts Create Shiny Bling'),
(35126, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Gappy Silvertooth - On Gossip Option 0 Selected - Close Gossip'),
(35128, 0, 0, 1, 62, 0, 100, 0, 10622, 0, 0, 0, 0, 86, 66781, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Szabo - On Gossip Option 0 Selected - Player Casts Create Hip New Outfit'),
(35128, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Szabo - On Gossip Option 0 Selected - Close Gossip'),
(35130, 0, 0, 1, 62, 0, 100, 0, 10624, 0, 0, 0, 0, 86, 66782, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Missa Spekkies - On Gossip Option 0 Selected - Player Casts Create Cool Shades'),
(35130, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Missa Spekkies - On Gossip Option 0 Selected - Close Gossip');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (10620, 10622, 10624) AND `SourceEntry` = 0;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10620, 0, 0, 0, 2, 0, 47045, 1, 1, 1, 0, 0, '', 'The New You - Shiny Bling only without the item'),
(15, 10620, 0, 0, 0, 9, 0, 14110, 0, 0, 0, 0, 0, '', 'The New You - Shiny Bling only on The New You (female)'),
(15, 10620, 0, 0, 1, 2, 0, 47045, 1, 1, 1, 0, 0, '', 'The New You - Shiny Bling only without the item'),
(15, 10620, 0, 0, 1, 9, 0, 14109, 0, 0, 0, 0, 0, '', 'The New You - Shiny Bling only on The New You (male)'),
(15, 10622, 0, 0, 0, 2, 0, 47046, 1, 1, 1, 0, 0, '', 'The New You - Hip New Outfit only without the item'),
(15, 10622, 0, 0, 0, 9, 0, 14110, 0, 0, 0, 0, 0, '', 'The New You - Hip New Outfit only on The New You (female)'),
(15, 10622, 0, 0, 1, 2, 0, 47046, 1, 1, 1, 0, 0, '', 'The New You - Hip New Outfit only without the item'),
(15, 10622, 0, 0, 1, 9, 0, 14109, 0, 0, 0, 0, 0, '', 'The New You - Hip New Outfit only on The New You (male)'),
(15, 10624, 0, 0, 0, 2, 0, 47047, 1, 1, 1, 0, 0, '', 'The New You - Cool Shades only without the item'),
(15, 10624, 0, 0, 0, 9, 0, 14110, 0, 0, 0, 0, 0, '', 'The New You - Cool Shades only on The New You (female)'),
(15, 10624, 0, 0, 1, 2, 0, 47047, 1, 1, 1, 0, 0, '', 'The New You - Cool Shades only without the item'),
(15, 10624, 0, 0, 1, 9, 0, 14109, 0, 0, 0, 0, 0, '', 'The New You - Cool Shades only on The New You (male)');

-- Life of the Party: the Kezan Partygoers (35175 and 35186) were not spawned, had no script and 35175 offered both
-- quest versions to everyone as a quest giver. Spawns, template flags and SmartAI are the 5.4.8 ones, with two changes
-- for this core: the kill credit is given from the spell hit (the 5.4.8 version gave it from an aura event, which has no
-- player in this core), and a guest that is already happy ignores further party favors, so a guest counts once. The
-- party spells (66909 to 66913) come from the Awesome Party Ensemble aura 66908, which the outfit spells 66927 (female)
-- and 66928 (male) apply while the quest is in the log (5.4.8 spell areas and linked spells). Guests are not phased
-- (the world here has no Kezan phases); they stand in the party area from the start.
UPDATE `creature_template` SET `npcflag` = 0, `AIName` = 'SmartAI' WHERE `entry` = 35175;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 35186;
DELETE FROM `creature_queststarter` WHERE `id` = 35175 AND `quest` IN (14113, 14153);

DELETE FROM `spell_area` WHERE `spell` IN (66927, 66928, 91551) AND `area` = 4737;
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(66927, 4737, 14153, 14153, 0, 0, 1, 1, 74, 11),
(66928, 4737, 14113, 14113, 0, 0, 0, 1, 74, 11),
(91551, 4737, 14071, 14125, -66908, 0, 2, 1, 74, 11);
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (66927, 66928) AND `spell_effect` = 66908 AND `type` = 2;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(66927, 66908, 2, 'Life Of The Party: Outfit - Female - Awesome Party Ensemble'),
(66928, 66908, 2, 'Life of the Party: Outfit - Male - Awesome Party Ensemble');

DELETE FROM `creature` WHERE `guid` BETWEEN 8334010 AND 8334075;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8334010, 35175, 648, 1, 0, 0, 29666, 0, -8514.08, 1310.49, 101.822, 3.83153, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334011, 35175, 648, 1, 0, 0, 29667, 0, -8523.35, 1304.36, 102.514, 5.3058, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334012, 35175, 648, 1, 0, 0, 29630, 0, -8493.72, 1378.5, 101.786, 0.0946573, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                       ,
(8334013, 35175, 648, 1, 0, 0, 29666, 0, -8506.33, 1378.9, 101.822, 6.08581, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334014, 35175, 648, 1, 0, 0, 29667, 0, -8460.97, 1314.61, 101.822, 2.84489, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334015, 35175, 648, 1, 0, 0, 29632, 0, -8515.76, 1309.21, 102.057, 0.453786, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                       ,
(8334016, 35175, 648, 1, 0, 0, 29666, 0, -8483.39, 1379.09, 101.78, 5.95157, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334017, 35175, 648, 1, 0, 0, 29632, 0, -8473.61, 1374.94, 101.78, 1.20428, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334018, 35175, 648, 1, 0, 0, 29667, 0, -8517.14, 1366.8, 101.78, 1.37881, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                          ,
(8334019, 35175, 648, 1, 0, 0, 29667, 0, -8509.48, 1364.36, 101.78, 1.55334, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334020, 35175, 648, 1, 0, 0, 29667, 0, -8491.55, 1377.58, 101.78, 1.85005, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334021, 35175, 648, 1, 0, 0, 29667, 0, -8503.37, 1379.1, 101.871, 3.97935, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334022, 35175, 648, 1, 0, 0, 29667, 0, -8505.14, 1377.04, 101.81, 0.942478, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334023, 35175, 648, 1, 0, 0, 29666, 0, -8509.19, 1352.64, 101.78, 1.25664, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334024, 35175, 648, 1, 0, 0, 29667, 0, -8515.31, 1380.7, 101.78, 6.17846, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                          ,
(8334025, 35175, 648, 1, 0, 0, 29666, 0, -8455.39, 1325.8, 101.78, 3.10669, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                          ,
(8334026, 35175, 648, 1, 0, 0, 29667, 0, -8452.2, 1313.13, 102.091, 1.29154, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334027, 35175, 648, 1, 0, 0, 29666, 0, -8472.32, 1308.21, 101.783, 4.29351, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334028, 35175, 648, 1, 0, 0, 29632, 0, -8504.13, 1300.22, 102.304, 2.28638, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334029, 35175, 648, 1, 0, 0, 29630, 0, -8505.64, 1302.1, 102.248, 5.34071, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334030, 35175, 648, 1, 0, 0, 29667, 0, -8481.64, 1307.82, 101.78, 4.04916, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334031, 35175, 648, 1, 0, 0, 29630, 0, -8484.33, 1307.94, 101.78, 5.63741, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334032, 35175, 648, 1, 0, 0, 29632, 0, -8508.08, 1327.76, 101.78, 3.80482, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334033, 35175, 648, 1, 0, 0, 29632, 0, -8503.13, 1339.53, 101.78, 3.61283, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334034, 35175, 648, 1, 0, 0, 29666, 0, -8506.96, 1353.26, 101.78, 2.18166, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334035, 35175, 648, 1, 0, 0, 29630, 0, -8506.05, 1354.55, 101.78, 2.80998, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334036, 35175, 648, 1, 0, 0, 29630, 0, -8462.52, 1315.24, 101.78, 6.26573, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334037, 35175, 648, 1, 0, 0, 29667, 0, -8459.48, 1349.18, 101.78, 6.07375, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334038, 35175, 648, 1, 0, 0, 29667, 0, -8458.83, 1346.29, 101.78, 1.15192, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334039, 35175, 648, 1, 0, 0, 29666, 0, -8460.79, 1367.3, 101.78, 0.593412, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334040, 35175, 648, 1, 0, 0, 29632, 0, -8452.95, 1333.82, 101.8, 0.680678, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334041, 35175, 648, 1, 0, 0, 29667, 0, -8449, 1334.82, 102.036, 3.7001, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                            ,
(8334042, 35175, 648, 1, 0, 0, 29632, 0, -8474.07, 1307.28, 101.805, 6.0912, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334043, 35175, 648, 1, 0, 0, 29667, 0, -8480.81, 1305.31, 101.78, 2.89725, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334044, 35175, 648, 1, 0, 0, 29632, 0, -8506.91, 1316.49, 101.78, 3.92699, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334045, 35186, 648, 1, 0, 0, 29633, 0, -8506.14, 1366.97, 101.78, 2.79253, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334046, 35186, 648, 1, 0, 0, 29669, 0, -8514.45, 1354.82, 101.814, 5.11381, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334047, 35186, 648, 1, 0, 0, 29668, 0, -8522.73, 1303.39, 102.514, 2.28638, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334048, 35186, 648, 1, 0, 0, 29668, 0, -8481.74, 1378.75, 101.786, 2.94961, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334049, 35186, 648, 1, 0, 0, 29633, 0, -8457, 1349.96, 101.749, 4.44571, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                           ,
(8334050, 35186, 648, 1, 0, 0, 29633, 0, -8509.01, 1356.71, 101.78, 4.99164, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334051, 35186, 648, 1, 0, 0, 29633, 0, -8459.01, 1368.94, 101.78, 4.08407, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334052, 35186, 648, 1, 0, 0, 29633, 0, -8472.75, 1377.01, 101.78, 4.38078, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334053, 35186, 648, 1, 0, 0, 29668, 0, -8492.28, 1380.14, 101.78, 4.99164, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334054, 35186, 648, 1, 0, 0, 29669, 0, -8509.26, 1368.15, 101.78, 5.11381, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334055, 35186, 648, 1, 0, 0, 29633, 0, -8513.19, 1309.09, 101.793, 3.05433, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334056, 35186, 648, 1, 0, 0, 29669, 0, -8516.92, 1368.93, 101.78, 4.62512, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334057, 35186, 648, 1, 0, 0, 29668, 0, -8458.04, 1324.47, 101.78, 0.785398, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334058, 35186, 648, 1, 0, 0, 29631, 0, -8451.89, 1332.77, 101.827, 0.680678, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                       ,
(8334059, 35186, 648, 1, 0, 0, 29668, 0, -8450.01, 1335.9, 102.021, 3.76991, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334060, 35186, 648, 1, 0, 0, 29669, 0, -8450.53, 1315.14, 102.042, 3.76991, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334061, 35186, 648, 1, 0, 0, 29631, 0, -8474.07, 1307.28, 101.805, 6.0912, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334062, 35186, 648, 1, 0, 0, 29668, 0, -8483.19, 1305.74, 101.78, 0.925025, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334063, 35186, 648, 1, 0, 0, 29633, 0, -8480.81, 1305.31, 101.78, 2.89725, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334064, 35186, 648, 1, 0, 0, 29633, 0, -8506.91, 1316.49, 101.78, 3.92699, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334065, 35186, 648, 1, 0, 0, 29633, 0, -8508.44, 1325.89, 101.78, 1.90241, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334066, 35186, 648, 1, 0, 0, 29668, 0, -8505.4, 1338.49, 101.78, 0.610865, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334067, 35186, 648, 1, 0, 0, 29669, 0, -8455.09, 1360.16, 101.878, 5.51524, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334068, 35186, 648, 1, 0, 0, 29669, 0, -8456.2, 1347.52, 101.833, 2.58309, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334069, 35186, 648, 1, 0, 0, 29633, 0, -8462.21, 1373.2, 101.78, 5.89921, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                          ,
(8334070, 35186, 648, 1, 0, 0, 29631, 0, -8452.91, 1357.92, 101.982, 2.37365, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                        ,
(8334071, 35186, 648, 1, 0, 0, 29668, 0, -8449, 1334.82, 102.036, 3.7001, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                            ,
(8334072, 35186, 648, 1, 0, 0, 29631, 0, -8458.22, 1327.04, 101.78, 5.72468, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334073, 35186, 648, 1, 0, 0, 29631, 0, -8452.95, 1333.82, 101.8, 0.680678, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334074, 35186, 648, 1, 0, 0, 29668, 0, -8462.52, 1315.24, 101.78, 6.26573, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ,
(8334075, 35186, 648, 1, 0, 0, 29631, 0, -8508.08, 1327.76, 101.78, 3.80482, 60, 0, 0, 0, 0, 0, 0, 0, 0)                                                                                                                         ;

DELETE FROM `creature_text` WHERE `entry` IN (35175, 35186);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(35175, 0, 0, 'I could really use a refill on my drink here.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                                        ,
(35175, 0, 1, 'This sparkling white wine is delicious! Wherever did you get it?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                     ,
(35175, 1, 0, 'Thanks for the refill, $g sir : ma''am;!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                                             ,
(35175, 1, 1, 'A fresh glass of bubbly. Just what the doctor ordered, $n.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                           ,
(35175, 2, 0, 'Ugh... I need a bucket!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                                                                              ,
(35175, 2, 1, 'I think this one will have to be my last. I''m driving home after the party.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                         ,
(35175, 3, 0, 'Oh, my head hurtsh.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                                                                                  ,
(35175, 3, 1, 'Shorry about your shoes.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                                                                             ,
(35175, 4, 0, 'If only I had someone to dance with.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Dance')                                                                                  ,
(35175, 5, 0, 'We should totally have a pants-off dance-off!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Dance')                                                                         ,
(35175, 5, 1, 'A disco ball?! Groovy!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Dance')                                                                                                ,
(35175, 6, 0, 'I love fireworks!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Fireworks')                                                                                                 ,
(35175, 6, 1, 'Wow! That sure beats this puny, little sparkler!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Fireworks')                                                                  ,
(35175, 7, 0, 'Best... Party... Evar!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Fireworks')                                                                                            ,
(35175, 7, 1, 'Woo hoo, fireworks! More, more!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Fireworks')                                                                                   ,
(35175, 8, 0, 'This is delicious! Are there more hors d''oeuvres?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Hors D''oeuvres')                                                          ,
(35175, 8, 1, 'Nom, nom, nom!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Hors D''oeuvres')                                                                                              ,
(35175, 9, 0, 'Thanks. I was almost out. So hungry!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Hors D''oeuvres')                                                                        ,
(35175, 9, 1, 'Finger licking good!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Hors D''oeuvres')                                                                                        ,
(35186, 0, 0, 'If only I had someone to dance with.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                          ,
(35186, 0, 1, 'This sparkling white wine is delicious! Wherever did you get it?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                     ,
(35186, 1, 0, 'Ugh... I need a bucket!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                       ,
(35186, 1, 1, 'A fresh glass of bubbly. Just what the doctor ordered, $n.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bubbly')                                                           ,
(35186, 2, 0, 'I could really use a refill on my drink here.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                 ,
(35186, 2, 1, 'I think this one will have to be my last. I''m driving home after the party.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                         ,
(35186, 3, 0, 'I love fireworks!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                             ,
(35186, 3, 1, 'Thanksh!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer - Bucket')                                                                                                             ,
(35186, 4, 0, 'This is delicious! Are there more hors d''oeuvres?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                            ,
(35186, 5, 0, 'A fresh glass of bubbly. Just what the doctor ordered, $n.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                    ,
(35186, 5, 1, 'Thanks for the refill, $g sir : ma''am;!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                      ,
(35186, 5, 2, 'This sparkling white wine is delicious! Wherever did you get it?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                              ,
(35186, 5, 3, 'I think this one will have to be my last. I''m driving home after the party.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                  ,
(35186, 5, 4, 'Thanksh!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                                      ,
(35186, 6, 0, 'I feel much better now...hic!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                 ,
(35186, 6, 1, 'Oh, my head hurtsh.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                           ,
(35186, 6, 2, 'Shorry about your shoes.', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                      ,
(35186, 7, 0, 'A disco ball?! Groovy!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                        ,
(35186, 7, 1, 'How do you like my moves, $n?', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                 ,
(35186, 7, 2, 'We should totally have a pants-off dance-off!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                 ,
(35186, 7, 3, 'Shake it like a goblinoid picture!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                            ,
(35186, 8, 0, 'Wow! That sure beats this puny, little sparkler!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                              ,
(35186, 8, 1, 'You really stop at no expense, $n! Amazing!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                   ,
(35186, 8, 2, 'Best... Party... Evar!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                        ,
(35186, 8, 3, 'Woo hoo, fireworks! More, more!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                               ,
(35186, 9, 0, 'Thanks. I was almost out. So hungry!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                          ,
(35186, 9, 1, 'These are the most delicious hors d''oeurves I have ever tasted. You must share your recipe, $n!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                              ,
(35186, 9, 2, 'Finger licking good!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                          ,
(35186, 9, 3, 'Nom, nom, nom!', 12, 0, 100, 0, 0, 0, 'Kezan Partygoer')                                                                                                                ;

DELETE FROM `smart_scripts` WHERE (`source_type` = 0 AND `entryorguid` IN (35175, 35186)) OR (`source_type` = 9 AND `entryorguid` BETWEEN 3517501 AND 3517505);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(35175, 0, 0, 1, 8, 0, 100, 0, 66909, 0, 0, 0, 0, 28, 75042, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Bubbly - Remove Aura Bubbly'),
(35175, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35175, 0, 2, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 1'),
(35175, 0, 3, 4, 8, 0, 100, 0, 66910, 0, 0, 0, 0, 28, 75044, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Bucket - Remove Aura Bucket'),
(35175, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35175, 0, 5, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 3'),
(35175, 0, 6, 7, 8, 0, 100, 0, 66911, 0, 0, 0, 0, 28, 75046, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Dance - Remove Aura Dance'),
(35175, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35175, 0, 8, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 5'),
(35175, 0, 9, 10, 8, 0, 100, 0, 66912, 0, 0, 0, 0, 28, 75048, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Fireworks - Remove Aura Fireworks'),
(35175, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35175, 0, 11, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 7'),
(35175, 0, 12, 13, 8, 0, 100, 0, 66913, 0, 0, 0, 0, 28, 75050, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Hors D''oeuvres - Remove Aura Hors D''oeuvres'),
(35175, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35175, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 9'),
(35175, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 35175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Give Kill Credit'),
(35175, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Forced Despawn'),
(35175, 0, 17, 0, 23, 0, 100, 0, 75042, 1, 60000, 60000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Bubbly - Say Text 0'),
(35175, 0, 18, 0, 23, 0, 100, 0, 75044, 1, 60000, 60000, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Bucket - Say Text 2'),
(35175, 0, 19, 0, 23, 0, 100, 0, 75046, 1, 60000, 60000, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Dance - Say Text 4'),
(35175, 0, 20, 0, 23, 0, 100, 0, 75048, 1, 60000, 60000, 0, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Fireworks - Say Text 6'),
(35175, 0, 21, 0, 23, 0, 100, 0, 75050, 1, 60000, 60000, 0, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Hors D''oeuvres - Say Text 8'),
(35175, 0, 22, 0, 1, 0, 100, 1, 1, 10000, 0, 0, 0, 87, 3517501, 3517502, 3517503, 3517504, 3517505, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Out of Combat - Random Actionlist'),
(35186, 0, 0, 1, 8, 0, 100, 0, 66909, 0, 0, 0, 0, 28, 75042, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Bubbly - Remove Aura Bubbly'),
(35186, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35186, 0, 2, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 5'),
(35186, 0, 3, 4, 8, 0, 100, 0, 66910, 0, 0, 0, 0, 28, 75044, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Bucket - Remove Aura Bucket'),
(35186, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35186, 0, 5, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 6, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 6'),
(35186, 0, 6, 7, 8, 0, 100, 0, 66911, 0, 0, 0, 0, 28, 75046, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Dance - Remove Aura Dance'),
(35186, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35186, 0, 8, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 7'),
(35186, 0, 9, 10, 8, 0, 100, 0, 66912, 0, 0, 0, 0, 28, 75048, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Fireworks - Remove Aura Fireworks'),
(35186, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35186, 0, 11, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 8, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 8'),
(35186, 0, 12, 13, 8, 0, 100, 0, 66913, 0, 0, 0, 0, 28, 75050, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Spellhit Hors D''oeuvres - Remove Aura Hors D''oeuvres'),
(35186, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 66916, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Cast Aura Happy Partygoer'),
(35186, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 9, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Say Text 9'),
(35186, 0, 15, 16, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 35175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Give Kill Credit'),
(35186, 0, 16, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Forced Despawn'),
(35186, 0, 17, 0, 23, 0, 100, 0, 75042, 1, 60000, 60000, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Bubbly - Say Text 2'),
(35186, 0, 18, 0, 23, 0, 100, 0, 75044, 1, 60000, 60000, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Bucket - Say Text 1'),
(35186, 0, 19, 0, 23, 0, 100, 0, 75046, 1, 60000, 60000, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Dance - Say Text 0'),
(35186, 0, 20, 0, 23, 0, 100, 0, 75048, 1, 60000, 60000, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Fireworks - Say Text 3'),
(35186, 0, 21, 0, 23, 0, 100, 0, 75050, 1, 60000, 60000, 0, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Aura Hors D''oeuvres - Say Text 4'),
(35186, 0, 22, 0, 1, 0, 100, 1, 1, 10000, 0, 0, 0, 87, 3517501, 3517502, 3517503, 3517504, 3517505, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - Out of Combat - Random Actionlist'),
(3517501, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75042, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Cast Aura Bubbly'),
(3517501, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 423, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Play Emote 423'),
(3517502, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75044, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Cast Aura Bucket'),
(3517502, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 64, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Play Emote 64'),
(3517503, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75046, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Cast Aura Dance'),
(3517503, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Play Emote 10'),
(3517504, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75048, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Cast Aura Fireworks'),
(3517504, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 333, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Play Emote 333'),
(3517505, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75050, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Cast Aura Hors D''oeuvres'),
(3517505, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 5, 423, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Kezan Partygoer - On Script - Play Emote 423');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceId` = 0 AND `SourceEntry` IN (35175, 35186) AND `SourceGroup` IN (1, 4, 7, 10, 13);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 1, 35175, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 0: Bubbly only counts if the guest is not already happy'),
(22, 4, 35175, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 3: Bucket only counts if the guest is not already happy'),
(22, 7, 35175, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 6: Dance only counts if the guest is not already happy'),
(22, 10, 35175, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 9: Fireworks only counts if the guest is not already happy'),
(22, 13, 35175, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 12: Hors D''oeuvres only counts if the guest is not already happy'),
(22, 1, 35186, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 0: Bubbly only counts if the guest is not already happy'),
(22, 4, 35186, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 3: Bucket only counts if the guest is not already happy'),
(22, 7, 35186, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 6: Dance only counts if the guest is not already happy'),
(22, 10, 35186, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 9: Fireworks only counts if the guest is not already happy'),
(22, 13, 35186, 0, 0, 1, 1, 66916, 0, 0, 1, 0, 0, '', 'Kezan Partygoer event 12: Hors D''oeuvres only counts if the guest is not already happy');

-- The class quests (Steady Shot to Charge) open after Good Help is Hard to Find in 5.4.8, not after Rolling with my
-- Homies (owner, 2026-11-06: follow 5.4.8).
UPDATE `quest_template` SET `PrevQuestId` = 14069 WHERE `Id` BETWEEN 14007 AND 14013 AND `PrevQuestId` = 14071;
