-- MoP hunter Call Pet / stable: remap pet slots and restore stable-master gossip.

-- character_pet.slot: active 0-4 (Call Pet 1-5), stable from 5.
-- Remap legacy WotLK/Cata layout: 0 = current, 1-4 = stable, 100 = dismissed.

-- Old stable slots 1-4 -> MoP stable 5-8.
UPDATE `character_pet` SET `slot` = `slot` + 4
WHERE `PetType` = 1 AND `slot` BETWEEN 1 AND 4;

-- Dismissed pets (100) with no active-slot sibling: Call Pet 1.
UPDATE `character_pet` AS `cp`
LEFT JOIN `character_pet` AS `sib`
  ON `sib`.`owner` = `cp`.`owner`
 AND `sib`.`PetType` = 1
 AND `sib`.`slot` BETWEEN 0 AND 4
 AND `sib`.`id` <> `cp`.`id`
SET `cp`.`slot` = 0
WHERE `cp`.`PetType` = 1 AND `cp`.`slot` = 100 AND `sib`.`id` IS NULL;

-- Stable-master gossip needs UNIT_NPC_FLAG_STABLEMASTER (0x400000).
-- Without it, only battle-pet "lost companion" remains.
UPDATE `world`.`creature_template`
SET `npcflag` = `npcflag` | 4194304
WHERE `gossip_menu_id` IN (7451, 9212, 9437, 9576, 9821, 9864, 11971, 21194)
  AND (`npcflag` & 4194304) = 0;
