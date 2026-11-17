-- Keys to the Hot Rod (item 46856) summons the Hot Rod and seats its owner in it, but the Hot Rod had no vehicle, so
-- nobody could get in.
UPDATE `creature_template` SET `VehicleId` = 448 WHERE `entry` = 34840;
