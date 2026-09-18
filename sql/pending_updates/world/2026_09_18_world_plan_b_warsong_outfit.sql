-- Quest 11658 "Plan B": the gossip that hands out Warsong Outfits is gated backwards.
--
-- Item 34842 "Warsong Outfit" has no loot row anywhere - by design. The outfits come from
-- talking to the dead of the Warsong caravan, Dead Caravan Guard (25342, menu 9155) and Dead
-- Caravan Worker (25343, menu 9156), and picking "Retrieve Warsong Outfit.", which SmartAI
-- answers with SMART_ACTION_ADD_ITEM before despawning the corpse. That wiring is intact.
--
-- The gate is the `conditions` row on each menu:
--
--   (15, 9155, 0)  CONDITION_ITEM 34842, count 10, ConditionValue3 0, NegativeCondition 0
--   (15, 9156, 0)  same
--   Comment on both: "Only show gossip if player has less then 10 outfits"
--
-- ConditionMgr evaluates CONDITION_ITEM as HasItemCount(34842, 10, checkBank), so as written the
-- option is shown only to a player who already holds ten - i.e. the only source of the item is
-- hidden until it is no longer needed, and the quest cannot be started on the outfits at all.
--
-- NegativeCondition 1 makes the row match its comment, and ConditionValue3 1 counts the bank so
-- banked outfits still close the option. That pairing is the standard shape for a "does not have"
-- item condition here - 175 rows in this database use ConditionValue3 1 with NegativeCondition 1.
--
-- The banner half of the quest is already fine: gameobject 187577 is spawned and its loot
-- carries item 34690 at -100.

UPDATE `conditions` SET `NegativeCondition` = 1, `ConditionValue3` = 1
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` IN (9155, 9156)
  AND `SourceEntry` = 0
  AND `ConditionTypeOrReference` = 2
  AND `ConditionValue1` = 34842;
