-- Quest 29605 "Brother Against Brother": Lakka's gossip option is gated on a quest that
-- cannot be obtained.
--
-- The objective is "Free Lakka", type 0 on creature 18956, and the credit comes entirely from
-- her gossip: menu 7868 option 0 runs SMART_EVENT_GOSSIP_SELECT -> action list 1895600, which
-- closes the gossip, says her line, calls KILLEDMONSTER 18956 on the invoker and despawns her.
-- All of that is correct.
--
-- The option itself is behind
--
--   (15, 7868, 0)  CONDITION_QUESTTAKEN 10097
--
-- 10097 is the Burning Crusade edition of the same quest. It has no quest starter of any kind
-- in this database - zero rows in `creature_queststarter` and zero in `gameobject_queststarter`
-- - so no player can be on it, the condition is false for everyone, and the option is never
-- drawn. 29605 is the live quest: Isfar (54840) both starts and ends it, and 29605's objective
-- is the one keyed to Lakka.
--
-- Repointed rather than given a second ElseGroup, because 10097 is unobtainable and a branch
-- for it would be dead weight.
--
-- Note for whoever picks this up: creature 54840 Isfar has no `creature` row either, so 29605
-- is not obtainable in 26.002 today. That is a separate gap; this row is wrong regardless of
-- when Isfar is placed.

UPDATE `conditions` SET `ConditionValue1` = 29605
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 7868
  AND `SourceEntry` = 0
  AND `ConditionTypeOrReference` = 9
  AND `ConditionValue1` = 10097;
