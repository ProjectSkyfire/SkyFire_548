-- Krom'gar Hot Air Balloon (42038) and The Bomb (41988) on the Stonetalon elevator platform belong to a later phase of
-- the zone that is not implemented, and the balloon ride is missing, so everyone saw a parked balloon. Remove both
-- spawns until the phase and the ride are added.
DELETE FROM `creature` WHERE `guid` IN (213944, 213945) AND `id` IN (42038, 41988);
