-- Add RBAC entries for LFG debug commands.

INSERT INTO `rbac_permissions` (`id`, `name`)
SELECT 1010, 'Command: debug lfg requirements'
WHERE NOT EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1010
);

INSERT INTO `rbac_permissions` (`id`, `name`)
SELECT 1011, 'Command: debug lfg flex'
WHERE NOT EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1011
);

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`)
SELECT 197, 1010
WHERE EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1010
)
AND NOT EXISTS (
    SELECT 1 FROM `rbac_linked_permissions`
    WHERE `id` = 197
      AND `linkedId` = 1010
);

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`)
SELECT 197, 1011
WHERE EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1011
)
AND NOT EXISTS (
    SELECT 1 FROM `rbac_linked_permissions`
    WHERE `id` = 197
      AND `linkedId` = 1011
);
