-- High Inquisitor Fairbanks lies dead in the 5.4.8 Scarlet Monastery and cannot be targeted; the spawn was
-- an attackable level 40 elite. Keep the body: not attackable, not selectable, shown dead (his script keeps the dead
-- pose until combat, which can no longer start).
UPDATE `creature` SET `unit_flags` = 33587266, `dynamicflags` = 64 WHERE `guid` = 8301002 AND `id` = 4542 AND `map` = 1004;
