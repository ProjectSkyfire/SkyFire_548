-- Spell Group cleanup
-- Delete references to non-existing spell groups
DROP TABLE IF EXISTS `tempSpellGroup`;
CREATE TEMPORARY TABLE `tempSpellGroup` AS SELECT -id FROM `spell_Group`;
DELETE FROM `spell_group` WHERE `spell_id` < 0 AND `spell_id` NOT IN (SELECT * FROM `tempSpellGroup`);
DROP TABLE IF EXISTS `tempSpellGroup`;

-- Creature template addon
-- Only resolves creatures with exactly 1 aura
UPDATE `creature_template_addon` SET `auras`='' WHERE TRIM(`auras`) IN
(
-- Spell vehicle auras should not be defined here
'46598',
'87978',
'61424',
'50628',
'101528',
'130074',

-- not existing spells
'39707',
'465',
'63510',
'63514',
'63531',
'7056',
'96837',
'4294967295',
'2926628559',
'120555',
'118599',
'74019',
'73674',
'73422',
'71483'
);

-- Creature addon
-- Only resolves creatures with exactly 1 aura
UPDATE `creature_addon` SET `auras`='' WHERE TRIM(`auras`) IN
(
-- Spell vehicle auras should not be defined here
'46598',
'87978',
'61424',
'50628',
'101528',
'130074',

-- not existing spells
'39707',
'465',
'63510',
'63514',
'63531',
'7056',
'96837',
'4294967295',
'2926628559',
'120555',
'118599',
'74019',
'73674',
'73422',
'71483'
);

-- creature formations
DELETE FROM `creature_formations` WHERE `leaderGUID` NOT IN (SELECT `guid` FROM `creature`);
DELETE FROM `creature_formations` WHERE `memberGuid` NOT IN (SELECT `guid` FROM `creature`);

-- spellscripts (non-existing spells)
DELETE FROM `spell_script_names` WHERE ABS(`spell_id`) IN
(
62900,
58872,
52284,
51685,
50294,
49158,
49145,
48496,
47509,
44449,
33851,
31228,
31130,
29074,
16972,
12162,
11119,
6143,
543,
9799,
12809,
13161,
20425,
20911,
21084,
23880,
23989,
25899,
27683,
29834,
29841,
33695,
34074,
34497,
34498,
34499,
47193,
47422,
47948,
50365,
50391,
52041,
53412,
53434,
54171,
54646,
54846,
55680,
59566,
63108,
63521,
64205,
69366,
77755,
77794,
77795,
77796,
82984,
82988,
85285,
86181,
86183,
86184,
86185,
88756
);
