-- Pending world cleanup from the 2026-06-22 DBErrors.log audit.
--
-- Scope:
-- - Clear invalid kill-credit references that worldserver already drops at runtime.
-- - Remove graveyard subzone rows that worldserver already skips during startup.
--
-- Apply manually to a staging world database before promotion to sql/updates/world.

UPDATE `creature_template`
SET `KillCredit2` = 0
WHERE `entry` IN (71292, 71293)
  AND `KillCredit2` = 71066;

DELETE FROM `game_graveyard_zone`
WHERE `id` IN (1692, 1693, 1694, 1695)
  AND `ghost_zone` IN (4756, 4757, 4758, 4759)
  AND `faction` = 0;
