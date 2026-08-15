-- Wayward Fire Elemental
DELETE FROM `creature_text` WHERE `entry`=37112;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(37112, 0, 0, 'A gi kras tor shodru\'ga bromo!', 14, 12, 100, 0, 0, 0, 'Wayward Fire Elemental - SAY_0'),
(37112, 1, 0, 'T MA TORO NUK KRAST\'VEN FMERK', 12, 12, 100, 0, 0, 0, 'Wayward Fire Elemental - SAY_AGGRO');

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=37112;

DELETE FROM `smart_scripts` WHERE `entryorguid`=37112;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(37112, 0, 0, 0, 4, 0, 100, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Fire Elemental - On Aggro - Say 1'),
(37112, 0, 1, 0, 1, 0, 100, 0, 60000, 60000, 60000, 60000, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wayward Fire Elemental - Out of Combat - Say 0'),
(37112, 0, 2, 0, 0, 0, 100, 0, 4000, 6000, 9000, 11000, 0, 11, 9053, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Wayward Fire Elemental - In Combat - Cast Fireball');
