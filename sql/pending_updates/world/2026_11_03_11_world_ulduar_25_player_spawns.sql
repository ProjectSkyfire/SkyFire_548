-- Ulduar: these spawns stood only in the 10-player raid (mask 8). They keep their positions and also spawn in the
-- 25-player raid (mask 24): Freya, the Thorim Event Bunny, Goran Steelbreaker, the Hired Engineers and Demolitionists,
-- Weslex Quickwrench, the Winter Revenants, the Guardian Lashers, the Guardians of Life and the Mimiron DB Target.
UPDATE `creature` SET `spawnMask` = 24 WHERE `map` = 603 AND `guid` IN (136271, 136272, 136273, 136347, 136349, 136350, 136355, 136356, 136357, 136359, 136360, 136361, 136365, 136366, 136369, 136370, 136515, 136554, 136604, 136606, 136608, 137528, 137529, 137531, 137620);

-- The seven Razorscale Controllers the instance script looks up to build the harpoons.
UPDATE `creature` SET `spawnMask` = 24 WHERE `map` = 603 AND `id` = 33233 AND `guid` IN (48304, 48305, 48306, 48307, 48308, 48309, 48310);
