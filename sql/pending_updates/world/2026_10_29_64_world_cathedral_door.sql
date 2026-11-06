-- Scarlet Monastery entrance (Tirisfal Glades): six copies of the Cathedral Door (101850) were stacked on one spot, so
-- players had to open it six times. Keep 1238.
DELETE FROM `gameobject` WHERE `guid` IN (3282, 5472, 9342, 79511, 82466) AND `id` = 101850;
