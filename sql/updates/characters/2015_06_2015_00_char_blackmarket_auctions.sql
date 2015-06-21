DROP TABLE IF EXISTS `blackmarket_auctions`;
CREATE TABLE `blackmarket_auctions` (
    `auctionId` MEDIUMINT(9) NOT NULL DEFAULT '0' DEFAULT '0',
    `templateId` MEDIUMINT(9) NOT NULL DEFAULT '0',
    `startTime` MEDIUMINT(9) NOT NULL DEFAULT '0',
    `currentBidder` MEDIUMINT(9) NOT NULL DEFAULT '0',
    `currentBid` INT(11) NOT NULL DEFAULT '0',
    `minIncrement` MEDIUMINT(9) NOT NULL DEFAULT '0',
    `numBids` MEDIUMINT(9) NOT NULL DEFAULT '0'
)
ENGINE=InnoDB;