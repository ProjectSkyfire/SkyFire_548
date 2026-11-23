-- Cataclysm dungeon creatures whose level or expansion differs from Legends of Azeroth 5.4.8: placeholder
-- templates at level 1 get their real level, and the expansion field follows. Creatures that Legends of
-- Azeroth lists at level 1 keep the level they have. Each statement checks the old values.
-- Based on Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `exp` = 1 WHERE `entry` IN (36171, 46766, 47924, 47925, 47926) AND `exp` = 0;
UPDATE `creature_template` SET `exp` = 2 WHERE `entry` IN (37231, 55415) AND `exp` = 0;
UPDATE `creature_template` SET `exp` = 3 WHERE `entry` IN (
    43873, 43875, 43878, 44895, 45455, 45477, 45504, 45572, 45912, 45915, 45917, 45919, 45922, 45924, 45926, 45928,
    45930, 45932, 45935, 45991, 46419, 47066, 47806, 48977, 49045, 49943, 51157, 54500, 54506, 54513, 54522, 54555,
    54560, 54589, 54590, 54598, 54599, 54612, 54628, 54632, 54633, 54634, 54644, 54645, 54663, 54664, 54665, 54666,
    54686, 54728, 54735, 54747, 54853, 54882, 54883, 54884, 54927, 54938, 54955, 54968, 54969, 54971, 54972, 55078,
    55085, 55106, 55107, 55109, 55111, 55112, 55154, 55366, 55384, 55385, 55389, 55390, 55391, 55393, 55394, 55395,
    55419, 55426, 55445, 55453, 55465, 55466, 55500, 55503, 55510, 55519, 55524, 55532, 55549, 55559, 55563, 55570,
    55624, 55636, 55654, 55656, 55700, 55739, 55762, 55838, 55839, 55911, 55912, 55913, 55914, 55967, 55969, 55971,
    56073, 56078, 56087, 56102, 56139, 56140, 56141, 56142, 56143, 56144, 56389, 56579, 57107, 57197, 57261, 57458,
    57913, 58226, 58233, 58235
) AND `exp` = 0;
UPDATE `creature_template` SET `exp` = 4 WHERE `entry` IN (55728, 55779) AND `exp` = 0;
UPDATE `creature_template` SET `exp` = 0 WHERE `entry` IN (45439, 49725, 54486, 54488) AND `exp` = 3;
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82 WHERE `entry` IN (
    44711, 44840, 55868, 55879
) AND `minlevel` = 1 AND `maxlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 84, `maxlevel` = 84, `exp` = 3 WHERE `entry` = 46492 AND `minlevel` = 1 AND `maxlevel` = 1 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85 WHERE `entry` = 46973 AND `minlevel` = 1 AND `maxlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3 WHERE `entry` IN (
    39369, 39370, 39634, 39635, 40550, 40585, 40592, 40620, 40622, 40630, 40669, 40835, 40927, 41168, 43658, 43817,
    43926, 43934, 47922, 54191, 54197, 54403, 54435, 54566, 54585, 54600, 54795, 54864, 54865, 54952, 54995, 55474,
    55502, 55837, 56001, 56002, 56308, 57410, 57631, 58234
) AND `minlevel` = 1 AND `maxlevel` = 1 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 86, `exp` = 3 WHERE `entry` = 56036 AND `minlevel` = 1 AND `maxlevel` = 1 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87 WHERE `entry` IN (
    43655, 46186, 46246
) AND `minlevel` = 1 AND `maxlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3 WHERE `entry` IN (
    44713, 47040, 48904, 54385, 54434, 54446, 54494, 54574, 54580, 54581, 54582, 54594, 54597, 54641, 54646, 54696,
    54939, 54940, 54941, 54942, 55377, 55427, 55441, 55467, 55468, 55469
) AND `minlevel` = 1 AND `maxlevel` = 1 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 88, `maxlevel` = 88, `exp` = 3 WHERE `entry` IN (
    47177, 55400
) AND `minlevel` = 1 AND `maxlevel` = 1 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 73 WHERE `entry` = 26705 AND `minlevel` = 72;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85 WHERE `entry` = 40283 AND `minlevel` = 80 AND `maxlevel` = 80;
UPDATE `creature_template` SET `minlevel` = 85, `exp` = 3 WHERE `entry` = 45704 AND `minlevel` = 82 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85 WHERE `entry` = 44796 AND `minlevel` = 82 AND `maxlevel` = 82;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 0 WHERE `entry` = 44586 AND `minlevel` = 82 AND `maxlevel` = 82 AND `exp` = 3;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85 WHERE `entry` = 48092 AND `minlevel` = 84 AND `maxlevel` = 84;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3 WHERE `entry` = 56248 AND `minlevel` = 85 AND `maxlevel` = 85 AND `exp` = 0;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3 WHERE `entry` = 54548 AND `minlevel` = 90 AND `maxlevel` = 90 AND `exp` = 0;
