-- Halls of Origination: Brann Bronzebeard offers "We're ready! Go, Brann!" (menu 11339) to start the Vault of Lights
-- event. Menu 12512 is the text he gives once the vault is done and has no option, so the event could not be started.
UPDATE `creature_template` SET `gossip_menu_id` = 11339 WHERE `entry` = 39908 AND `gossip_menu_id` = 12512;
