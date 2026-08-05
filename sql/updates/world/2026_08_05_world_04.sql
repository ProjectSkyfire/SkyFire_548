-- Add missing MoP Darkmoon Faire souvenir items to Gelvas Grimegate.
UPDATE `creature_template`
SET `subname` = 'Souvenir & Toy Prizes',
    `npcflag` = `npcflag` | 128
WHERE `entry` = 14828;

UPDATE `gossip_menu_option`
SET `OptionType` = 3,
    `OptionNpcflag` = 128
WHERE `MenuID` = 6183 AND `OptionID` = 0;

DELETE FROM `npc_vendor` WHERE `entry` = 14828 AND `item` IN (90899, 93730);
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(14828, 8, 90899, 0, 0, 3700, 1), -- Darkmoon Whistle
(14828, 9, 93730, 0, 0, 3817, 1); -- Darkmoon Top Hat
