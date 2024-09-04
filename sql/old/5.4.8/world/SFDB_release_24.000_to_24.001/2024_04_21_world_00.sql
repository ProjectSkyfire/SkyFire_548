UPDATE `quest_objective` SET `description`='Learn: Ginseng Tea' WHERE `id`=268626;
UPDATE `quest_objective` SET `description`='Learn: Wildfowl Roast' WHERE `id`=268621;
UPDATE `quest_objective` SET `description`='Learn: Shrimp Dumplings' WHERE `id`=268620;
UPDATE `quest_objective` SET `description`='Learn: Swirling Mist Soup' WHERE `id`=268617;
UPDATE `quest_objective` SET `description`='Learn: Sauteed Carrots' WHERE `id`=268614;
UPDATE `quest_objective` SET `description`='Learn: Charbroiled Tiger Steak' WHERE `id`=268607;

UPDATE `quest_objective` SET `index`=2 WHERE `id`=268626;
UPDATE `quest_objective` SET `index`=2 WHERE `id`=268621;
UPDATE `quest_objective` SET `index`=2 WHERE `id`=268620;
UPDATE `quest_objective` SET `index`=2 WHERE `id`=268617;
UPDATE `quest_objective` SET `index`=2 WHERE `id`=268614;
UPDATE `quest_objective` SET `index`=2 WHERE `id`=268607;

-- Sungshin Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=14379 WHERE `entry`=64231;
UPDATE `creature_template` SET `npcflag`=211 WHERE `entry`=64231;

DELETE FROM `gossip_menu` WHERE `entry`=14379;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14379, 20624);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14379;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14379, 0, 3, 'I seek training in Pandaren cooking.', 5, 16, 0, 0, 0, 0, NULL),
(14379, 1, 1, 'I need supplies for cooking lessons.', 3, 128, 0, 0, 0, 0, NULL),
(14379, 2, 0, 'I\'m ready to discover the secret ingredient in the Lin family\'s secret ingredient noodle soup!', 1, 1, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=20624;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `prob0`) VALUES
(20624, 'Your skill in cooking is only as great as your most advanced Way.', 'Your skill in cooking is only as great as your most advanced Way.', 100);

DELETE FROM `npc_trainer` WHERE `entry`=64231;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES 
(64231, 2551, 100, 185, 0, 0),
(64231, 3412, 500, 185, 50, 0),
(64231, 19886, 10000, 185, 125, 0),
(64231, 19887, 25000, 185, 200, 0),
(64231, 33361, 100000, 185, 275, 0),
(64231, 51295, 350000, 185, 350, 0),
(64231, 88054, 500000, 185, 425, 0),
(64231, 104237, 4444, 185, 270, 0),
(64231, 104297, 5555, 185, 300, 0),
(64231, 124223, 4000, 185, 120, 0),
(64231, 124224, 4444, 185, 150, 0),
(64231, 124225, 4000, 185, 60, 0),
(64231, 124226, 4444, 185, 180, 0),
(64231, 124227, 4000, 185, 90, 0),
(64231, 124228, 4444, 185, 210, 0),
(64231, 124229, 5555, 185, 360, 0),
(64231, 124230, 23666, 185, 480, 0),
(64231, 124231, 23666, 185, 420, 0),
(64231, 124232, 23666, 185, 450, 0),
(64231, 124233, 5555, 185, 330, 0),
(64231, 124234, 23666, 185, 390, 0),
(64231, 125067, 4000, 185, 30, 0),
(64231, 125078, 4444, 185, 240, 0),
(64231, 125080, 36111, 185, 495, 0),
(64231, 125117, 111, 185, 1, 0),
(64231, 125121, 36111, 185, 510, 0),
(64231, 125122, 36111, 185, 520, 0);

DELETE FROM `npc_vendor` WHERE `entry`=64231;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES 
(64231, 0, 74854, 0, 0, 0, 1),
(64231, 1, 85583, 0, 0, 0, 1),
(64231, 2, 74660, 0, 0, 0, 1),
(64231, 3, 85584, 0, 0, 0, 1),
(64231, 4, 74852, 0, 0, 0, 1),
(64231, 5, 74851, 0, 0, 0, 1),
(64231, 6, 85585, 0, 0, 0, 1),
(64231, 7, 74659, 0, 0, 0, 1),
(64231, 8, 74832, 0, 0, 0, 1),
(64231, 9, 74845, 0, 0, 0, 1);

-- Script for quest 'Ready for Greatness'
DELETE FROM `spell_script_names` WHERE `spell_id`=125122;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(125122, 'spell_gen_rice_pudding');

-- Bobo Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=13608 WHERE  `entry`=58717;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58717;

DELETE FROM `gossip_menu` WHERE `entry`=13608;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13608, 19513);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=13608;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(13608, 0, 0, 'I\'ve got all this fruit here. Could you brew up some hooch for me?', 1, 1, 0, 0, 0, 0, NULL),
(13608, 1, 3, 'I seek training in the Way of the Brew.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=19513;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(19513, 'Brew here!', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58717;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58717, 124052, 42222, 980, 1, 0),
(58717, 124053, 42222, 980, 25, 0),
(58717, 124054, 42222, 980, 75, 0),
(58717, 125602, 42222, 980, 50, 0),
(58717, 125603, 42222, 980, 50, 0);

-- Kol Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=13609 WHERE `entry`=58712;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58712;

DELETE FROM `gossip_menu` WHERE `entry`=13609;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13609, 19526);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=13609;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(13609, 0, 0, 'Chef, could you grill this plainshawk leg for me?', 1, 1, 0, 0, 0, 0, NULL),
(13609, 1, 3, 'I seek training in the Way of the Grill.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=19526;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(19526, 'Looking for meat? I\'ve got plenty right here.$b$bOn my grill, I mean.', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58712;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58712, 104298, 42222, 975, 1, 0),
(58712, 104299, 42222, 975, 25, 0),
(58712, 104300, 42222, 975, 75, 0),
(58712, 125141, 42222, 975, 50, 0),
(58712, 125142, 42222, 975, 50, 0);

-- Anthea Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=14581 WHERE `entry`=58713;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58713;

DELETE FROM `gossip_menu` WHERE `entry`=14581;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14581, 20618);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14581;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14581, 0, 3, 'I seek training in the Way of the Wok.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=20618;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(20618, 'Hello, friend, Hungry?', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58713;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58713, 104301, 42222, 976, 1, 0),
(58713, 104302, 42222, 976, 25, 0),
(58713, 104303, 42222, 976, 75, 0),
(58713, 125594, 42222, 976, 50, 0),
(58713, 125595, 42222, 976, 50, 0);

-- Mei Mei Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=14584 WHERE `entry`=58714;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58714;

DELETE FROM `gossip_menu` WHERE `entry`=14584;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14584, 20622);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14584;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14584, 0, 3, 'I seek training in the Way of the Pot.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=20622;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(20622, 'Did you need something?', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58714;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58714, 104304, 42222, 977, 1, 0),
(58714, 104305, 42222, 977, 25, 0),
(58714, 104306, 42222, 977, 75, 0),
(58714, 125596, 42222, 977, 50, 0),
(58714, 125597, 42222, 977, 50, 0);

-- Yan Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=14583 WHERE `entry`=58715;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58715;

DELETE FROM `gossip_menu` WHERE `entry`=14583;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14583, 20621);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14583;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14583, 0, 3, 'I seek training in the Way of the Steamer.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=20621;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(20621, 'It smells delicious, doesn\'t it?', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58715;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58715, 104307, 42222, 978, 1, 0),
(58715, 104308, 42222, 978, 25, 0),
(58715, 104309, 42222, 978, 75, 0),
(58715, 125598, 42222, 978, 50, 0),
(58715, 125599, 42222, 978, 50, 0);

-- Jian Ironpaw
UPDATE `creature_template` SET `gossip_menu_id`=14422 WHERE `entry`=58716;
UPDATE `creature_template` SET `npcflag`=83 WHERE `entry`=58716;

DELETE FROM `gossip_menu` WHERE `entry`=14422;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14422, 20620);

DELETE FROM `gossip_menu_option` WHERE `menu_id`=14422;
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES
(14422, 0, 3, 'I seek training in the Way of the Oven.', 5, 64, 0, 0, 0, 0, NULL);

DELETE FROM `npc_text` WHERE `ID`=20620;
INSERT INTO `npc_text` (`ID`, `text0_0`, `prob0`, `em0_1`) VALUES
(20620, 'Yes?', 100, 2);

DELETE FROM `npc_trainer` WHERE `entry`=58716;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(58716, 104310, 42222, 979, 1, 0),
(58716, 104311, 42222, 979, 25, 0),
(58716, 104312, 42222, 979, 75, 0),
(58716, 125600, 42222, 979, 50, 0),
(58716, 125601, 42222, 979, 50, 0);

-- Quest 'Ready for Greatness' only available if Cooking 525
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=19 AND `SourceEntry`=31302;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(19, 0, 31302, 0, 0, 7, 0, 185, 525, 0, 0, 0, 0, '', 'Accept Quest 31302 only if Cooking skill 525');

-- Quests require prev quest 'Ready for Greatness'
UPDATE `quest_template` SET `OfferRewardText`='You are ready. You are now a zen master cook. ' WHERE `Id`=31302;
UPDATE `quest_template` SET `RequestItemsText`='I believe you have the potential to be a great chef, but the proof is in the pudding, so to speak.' WHERE `Id`=31302;
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31311; -- Grill
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31470; -- Wok
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31472; -- Pot
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31475; -- Steamer
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31478; -- Oven
UPDATE `quest_template` SET `PrevQuestId`=31302 WHERE `Id`=31479; -- Brew

-- Way of the Grill
UPDATE `quest_template` SET `OfferRewardText`='Good cuts. ' WHERE `Id`=31311;
UPDATE `quest_template` SET `RequestItemsText`='If you wish to learn the Way of the Grill, bring me some raw tiger steaks.' WHERE `Id`=31311;

-- Way of the Wok
UPDATE `quest_template` SET `OfferRewardText`='Well done. ' WHERE `Id`=31470;
UPDATE `quest_template` SET `RequestItemsText`='Hello, young chef. Do you wish to learn the Way of the Wok?' WHERE `Id`=31470;

-- Way of the Pot
UPDATE `quest_template` SET `OfferRewardText`='Good stuff. Now, let\'s get to your lesson. ' WHERE `Id`=31472;
UPDATE `quest_template` SET `RequestItemsText`='Want to learn the recipe for swirling mist soup?  Bring me some jade lungfish.' WHERE `Id`=31472;

-- Way of the Steamer
UPDATE `quest_template` SET `OfferRewardText`='Fine ingredients. ' WHERE `Id`=31475;
UPDATE `quest_template` SET `RequestItemsText`='If you want to learn the Way of the Steamer, you\'ll need some shrimp.' WHERE `Id`=31475;

-- Way of the Oven
UPDATE `quest_template` SET `OfferRewardText`='We can work with these. ' WHERE `Id`=31478;
UPDATE `quest_template` SET `RequestItemsText`='If you want to make wildfowl roast, you need to obtain wildfowl breasts.' WHERE `Id`=31478;

-- Way of the Brew
UPDATE `quest_template` SET `OfferRewardText`='This is a fine example of good ginseng. Would you like to learn how to make tea with it? ' WHERE `Id`=31479;
UPDATE `quest_template` SET `RequestItemsText`='You\'d like to learn the Way of the Brew?' WHERE `Id`=31479;

-- Strong as a Tiger
UPDATE `quest_template` SET `PrevQuestId`=31311 WHERE `Id`=31467;
UPDATE `quest_template` SET `OfferRewardText`='Strength and good health! This is the Way of the Grill. ' WHERE `Id`=31467;
UPDATE `quest_template` SET `RequestItemsText`='Learn the recipe from me and demonstrate what you have learned.' WHERE `Id`=31467;

-- Agile as a Tiger
UPDATE `quest_template` SET `PrevQuestId`=31470 WHERE `Id`=31471;
UPDATE `quest_template` SET `OfferRewardText`='Nice work. ' WHERE `Id`=31471;
UPDATE `quest_template` SET `RequestItemsText`='Let me teach you how to make sauteed carrots.' WHERE `Id`=31471;

-- The Soup of Contemplation
UPDATE `quest_template` SET `PrevQuestId`=31472 WHERE `Id`=31474;
UPDATE `quest_template` SET `OfferRewardText`='Intellect is a gift you must hone with time. ' WHERE `Id`=31474;
UPDATE `quest_template` SET `RequestItemsText`='If you want to learn the Way of the Pot, you have to start with swirling mist soup.' WHERE `Id`=31474;

-- The Spirit of Cooking
UPDATE `quest_template` SET `PrevQuestId`=31475 WHERE `Id`=31476;
UPDATE `quest_template` SET `OfferRewardText`='Spirits are lifted by good food. ' WHERE `Id`=31476;
UPDATE `quest_template` SET `RequestItemsText`='Let me teach you how to make shrimp dumplings.' WHERE `Id`=31476;

-- Endurance
UPDATE `quest_template` SET `PrevQuestId`=31478 WHERE `Id`=31477;
UPDATE `quest_template` SET `OfferRewardText`='Stamina and endurance are needed for mastering anything. To slave over a hot oven takes patience, but the results are very much worth the effort. ' WHERE `Id`=31477;
UPDATE `quest_template` SET `RequestItemsText`='Learn the recipe for wildfowl roast from me and let\'s see what you can do.' WHERE `Id`=31477;

-- Have a Drink
UPDATE `quest_template` SET `PrevQuestId`=31479 WHERE `Id`=31480;
UPDATE `quest_template` SET `OfferRewardText`='Well done. This is delicious. ' WHERE `Id`=31480;
UPDATE `quest_template` SET `RequestItemsText`='Let me teach you how to make ginseng tea.' WHERE `Id`=31480;
