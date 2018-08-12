DROP TABLE IF EXISTS `ticket_bug`;
CREATE TABLE `ticket_bug` (
    `ticketId` INT(10) UNSIGNED NOT NULL,
    `playerGuid` BIGINT(20) UNSIGNED NOT NULL,
    `bugNote` TEXT NOT NULL,
    `ticketCreateTime` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `mapId` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `posX` FLOAT NOT NULL DEFAULT '0',
    `posY` FLOAT NOT NULL DEFAULT '0',
    `posZ` FLOAT NOT NULL DEFAULT '0',
    `orientation` FLOAT NOT NULL DEFAULT '0',
    `closedBy` BIGINT(20) NOT NULL DEFAULT '0',
    `assignedTo` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'GUID of admin to whom ticket is assigned',
    `comment` TEXT NOT NULL,
    PRIMARY KEY (`ticketId`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB