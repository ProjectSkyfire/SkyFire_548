DROP TABLE IF EXISTS `blackmarket_template`;
CREATE TABLE `blackmarket_template` (
  `Id` int(10) unsigned NOT NULL DEFAULT '0',
  `MarketId` int(10) unsigned NOT NULL DEFAULT '0',
  `SellerID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemEntry` int(10) unsigned NOT NULL DEFAULT '0',
  `Quantity` int(10) unsigned NOT NULL DEFAULT '0',
  `MinBid` INT(10) NOT NULL DEFAULT '0',
  `Duration` int(10) unsigned NOT NULL DEFAULT '0',
  `Chance` FLOAT DEFAULT '0',
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
