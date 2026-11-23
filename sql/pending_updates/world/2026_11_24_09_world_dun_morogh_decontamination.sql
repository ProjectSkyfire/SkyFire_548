-- Decontamination (27635): the gnome rides the Sanitron 500 (46185) through the decontamination chamber, and its script credits
-- the quest when the ride ends. The Sanitron had no click flag, so it could not be boarded, and its only click row was a hardcoded
-- ride spell for a friend. Click flag, immunity mask and the click row (Decontamination, spell 86106) are as in Legends of
-- Azeroth 5.4.8.

UPDATE `creature_template` SET `npcflag` = 16777216, `mechanic_immune_mask` = 16384 WHERE `entry` = 46185 AND `ScriptName` = '';

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 46185;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(46185, 86106, 1, 0);
