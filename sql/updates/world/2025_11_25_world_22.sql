-- Fix gossip_menu error

-- Remove gossip_menu entry 14988 with non-existent text_id 21186
-- text_id 21186 does not exist in npc_text table
DELETE FROM `gossip_menu` WHERE `entry` = 14988 AND `text_id` = 21186;

