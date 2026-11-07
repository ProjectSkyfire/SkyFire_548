-- Felwood: quests that still could not be completed: drops and their prerequisites, placeholder templates, Rainbow's
-- aura, spell targets, and clicks for Slap and Cap and the Lasher. Safe to run twice.
-- FW-1: remove the obsolete feather drop prerequisite.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 1
  AND `SourceGroup` IN (7153, 7154, 7155, 7156, 7157, 7158, 9462, 9464, 14342)
  AND `SourceEntry` = 21377
  AND `ConditionTypeOrReference` = 8
  AND `ConditionValue1` = 8460;

-- FW-2: quest drops missing from the older creature loot tables.
DELETE FROM `creature_loot_template`
WHERE (`entry`, `item`) IN
  ((7086, 62918), (7136, 63123), (8956, 63088),
   (8959, 63088), (9462, 63695), (9879, 63522));

INSERT INTO `creature_loot_template`
  (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`,
   `mincountOrRef`, `maxcount`)
VALUES
  (7086, 62918, -77.3868, 1, 0, 1, 1),
  (7136, 63123, -11.775, 1, 0, 1, 1),
  (8956, 63088, -47.4298, 1, 0, 1, 1),
  (8959, 63088, -76.5245, 1, 0, 1, 1),
  (9462, 63695, -100, 1, 0, 1, 1),
  (9879, 63522, -100, 1, 0, 1, 1);

-- FW-3: replace the two remaining level-one placeholders.
UPDATE `creature_template`
SET `minlevel` = 48, `maxlevel` = 48,
    `faction_A` = 954, `faction_H` = 954,
    `unit_flags` = 64, `type_flags` = 2147483648
WHERE `entry` = 48160 AND `minlevel` = 1;

UPDATE `creature_template`
SET `minlevel` = 47, `maxlevel` = 47,
    `faction_A` = 14, `faction_H` = 14
WHERE `entry` = 48387 AND `minlevel` = 1;

-- FW-7: give Rainbow its aura (the spell script spell_terrifyng_rainbow already limits the spell to the imps).
UPDATE `creature_template`
SET `AIName` = 'SmartAI'
WHERE `entry` = 48212 AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts`
WHERE `entryorguid` = 48212 AND `source_type` = 0 AND `id` = 0;

INSERT INTO `smart_scripts`
  (`entryorguid`, `source_type`, `id`, `link`, `event_type`,
   `event_phase_mask`, `event_chance`, `event_flags`,
   `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`,
   `action_type`, `action_param1`, `action_param2`, `action_param3`,
   `action_param4`, `action_param5`, `action_param6`,
   `target_type`, `target_param1`, `target_param2`, `target_param3`,
   `target_x`, `target_y`, `target_z`, `target_o`, `comment`)
VALUES
  (48212, 0, 0, 0, 1, 0, 100, 1, 0, 0, 0, 0, 0,
   11, 88509, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
   'Rainbow! - Out Of Combat - Cast Rainbow Aura Once');

-- FW-4, FW-7, FW-8, FW-13, FW-15: nearby-entry spell targets.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 13
  AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 31 AND `ConditionTarget` = 0
  AND (
    (`SourceGroup` = 1 AND `SourceEntry` = 88254 AND `ConditionValue1` = 3 AND `ConditionValue2` = 47308) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 88697 AND `ConditionValue1` = 5 AND `ConditionValue2` = 206625) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 89406 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48077) OR
    (`SourceGroup` = 3 AND `SourceEntry` = 89620 AND `ConditionValue1` = 3 AND `ConditionValue2` = 47369) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 89822 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48310) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 89828 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48259) OR
    (`SourceGroup` = 7 AND `SourceEntry` = 89892 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48317) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 90114 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48454) OR
    (`SourceGroup` = 1 AND `SourceEntry` = 90156 AND `ConditionValue1` = 3 AND `ConditionValue2` = 48454)
  );

INSERT INTO `conditions`
  (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
   `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
   `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
   `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
  (13, 1, 88254, 0, 0, 31, 0, 3, 47308, 0, 0, 0, 0, '', 'Honey Stick targets Honey Bunny'),
  (13, 1, 88697, 0, 0, 31, 0, 5, 206625, 0, 0, 0, 0, '', 'Claw targets Jadefire Barrier'),
  (13, 1, 89406, 0, 0, 31, 0, 3, 48077, 0, 0, 0, 0, '', 'Moonwell Water targets Irontree Woods Island'),
  (13, 3, 89620, 0, 0, 31, 0, 3, 47369, 0, 0, 0, 0, '', 'Terrifying Rainbow targets Jadefire Imps'),
  (13, 1, 89822, 0, 0, 31, 0, 3, 48310, 0, 0, 0, 0, '', 'Grapple targets Oil Balloon'),
  (13, 1, 89828, 0, 0, 31, 0, 3, 48259, 0, 0, 0, 0, '', 'Oil Shredder targets Irontree Shredder'),
  (13, 7, 89892, 0, 0, 31, 0, 3, 48317, 0, 0, 0, 0, '', 'Explosives target Oil Gusher'),
  (13, 1, 90114, 0, 0, 31, 0, 3, 48454, 0, 0, 0, 0, '', 'Wisp Magnet targets Talonbranch Wisp'),
  (13, 1, 90156, 0, 0, 31, 0, 3, 48454, 0, 0, 0, 0, '', 'Swiftwind Switch targets Talonbranch Wisp');

-- FW-9: make the worker clickable during Slap and Cap.
DELETE FROM `npc_spellclick_spells`
WHERE `npc_entry` = 48331 AND `spell_id` = 89830;

INSERT INTO `npc_spellclick_spells`
  (`npc_entry`, `spell_id`, `cast_flags`, `user_type`)
VALUES
  (48331, 89830, 1, 0);

-- FW-10: enable the existing Lasher click.
UPDATE `creature_template`
SET `npcflag` = `npcflag` | 16777216
WHERE `entry` = 47747;

-- FW-9 and FW-10: restrict clicks to the relevant active quests.
DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 18
  AND `SourceId` = 0 AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9 AND `ConditionTarget` = 0
  AND (
    (`SourceGroup` = 47747 AND `SourceEntry` = 89960 AND `ConditionValue1` = 28374) OR
    (`SourceGroup` = 48331 AND `SourceEntry` = 89830 AND `ConditionValue1` = 28336)
  );

INSERT INTO `conditions`
  (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`,
   `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
   `ConditionValue1`, `ConditionValue2`, `ConditionValue3`,
   `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
  (18, 47747, 89960, 0, 0, 9, 0, 28374, 0, 0, 0, 0, 0, '', 'Lasher click requires Weeding the Lawn'),
  (18, 48331, 89830, 0, 0, 9, 0, 28336, 0, 0, 0, 0, 0, '', 'Worker click requires Slap and Cap');
