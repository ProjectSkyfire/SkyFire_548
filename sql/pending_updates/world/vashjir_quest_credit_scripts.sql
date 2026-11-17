-- Vashj'ir: quest credit that the combat-only scripts of these creatures left out. The Speckled Sea Turtle gives the credit
-- of its ride, the Faceless Defiler gives the kill credit of "Into the Totem" (42027), and the Seabrush Terrapin, Scourgut
-- Remora and Spinescale Hammerhead give the oil of "Environmental Awareness" when Extract Oil (78547) hits them. The
-- turtle and the defiler take the 5.4.8 rows; the three oil creatures keep their combat rows and get the oil rows after them.

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (40223, 41644);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND ((`entryorguid` = 42108 AND `id` >= 1) OR (`entryorguid` IN (42112) AND `id` >= 2) OR (`entryorguid` = 42113 AND `id` >= 1));
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(40223, 0, 0, 1, 8, 0, 100, 0, 75189, 0, 0, 0, 0, 11, 75210, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Speckled Sea Turtle'),
(40223, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 75198, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Speckled Sea Turtle'),
(40223, 0, 2, 4, 8, 0, 100, 0, 75275, 0, 0, 0, 0, 33, 40339, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Speckled Sea Turtle - Give Kill Credit'),
(40223, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 75198, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Speckled Sea Turtle'),
(40223, 0, 5, 0, 28, 0, 100, 0, 0, 0, 0, 0, 0, 28, 75210, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Speckled Sea Turtle'),
(41644, 0, 0, 0, 0, 0, 100, 0, 1000, 1000, 1000, 1000, 0, 45, 0, 1, 0, 0, 0, 0, 19, 42054, 40, 0, 0, 0, 0, 0, 'Faceless Defiler - On update IC - Set data 0 1'),
(41644, 0, 1, 0, 38, 0, 100, 0, 0, 2, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Faceless Defiler - On Data Set 0 2 - Set Phase 1'),
(41644, 0, 2, 0, 6, 1, 100, 0, 0, 0, 0, 0, 0, 33, 42027, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Faceless Defiler - On death - Quest Credit'),
(42108, 0, 1, 2, 8, 0, 100, 0, 78547, 0, 0, 0, 0, 56, 56818, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Seabrush Terrapin - On SpellHit Extract Oil - Add Item Terrapin Oil'),
(42108, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Seabrush Terrapin - Linked To Id 0 - Set Event Phase 1'),
(42112, 0, 2, 3, 8, 0, 100, 0, 78547, 0, 0, 0, 0, 56, 56819, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Scourgut Remora - On SpellHit Extract Oil - Add Item Remora Oil'),
(42112, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Scourgut Remora - Linked To Id 0 - Set Event Phase 1'),
(42113, 0, 1, 2, 8, 0, 100, 0, 78547, 0, 0, 0, 0, 56, 56820, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spinescale Hammerhead - On SpellHit Extract Oil - Hammerhead Oil'),
(42113, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Spinescale Hammerhead - Linked To Id 0 - Set Event Phase 1');
