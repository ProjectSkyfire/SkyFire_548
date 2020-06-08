ALTER TABLE `blackmarket_auctions`
  CHANGE COLUMN `currentBid` `currentBid` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `currentBidder`,
  CHANGE COLUMN `minIncrement` `minIncrement` BIGINT UNSIGNED NOT NULL DEFAULT 0 AFTER `currentBid`;
