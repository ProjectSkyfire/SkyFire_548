-- Westfall graveyard 4 had two Spirit Healers; keep the one at the graveyard (guid 16759), remove the second one 20 yards
-- away, which the 4.3.4 data does not have.
DELETE FROM `creature` WHERE `guid` = 16577 AND `id` = 6491;
