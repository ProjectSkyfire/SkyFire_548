-- Spell power coefficients come from the client data of each effect.
-- direct_bonus and dot_bonus: above 0 overrides the client coefficient, 0 uses the coefficient of the effect, below 0 means no spell power scaling.
-- The attack power columns are only touched where stated below.

-- Rows of class spells whose effects carry a client coefficient: removed so the client value applies.
DELETE FROM `spell_bonus_data` WHERE `entry` IN (
    116, 120, 133, 139, 331, 348, 403, 421,
    585, 589, 596, 603, 635, 980, 1064, 1449,
    1949, 2060, 2061, 2120, 2136, 2812, 2912, 2948,
    3606, 5176, 5185, 5857, 6353, 7268, 8004, 8042,
    8050, 8056, 8092, 8921, 8936, 11366, 14914, 17877,
    19750, 23455, 24275, 25912, 25914, 26364, 27243, 27285,
    29722, 30108, 30451, 30455, 31661, 31707, 31803, 32379,
    33763, 34861, 34914, 42208, 42231, 44203, 44425, 44457,
    44461, 44614, 45284, 45297, 47666, 47750, 48181, 50288,
    51505, 59638, 61295, 64844, 115310, 116670, 130316
);

-- Rows that keep an attack power value or a deliberate coefficient. Attack power columns change only for
-- Avenger's Shield, Hammer of Wrath, Denounce, Censure and Judgment of Truth, whose values were checked against the tooltips.
UPDATE `spell_bonus_data` SET `direct_bonus` = -1, `dot_bonus` = 0, `ap_bonus` = 0, `ap_dot_bonus` = 0 WHERE `entry` IN (2098);
UPDATE `spell_bonus_data` SET `direct_bonus` = -1, `dot_bonus` = 0, `ap_bonus` = 0.225, `ap_dot_bonus` = -1 WHERE `entry` IN (779);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0, `dot_bonus` = -1, `ap_bonus` = 0, `ap_dot_bonus` = 0 WHERE `entry` IN (63468);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0, `dot_bonus` = -1, `ap_bonus` = 0.8175, `ap_dot_bonus` = -1 WHERE `entry` IN (31935);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0.1064, `dot_bonus` = 0, `ap_bonus` = -1, `ap_dot_bonus` = -1 WHERE `entry` IN (47960);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0.13, `dot_bonus` = -1, `ap_bonus` = -1, `ap_dot_bonus` = -1 WHERE `entry` IN (974);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0.223, `dot_bonus` = 0, `ap_bonus` = 0.142, `ap_dot_bonus` = 0 WHERE `entry` IN (31804);
UPDATE `spell_bonus_data` SET `direct_bonus` = 0.786, `dot_bonus` = 0.131, `ap_bonus` = -1, `ap_dot_bonus` = -1 WHERE `entry` IN (2944);
UPDATE `spell_bonus_data` SET `direct_bonus` = 1.38, `dot_bonus` = -1, `ap_bonus` = -1, `ap_dot_bonus` = -1 WHERE `entry` IN (686);

-- Physical abilities whose client coefficient of 1 is a placeholder: no spell power scaling.
DELETE FROM `spell_bonus_data` WHERE `entry` IN (77758, 106830);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(77758, -1, -1, -1, -1, 'Druid - Thrash (bear)'),
(106830, -1, -1, -1, -1, 'Druid - Thrash (cat)');
