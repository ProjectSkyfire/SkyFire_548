-- Vashj'ir: Sea Legs (73701) gives water breathing and a faster swim speed in every area of the zone, and a dead
-- player's ghost is the Abyssal Seahorse (75207). Without these rows a player drowns after three minutes.

DELETE FROM `spell_area` WHERE `spell` IN (73701, 75207) AND `area` IN (4815, 4953, 5144, 5145, 5146);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(73701, 4815, 0, 0, 0, 0, 2, 1, 74, 11),
(73701, 4953, 0, 0, 0, 0, 2, 1, 74, 11),
(73701, 5144, 0, 0, 0, 0, 2, 1, 74, 11),
(73701, 5145, 0, 0, 0, 0, 2, 1, 74, 11),
(73701, 5146, 0, 0, 0, 0, 2, 1, 74, 11),
(75207, 4815, 0, 0, 8326, 0, 2, 1, 64, 11),
(75207, 4953, 0, 0, 8326, 0, 2, 1, 64, 11),
(75207, 5144, 0, 0, 8326, 0, 2, 1, 64, 11),
(75207, 5145, 0, 0, 8326, 0, 2, 1, 64, 11),
(75207, 5146, 0, 0, 8326, 0, 2, 1, 64, 11);
