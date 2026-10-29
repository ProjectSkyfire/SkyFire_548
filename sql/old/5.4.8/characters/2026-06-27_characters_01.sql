UPDATE `character_pet` cp
INNER JOIN (
    SELECT starter_pet.`owner`, MIN(starter_pet.`id`) AS `id`
    FROM `character_pet` starter_pet
    LEFT JOIN `character_pet` active_pet ON active_pet.`owner` = starter_pet.`owner` AND active_pet.`active` = 1
    WHERE active_pet.`owner` IS NULL
        AND starter_pet.`PetType` = 1
        AND (
            starter_pet.`CreatedBySpell` IN (79593, 79594, 79595, 79596, 79597, 79598, 79599, 79600, 79601, 79602, 79603, 107924)
            OR starter_pet.`entry` IN (42710, 42712, 42713, 42715, 42717, 42718, 42719, 42720, 42721, 42722, 51107, 57239)
        )
        AND (starter_pet.`slot` = 0 OR starter_pet.`slot` > 4)
    GROUP BY starter_pet.`owner`
) active_starter ON active_starter.`owner` = cp.`owner` AND active_starter.`id` = cp.`id`
SET cp.`active` = 1,
    cp.`slot` = 0;
