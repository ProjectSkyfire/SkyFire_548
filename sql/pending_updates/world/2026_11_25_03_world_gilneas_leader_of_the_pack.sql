-- Gilneas, Leader of the Pack (14386): the Mastiff Whistle (spell 68682) takes its target from the nearby entry
-- search, which fails without target conditions, so the whistle could not be used on Dark Ranger Thyala. The whistle
-- needs Thyala within 40 yards. Thyala then calls eight Attack Mastiffs (36405) that attack her, and they leave when
-- they evade. Her death gives the quest credit to every player within 100 yards.
-- Based on Legends of Azeroth 5.4.8 (smart scripts and spell conditions); the implicit target condition is also in
-- the Cataclysm Preservation Project 4.3.4.

-- 1. Target conditions of the whistle: effect 0 must hit Thyala, and the caster must stand within 40 yards of her.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13, 17) AND `SourceEntry` = 68682;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 68682, 0, 0, 31, 0, 3, 36312, 0, 0, 0, 0, '', 'Call Attack Mastiffs - Target Dark Ranger Thyala'),
(17, 0, 68682, 0, 0, 29, 0, 36312, 40, 0, 0, 0, 0, '', 'Call Attack Mastiffs - Caster near Dark Ranger Thyala');

-- 2. Dark Ranger Thyala summons the mastiffs when the whistle hits her (once a minute) and gives the credit on death.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (36312, 36405);
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (36312, 36405);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(36312, 0, 0, 1, 8, 0, 100, 0, 68682, 0, 60000, 60000, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, -2.5, -2.5, 0, 0, 'Dark Ranger Thyala - On Spell Hit (Call Attack Mastiffs) - Summon Attack Mastiff'),
(36312, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, -2.5, 2.5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, 2.5, -2.5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, 2.5, 2.5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, -5, -5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, -5, 5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, 5, -5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 12, 36405, 1, 60000, 1, 0, 0, 1, 0, 0, 0, 5, 5, 0, 0, 'Dark Ranger Thyala - Linked - Summon Attack Mastiff'),
(36312, 0, 8, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 33, 36312, 0, 0, 0, 0, 0, 17, 0, 100, 0, 0, 0, 0, 0, 'Dark Ranger Thyala - On Death - Give Quest Credit'),
(36405, 0, 0, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Attack Mastiff - On Evade - Despawn');
