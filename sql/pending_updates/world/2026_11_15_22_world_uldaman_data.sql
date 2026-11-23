-- Uldaman (map 70): boss immunities, the Lore Keeper's gossip and the wording of the boss texts.
-- Archaedas and Ironaya were not immune to crowd control (the 4.3.4 data and the 5.4.8 data give both the same
-- mask: charm, fear, root, sleep, freeze, polymorph and sap). The Lore Keeper of Norgannon started his gossip at its
-- last menu ("I will access the discs now"), so the Earthen lore chain that leads there was never shown; with menu
-- 562 the chain is offered while quest 2278 is in progress and ends at the same option. The boss lines get the
-- wording of the broadcast texts (Makers, Disks, the exclamation marks).
-- Idempotent: absolute UPDATEs.

UPDATE `creature_template` SET `mechanic_immune_mask` = 536941137 WHERE `entry` IN (2748, 7228);

UPDATE `creature_template` SET `gossip_menu_id` = 562 WHERE `entry` = 7172;

UPDATE `creature_text` SET `text` = 'Who dares awaken Archaedas? Who dares the wrath of the Makers?' WHERE `entry` = 2748 AND `groupid` = 0 AND `id` = 0;
UPDATE `creature_text` SET `text` = 'Awake, ye servants! Defend the Disks!' WHERE `entry` = 2748 AND `groupid` = 1 AND `id` = 0;
UPDATE `creature_text` SET `text` = 'To my side, brothers! For the Makers!' WHERE `entry` = 2748 AND `groupid` = 2 AND `id` = 0;
UPDATE `creature_text` SET `text` = 'Reckless mortal!' WHERE `entry` = 2748 AND `groupid` = 3 AND `id` = 0;
UPDATE `creature_text` SET `text` = 'Die! Die!' WHERE `entry` = 4854 AND `groupid` = 2 AND `id` = 0;
UPDATE `creature_text` SET `text` = 'None may steal the secrets of the Makers!' WHERE `entry` = 7228 AND `groupid` = 0 AND `id` = 0;
