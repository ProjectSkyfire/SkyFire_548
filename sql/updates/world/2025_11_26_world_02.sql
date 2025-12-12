-- Training dummies must award Spell Practice Credit when Seal of Command procs

DELETE FROM `smart_scripts`
WHERE `entryorguid` IN (44171, 44548, 44614, 44389, 44820, 38038, 44794, 44848)
  AND `source_type` = 0
  AND `event_type` = 8
  AND `event_param1` = 118215;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44171, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(44548, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Northshire Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(44614, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Aldrassil Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(44389, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Anvilmar Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(44820, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Valley of Trials Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(38038, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Darkspear Tiki Target - On Spellhit - Seal of Command - Give Quest Credit'),
(44794, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Deathknell Training Dummy - On Spellhit - Seal of Command - Give Quest Credit'),
(44848, 0, 10, 0, 8, 0, 100, 0, 118215, 0, 0, 0, 33, 44175, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Mulgore Training Dummy - On Spellhit - Seal of Command - Give Quest Credit');

