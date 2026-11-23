-- Desolace: Cleansing Our Crevasse (14346, 14381). The Slitherblade kills gave no credit and Grimfeather could not
-- be clicked, so the objectives could not be filled. Safe to run twice.
UPDATE `creature_template` SET `KillCredit1` = 36230 WHERE `entry` = 90;
UPDATE `creature_template` SET `KillCredit1` = 36229 WHERE `entry` = 35605;
UPDATE `creature_template` SET `KillCredit1` = 36228 WHERE `entry` = 35606;

UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 36227;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 36227;
INSERT INTO `npc_spellclick_spells` (`npc_entry`,`spell_id`,`cast_flags`,`user_type`)
VALUES (36227,46990,1,0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 36227;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
 `ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,
 `ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(18,36227,46990,0,0,9,0,14346,0,0,0,0,0,'','Required quest active for spellclick'),
(18,36227,46990,0,1,9,0,14381,0,0,0,0,0,'','Required quest active for spellclick');
