-- Restore quest-gated ChanceOrQuestChance=-100 for RequiredSourceItem containers
-- on quests that also have item objectives. With HasQuestForItem() checking
-- RequiredSourceItemId even when the quest has item objectives, negative quest
-- chance works again. Replaces temporary +100% (and condition) workarounds.

-- Creature loot: Gnarlpine Necklace / Doctor's Key / Zim'bo's Mojo
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE (`entry`,`item`) IN
((7234,8049),(21778,30712),(21779,30712),(26544,35836));

-- Gameobject loot: Thieves' Tools / Felvine Shard / Battle-Worn Sword
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance`=-100 WHERE (`entry`,`item`) IN
((4665,5060),(16594,18501),(24611,38607));

-- Remove Ferocitas objective-progress condition workaround (only one of its kind)
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId`=1 AND `SourceGroup`=7234 AND `SourceEntry`=8049
  AND `ConditionTypeOrReference`=48;
