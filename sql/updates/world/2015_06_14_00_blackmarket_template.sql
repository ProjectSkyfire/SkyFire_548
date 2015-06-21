DROP TABLE IF EXISTS `blackmarket_auctions`;
CREATE TABLE `blackmarket_auctions` (
  `auctionId` int(10) unsigned NOT NULL DEFAULT '0',
  `templateId` int(10) unsigned NOT NULL DEFAULT '0',
  `startTime` int(10) unsigned NOT NULL DEFAULT '0',
  `currentBidder` int(10) unsigned NOT NULL DEFAULT '0',
  `currentBid` int(10) unsigned NOT NULL DEFAULT '0',
  `minIncrement` int(10) unsigned NOT NULL DEFAULT '0',
  `numBids` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`auctionId`),
  UNIQUE KEY `templateId` (`templateId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
