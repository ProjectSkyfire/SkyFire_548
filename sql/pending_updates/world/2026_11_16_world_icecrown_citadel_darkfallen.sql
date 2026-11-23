-- Icecrown Citadel: SmartAI for the Crimson Hall trash (Darkfallen Blood Knight, Noble, Archmage, Advisor, Tactician) and the Vampiric Fiend
-- Ported from TrinityCore 3.3.5.

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (37571,37595,37663,37664,37666,37901) AND `AIName` = '' AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (37571,37595,37663,37664,37666,37901);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37571, 0, 0, 0, 0, 0, 100, 0, 8000, 8000, 12000, 12000, 0, 11, 72057, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Advisor - In Combat - Cast ''Lich Slap'''),
(37571, 0, 1, 0, 14, 0, 100, 0, 200000, 40, 20000, 25000, 0, 11, 72066, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Advisor - On Friendly Unit Missing Health - Cast ''Shroud of Spell Warding'''),
(37595, 0, 0, 0, 0, 0, 100, 1, 500, 500, 0, 0, 0, 11, 71736, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Blood Knight - In Combat - Cast ''Vampiric Aura'' (No Repeat)'),
(37595, 0, 1, 0, 0, 0, 100, 0, 8000, 8000, 8000, 9000, 0, 11, 70437, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Blood Knight - In Combat - Cast ''Unholy Strike'''),
(37595, 0, 2, 0, 0, 0, 100, 0, 6000, 6000, 34000, 34000, 0, 11, 70450, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Blood Knight - In Combat - Cast ''Blood Mirror'''),
(37663, 0, 0, 0, 0, 0, 100, 0, 0, 500, 3000, 3500, 0, 11, 72960, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Noble - In Combat - Cast ''Shadow Bolt'''),
(37663, 0, 1, 0, 0, 0, 100, 1, 500, 500, 0, 0, 0, 11, 70645, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Noble - In Combat - Cast ''Chains of Shadow'' (No Repeat)'),
(37663, 0, 2, 0, 0, 0, 100, 0, 11000, 11000, 30000, 30000, 0, 12, 37901, 5, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Noble - In Combat - Summon Vampiric Fiend'),
(37664, 0, 0, 0, 0, 0, 100, 0, 1000, 1000, 15000, 24000, 0, 11, 70408, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Archmage - In Combat - Cast ''Amplify Magic'''),
(37664, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 25000, 30000, 0, 11, 70407, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Archmage - In Combat - Cast ''Blast Wave'''),
(37664, 0, 2, 0, 0, 0, 100, 0, 17000, 17000, 25000, 35000, 0, 11, 70410, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Archmage - In Combat - Cast ''Polymorph: Spider'''),
(37664, 0, 3, 0, 0, 0, 100, 0, 0, 500, 3500, 4500, 0, 11, 70409, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Archmage - In Combat - Cast ''Fireball'''),
(37666, 0, 0, 0, 0, 0, 100, 0, 8000, 8000, 8000, 11000, 0, 11, 70437, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Tactician - In Combat - Cast ''Unholy Strike'''),
(37666, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 20000, 20000, 0, 11, 70431, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Tactician - In Combat - Cast ''Shadowstep'''),
(37666, 0, 2, 0, 0, 0, 100, 0, 10500, 10500, 20000, 20000, 0, 11, 70432, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Darkfallen Tactician - In Combat - Cast ''Blood Sap'''),
(37901, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 11, 41290, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vampiric Fiend - On Aggro - Cast ''Disease Cloud'''),
(37901, 0, 1, 0, 0, 0, 100, 1, 9000, 9000, 0, 0, 0, 11, 70671, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Vampiric Fiend - In Combat - Cast ''Leeching Rot'' (No Repeat)'),
(37901, 0, 2, 0, 0, 0, 100, 1, 38000, 38000, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vampiric Fiend - In Combat - Despawn'),
(37901, 0, 3, 0, 7, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Vampiric Fiend - On Evade - Despawn');
