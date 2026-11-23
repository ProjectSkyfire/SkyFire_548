-- Out of Place (27961) asks for 8 Ash Chickens, which are picked up by clicking the Ash Chicken (47278). The chicken
-- had its SmartAI that adds the item and despawns it, but neither the spell click nor the spell click flag, so it could
-- not be clicked and the quest could not be finished. Spell click data and flag are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 47278;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 47278;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(47278, 90102, 2, 0);
