-- Stranglethorn Tigress
-- Pristine Tigress Fang
DELETE FROM `creature_loot_template` WHERE `entry`=772 AND `item`=3839;
-- Supple Tigress Fur
DELETE FROM `creature_loot_template` WHERE `entry`=772 AND `item`=58812;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(772, 58812, -80); -- Supple Tigress Fur

-- Elder Shadowmaw Panther
DELETE FROM `creature_loot_template` WHERE `entry`=772 AND `item`=58813;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(1713, 58813, -80); -- Velvety Panther Fur

-- Plush Pelts
UPDATE `quest_template` SET `OfferRewardText`='<Linzi feels the hides and smiles to herself.>$B$BThese are so nice and cushy!  I think my customers are going to LOVE them!' WHERE `Id`=26818;

-- Primal Reagents of Power
UPDATE `quest_template` SET `RequestItemsText`='Make sure the reagents are extra-cushy, $n.  A scratchy reagent simply won\'t do.' WHERE `Id`=26434;
UPDATE `quest_template` SET `OfferRewardText`='<Flem feels the hides and smiles to himself.>$B$BExcellent work, $n!  These reagents will serve perfectly.' WHERE `Id`=26434;
