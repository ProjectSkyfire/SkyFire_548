-- Zen'Kiki and the Cultists (26955): the four Captive Plaguebears (credit 44902) are freed by opening the Cultist Cages at
-- the Malicia Outpost, but the ten cages had no script, so opening one never gave the credit. Each cage now gives
-- the credit to the player who opens it.
-- Rows are as in Legends of Azeroth 5.4.8 (the game object state event reports the loot state, 2 is activated).

UPDATE `gameobject_template` SET `AIName` = 'SmartGameObjectAI' WHERE `entry` IN (205155,205156,205157,205158,205159,205160,207427,207428,207429,207430) AND `ScriptName` = '';

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (205155,205156,205157,205158,205159,205160,207427,207428,207429,207430) AND `source_type` = 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(205155, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(205156, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(205157, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(205158, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(205159, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(205160, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(207427, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(207428, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(207429, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit'),
(207430, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 0, 33, 44902, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Cultist Cage - On Activated - Quest Credit');
