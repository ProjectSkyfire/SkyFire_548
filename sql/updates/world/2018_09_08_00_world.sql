-- Remove debug plage command. RBAC_PERM_COMMAND_DEBUG_PHASE 776
DELETE FROM `command` WHERE `permission` = 776;
