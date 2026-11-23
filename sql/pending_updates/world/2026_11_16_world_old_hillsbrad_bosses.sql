-- Old Hillsbrad Foothills: Lieutenant Drake, Captain Skarloc and the Epoch Hunter were immune to players
UPDATE `creature_template` SET `unit_flags`=`unit_flags`&~0x00000100 WHERE `entry` IN (17848,17862,18096);
