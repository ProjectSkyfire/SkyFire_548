-- The Orgrimmar Safe 2 graveyard (1340.0, -4477.6) had two Spirit Healers 28 yards apart. Keep guid 16571, which stands on
-- the graveyard, and remove guid 16702.
DELETE FROM `creature` WHERE `guid` = 16702 AND `id` = 6491;
