-- Quest 28715 Demonic Thieves: Melithar's Stolen Bags (195074)
-- Loot was not marked quest-required (+100), so bags never sparked and were not
-- treated as quest gameobjects. Match Fel Cone / Timberling Sprout: -100 chance
-- and GO_FLAG_INTERACT_COND (flags=4).

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=195074;

UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `entry`=27260 AND `item`=46700;

-- Quest 489 Seek Redemption!: Fel Cone (1673) loot had junk reagents that could
-- leave the chest non-empty after the quest item was taken. Keep only the cone.
DELETE FROM `gameobject_loot_template` WHERE `entry`=1701 AND `item`<>3418;
