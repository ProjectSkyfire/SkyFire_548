-- Enable Adherent Hanjun's Tushui quartermaster service.
-- https://www.wowhead.com/mop-classic/npc=69334/adherent-hanjun
-- https://www.wowhead.com/mop-classic/item=92071/tushui-satchel
UPDATE `creature_template` SET `npcflag` = `npcflag` | 128 WHERE `entry` = 69334;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`)
VALUES
(69334, 0, 83079, 0, 0, 0, 1),
(69334, 0, 92071, 0, 0, 0, 1)
ON DUPLICATE KEY UPDATE `item` = VALUES(`item`);

-- The satchel requires Revered with Tushui Pandaren; the tabard has no reputation gate.
UPDATE `item_template`
SET `RequiredReputationFaction` = 1353, `RequiredReputationRank` = 6
WHERE `entry` = 92071;
