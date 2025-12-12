-- Fix GameObject SpawnTimeSecs Issue
-- GameObjects (Entry: 209354 - Golden Lotus) are marked as despawnable at action (consumable goober)
-- but have spawntimesecs=0, which prevents them from respawning after being consumed.
-- Setting spawntimesecs to 300 seconds (5 minutes) for gathering node respawn time.
UPDATE `gameobject` SET `spawntimesecs` = 300 WHERE `guid` IN (
101268, 101269, 101270, 101271, 101272, 101273, 101274, 101275, 101276, 101277, 101278, 101279
);

