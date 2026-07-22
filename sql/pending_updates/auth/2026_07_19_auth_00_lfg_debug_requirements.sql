-- Add RBAC entry for the LFG scenario requirement debug command.

INSERT INTO `rbac_permissions` (`id`, `name`)
SELECT 1010, 'Command: debug lfg requirements'
WHERE NOT EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1010
);

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`)
SELECT 197, 1010
WHERE EXISTS (
    SELECT 1 FROM `rbac_permissions`
    WHERE `id` = 1010
      AND `name` = 'Command: debug lfg requirements'
)
AND NOT EXISTS (
    SELECT 1 FROM `rbac_linked_permissions`
    WHERE `id` = 197
      AND `linkedId` = 1010
);
