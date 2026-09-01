-- Stable-master gossip needs UNIT_NPC_FLAG_STABLEMASTER (0x400000).
-- Without it, only battle-pet "lost companion" remains.
-- Moved out of the characters hunter-stable update (cross-DB UPDATE is invalid there).

UPDATE `creature_template`
SET `npcflag` = `npcflag` | 4194304
WHERE `gossip_menu_id` IN (7451, 9212, 9437, 9576, 9821, 9864, 11971, 21194)
  AND (`npcflag` & 4194304) = 0;
