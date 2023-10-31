-- Bombing Run
DELETE FROM `spell_area` WHERE `spell`=40200;

-- Valiance Keep Tavern
DELETE FROM `areatrigger_tavern` WHERE `id`=4961;

DELETE FROM `game_graveyard_zone` WHERE `ghost_zone`=4755;
INSERT INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`) VALUES
(1692, 4756, 0), -- Merchant Quarter Graveyard
(1693, 4757, 0), -- Military District Graveyard
(1694, 4758, 0), -- Greymane Square Graveyard
(1695, 4759, 0); -- Cathedral Graveyard
