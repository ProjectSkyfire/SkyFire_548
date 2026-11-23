-- The Deadmines, Vanessa VanCleef (heroic): the Rope of her detonations is bound to its script and can be clicked
-- (Ride Vehicle Hardcoded), Fiery Blaze burns only players, and her slay line is the only text of its group (the two
-- death lines sat in it as ids 1 and 2 and were said on a kill).
UPDATE `creature_template` SET `ScriptName` = 'npc_deadmines_vanessa_rope', `npcflag` = 16777216 WHERE `entry` = 49550;

DELETE FROM `creature_text` WHERE `entry` = 49541 AND `groupid` = 1 AND `id` IN (1, 2);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 49550;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(49550, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 93485;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 93485, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Vanessa VanCleef - Fiery Blaze - players only');
