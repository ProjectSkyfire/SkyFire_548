CREATE TABLE `item_instance_transmog` (
	`itemGuid` BIGINT(20) UNSIGNED NOT NULL,
	`transmogrifyId` INT(10) NOT NULL DEFAULT '0',
	PRIMARY KEY (`itemGuid`) USING BTREE
)
COLLATE='utf8mb4_unicode_ci' ENGINE=InnoDB;

INSERT INTO `item_instance_transmog`
SELECT guid, transmogrifyId FROM item_instance WHERE transmogrifyId > 0;

ALTER TABLE `item_instance`
  DROP `transmogrifyId`;