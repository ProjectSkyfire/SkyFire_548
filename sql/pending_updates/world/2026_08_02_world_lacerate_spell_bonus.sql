-- Lacerate (33745): MoP AP coefficients from DBC tooltip
-- Direct: m2 + (AP * 0.616)
-- Periodic: m1 + (AP * 0.0512) per tick (5 ticks over duration)
DELETE FROM `spell_bonus_data` WHERE `entry` = 33745;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(33745, 0, 0, 0.616, 0.0512, 'Druid - Lacerate (MoP: AP*0.616 direct, AP*0.0512/tick)');
