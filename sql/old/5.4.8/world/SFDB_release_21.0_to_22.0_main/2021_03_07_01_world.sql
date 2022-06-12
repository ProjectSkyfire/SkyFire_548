-- Two Hand Sword (Warrior)
UPDATE `quest_template` SET `RewardItemId1`='73210', `RewardItemCount1`='1' WHERE  `Id`=30038;
-- Crossbow (Hunter)
UPDATE `quest_template` SET `RewardItemId1`='73211', `RewardItemCount1`='1' WHERE  `Id`=30034;
-- Rogue Daggers (Rogue)
UPDATE `quest_template` SET `RewardItemId1`='73212', `RewardItemCount1`='1' WHERE  `Id`=30036;
UPDATE `quest_template` SET `RewardItemId2`='73208', `RewardItemCount2`='1' WHERE  `Id`=30036;
-- Book and Mace (Priest)
UPDATE `quest_template` SET `RewardItemId1`='73207', `RewardItemCount1`='1' WHERE  `Id`=30035;
UPDATE `quest_template` SET `RewardItemId2`='76393', `RewardItemCount2`='1' WHERE  `Id`=30035;
-- Axe and Shield (Shaman)
UPDATE `quest_template` SET `RewardItemId1`='76391', `RewardItemCount1`='1' WHERE  `Id`=30037;
UPDATE `quest_template` SET `RewardItemId2`='73213', `RewardItemCount2`='1' WHERE  `Id`=30037;
-- Blade and Fan (Mage)
UPDATE `quest_template` SET `RewardItemId1`='76390', `RewardItemCount1`='1' WHERE  `Id`=30033;
UPDATE `quest_template` SET `RewardItemId2`='73213', `RewardItemCount2`='1' WHERE  `Id`=30033;
-- Staff (Monk)
UPDATE `quest_template` SET `RewardItemId1`='73209', `RewardItemCount1`='1' WHERE  `Id`=30027;

-- Two Hand Sword (Warrior)
UPDATE `quest_template` SET `RequiredSourceItemId1`='73210', `RequiredSourceItemCount1`='1' WHERE  `Id`=30038;
UPDATE `quest_template` SET `RequiredSourceItemId2`='0', `RequiredSourceItemCount2`='0' WHERE  `Id`=30038;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE  `Id`=30038;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE  `Id`=30038;
-- Crossbow (Hunter)
UPDATE `quest_template` SET `RequiredSourceItemId1`='73211', `RequiredSourceItemCount1`='1' WHERE  `Id`=30034;
UPDATE `quest_template` SET `RequiredSourceItemId2`='0', `RequiredSourceItemCount2`='0' WHERE  `Id`=30034;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE  `Id`=30034;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE  `Id`=30034;
-- Rogue Daggers (Rogue)
UPDATE `quest_template` SET `RequiredSourceItemId1`='73212', `RequiredSourceItemCount1`='1' WHERE `Id`=30036;
UPDATE `quest_template` SET `RequiredSourceItemId2`='73208', `RequiredSourceItemCount2`='1' WHERE `Id`=30036;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE `Id`=30036;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE `Id`=30036;
-- Book and Mace (Priest)
UPDATE `quest_template` SET `RequiredSourceItemId1`='73207', `RequiredSourceItemCount1`='1' WHERE `Id`=30035;
UPDATE `quest_template` SET `RequiredSourceItemId2`='76393', `RequiredSourceItemCount2`='1' WHERE `Id`=30035;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE `Id`=30035;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE `Id`=30035;
-- Axe and Shield (Shaman)
UPDATE `quest_template` SET `RequiredSourceItemId1`='76391', `RequiredSourceItemCount1`='1' WHERE `Id`=30037;
UPDATE `quest_template` SET `RequiredSourceItemId2`='73213', `RequiredSourceItemCount2`='1' WHERE `Id`=30037;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE `Id`=30037;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE `Id`=30037;
-- Blade and Fan (Mage)
UPDATE `quest_template` SET `RequiredSourceItemId1`='76390', `RequiredSourceItemCount1`='1' WHERE `Id`=30033;
UPDATE `quest_template` SET `RequiredSourceItemId2`='73213', `RequiredSourceItemCount2`='1' WHERE `Id`=30033;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE `Id`=30033;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE `Id`=30033;
-- Staff (Monk)
UPDATE `quest_template` SET `RequiredSourceItemId1`='73209', `RequiredSourceItemCount1`='1' WHERE `Id`=30027;
UPDATE `quest_template` SET `RequiredSourceItemId2`='0', `RequiredSourceItemCount2`='0' WHERE `Id`=30027;
UPDATE `quest_template` SET `RequiredSourceItemId3`='0', `RequiredSourceItemCount3`='0' WHERE `Id`=30027;
UPDATE `quest_template` SET `RequiredSourceItemId4`='0', `RequiredSourceItemCount4`='0' WHERE `Id`=30027;

-- Tushui Trainee / Huojin Trainee
UPDATE creature_template SET faction_A=2136, faction_H=2136, ScriptName='' WHERE entry=65471;
UPDATE creature_template SET faction_A=2136, faction_H=2136, ScriptName='' WHERE entry=54587;
-- Jaomin Ro
UPDATE creature_template SET faction_A=2136, faction_H=2136, ScriptName='' WHERE entry=54587;