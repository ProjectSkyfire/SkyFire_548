DELETE FROM `spell_group` WHERE `id` IN (2000, 2001);
INSERT INTO `spell_group` VALUES
(2000, 8679),
(2000, 2823),
(2001, 3408),
(2001, 5761),
(2001, 108211),
(2001, 108215);

DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (2000, 2001);
INSERT INTO `spell_group_stack_rules` VALUES
(2000, 1),
(2001, 1);
