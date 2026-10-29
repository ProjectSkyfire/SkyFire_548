SET @character_pet_active_exists := (
    SELECT COUNT(*)
    FROM INFORMATION_SCHEMA.COLUMNS
    WHERE TABLE_SCHEMA = DATABASE()
        AND TABLE_NAME = 'character_pet'
        AND COLUMN_NAME = 'active'
);

SET @character_pet_active_sql := IF(
    @character_pet_active_exists = 0,
    'ALTER TABLE `character_pet` ADD COLUMN `active` tinyint unsigned NOT NULL DEFAULT ''0'' AFTER `renamed`',
    'DO 0'
);

PREPARE character_pet_active_stmt FROM @character_pet_active_sql;
EXECUTE character_pet_active_stmt;
DEALLOCATE PREPARE character_pet_active_stmt;

UPDATE `character_pet`
    SET `active` = 0;

UPDATE `character_pet` cp
INNER JOIN (
    SELECT `owner`, MIN(`id`) AS `id`
    FROM `character_pet`
    WHERE `slot` = 0
    GROUP BY `owner`
) active_pet ON active_pet.`owner` = cp.`owner` AND active_pet.`id` = cp.`id`
SET cp.`active` = 1;
