-- Volkhan's Molten Golems cool down over time and become Brittle Golems that the players can kill before the
-- Shattering Stomp. Heat needs its target conditions, the Brittle Golem was not attackable.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (52441, 59123);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(52441, 'spell_volkhan_cool_down'),
(59123, 'spell_volkhan_cosmetic_stun_immune_permanent');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (52387, 59528);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 52387, 0, 0, 31, 0, 3, 28695, 0, 0, 0, 0, '', 'Heat targets Molten Golems'),
(13, 1, 59528, 0, 0, 31, 0, 3, 28695, 0, 0, 0, 0, '', 'Heat targets Molten Golems');

UPDATE `creature_template` SET `unit_flags` = 32768 WHERE `entry` = 28681 AND `unit_flags` = 262146;
