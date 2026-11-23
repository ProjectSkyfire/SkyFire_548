-- Onyxia's Gate was spawned twice at the same spot (same orientation, phase and spawn mask). Kept guid 933, removed 48054.
-- Evidence for 48054: 0 yd from guid 933, same entry 177928 and orientation 0.62, not pooled, not in an event, no
-- SmartAI, condition, linked respawn or script reference, and the entry is never summoned. The 5.4.8 data of Legends of
-- Azeroth has one spawn of this gate (guid 173234); the 4.3.4 data has two.
DELETE FROM `gameobject` WHERE `guid` = 48054 AND `id` = 177928;
