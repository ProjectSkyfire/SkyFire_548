-- Striking Back (26094): the Repaired Bomber (42092) at the Ironforge Airfield takes the player on a bombing run (taxi path
-- 2203, spell 78510), during which the Iron Hammer Bomb (spell 78499) is dropped on the Dark Iron attackers. The bomber had no
-- click flag and no spellclick row, so it could not be boarded (upstream issue 1479). The
-- bomber's flag and immunity mask and its spellclick row are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216, `mechanic_immune_mask` = 16384 WHERE `entry` = 42092 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 42092;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(42092, 78510, 3, 0);
