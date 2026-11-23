-- Brann Bronzebeard at the Ulduar entrance offers the gossip his script handles (menu 10355 already exists).
UPDATE `creature_template` SET `npcflag` = 1, `gossip_menu_id` = 10355 WHERE `entry` = 33579;
