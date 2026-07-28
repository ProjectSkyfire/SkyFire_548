-- Clean remaining world database validation warnings outside deferred script assignment coverage.

-- These normal quests should not carry daily/weekly/repeatable or extra high flag state.
UPDATE `quest_template`
SET `Flags` = 128,
    `SpecialFlags` = 0
WHERE `Id` IN (28617, 28733, 28744, 28747, 28748)
  AND (`Flags` <> 128 OR `SpecialFlags` <> 0);

-- Reforge gossip uses the standard chat icon in this client.
UPDATE `gossip_menu_option`
SET `OptionIcon` = 0
WHERE `MenuID` = 12094
  AND `OptionID` = 0
  AND `OptionIcon` = 21;

-- Mulberry Barrel is the owner of gameobject loot template 40884.
UPDATE `gameobject_template`
SET `data1` = 40884
WHERE `entry` = 210080
  AND `type` = 3
  AND `data1` = 0;

-- Remove script bindings for spell ids that are not present in the 5.4.8 SpellStore.
DELETE FROM `spell_script_names`
WHERE (`spell_id` = 61698 AND `ScriptName` = 'spell_gen_ds_flush_knockback')
   OR (`spell_id` = 62791 AND `ScriptName` = 'spell_xt002_heart_overload_periodic');

-- Prefer kill-credit spells over direct SMART_ACTION_CALL_KILLEDMONSTER entries.
UPDATE `smart_scripts`
SET `action_type` = 11,
    `action_param1` = 82286,
    `action_param2` = 2,
    `comment` = 'RP Oliver - Cast Cry For The Moon quest credit'
WHERE `entryorguid` = 43858
  AND `source_type` = 0
  AND `id` = 30
  AND `link` = 0
  AND `action_type` = 33
  AND `action_param1` = 43969;

UPDATE `smart_scripts`
SET `action_type` = 85,
    `action_param1` = 130723,
    `action_param2` = 2,
    `comment` = 'General Nazgrim - On linked - Player cast quest credit'
WHERE `entryorguid` = 55054
  AND `source_type` = 0
  AND `id` = 1
  AND `link` = 2
  AND `action_type` = 33
  AND `action_param1` = 67040;

UPDATE `smart_scripts`
SET `action_type` = 85,
    `action_param1` = 131057,
    `action_param2` = 2,
    `comment` = 'Sky Admiral Rogers - On linked - Player cast quest credit'
WHERE `entryorguid` = 66292
  AND `source_type` = 0
  AND `id` = 1
  AND `link` = 2
  AND `action_type` = 33
  AND `action_param1` = 66292;
