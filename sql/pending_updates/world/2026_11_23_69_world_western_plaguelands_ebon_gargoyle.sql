-- The Farmers' Militia (27084): the Ebon Gargoyle that the Ebon Gargoyle Collar summons (spells 84009 and 83994) is
-- a vehicle in the client data and in Legends of Azeroth 5.4.8 (vehicle 1068, one seat), and had no vehicle id
-- here, so the summoned gargoyle was a plain creature.
-- The value is as in the source.

UPDATE `creature_template` SET `VehicleId` = 1068 WHERE `entry` = 44955 AND `VehicleId` = 0;
