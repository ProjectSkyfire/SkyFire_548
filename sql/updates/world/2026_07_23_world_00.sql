-- Make Mark of the Wild, Blessing of Kings, and Legacy of the Emperor
-- mutually exclusive (same stats raid-buff category).

-- Nested exclusive subgroups for BoK and Legacy Emperor applied auras.
DELETE FROM `spell_group` WHERE `id` IN (1148, 1149);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1148, 20217),  -- Blessing of Kings
(1148, 72586),  -- Blessing of Kings (alternate)
(1149, 117666), -- Legacy of the Emperor (raid aura)
(1149, 117667); -- Legacy of the Emperor (ally aura)

DELETE FROM `spell_group_stack_rules` WHERE `group_id` IN (1148, 1149);
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1148, 1), -- exclusive within BoK ranks
(1149, 1); -- exclusive within Emperor auras

-- Parent group: MotW / BoK / Emperor / pet Embrace share exclusive-same-effect.
DELETE FROM `spell_group` WHERE `id` = 1118;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES
(1118, -1148), -- Blessing of Kings subgroup
(1118, -1149), -- Legacy of the Emperor subgroup
(1118, 1126),  -- Mark of the Wild
(1118, 90363); -- Embrace of the Shale Spider

DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1118;
INSERT INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES
(1118, 3); -- SPELL_GROUP_STACK_RULE_EXCLUSIVE_SAME_EFFECT
