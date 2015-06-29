DROP TABLE IF EXISTS `blackmarket_auctions`;
CREATE TABLE `blackmarket_auctions` (
  `auctionId` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `templateId` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `startTime` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `currentBidder` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `currentBid` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `minIncrement` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `numBids` INT(10) UNSIGNED NOT NULL DEFAULT '0'
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB;