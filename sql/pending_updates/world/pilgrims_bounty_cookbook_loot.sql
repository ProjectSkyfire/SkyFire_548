-- Pilgrim's Bounty cookbooks contain five faction-specific recipes.
-- Loot mode zero excludes every recipe from normal item loot generation.
-- Preserve the existing chances, quantities and any additional loot modes.
-- https://www.wowhead.com/mop-classic/item=46809/bountiful-cookbook
-- https://www.wowhead.com/mop-classic/item=46810/bountiful-cookbook
UPDATE `item_loot_template` SET `lootmode`=`lootmode` | 1
 WHERE (`entry`=46809 AND `item` IN (44858,44859,44860,44861,44862))
    OR (`entry`=46810 AND `item` IN (46803,46804,46805,46806,46807));
