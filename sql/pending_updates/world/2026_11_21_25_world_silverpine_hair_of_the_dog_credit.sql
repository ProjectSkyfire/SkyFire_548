-- Silverpine Forest: "Hair of the Dog" (27226). The Orc Sea Dogs credited the quest when Explosive Ale (84517) hit
-- them, but 84517 is only triggered by the last tick of the Barrel of Explosive Ale channel (84514) and is aimed at the
-- channel target, which is already gone when the channel ends, so it never hit and the quest could not be finished.
-- The Sea Dogs now react to the barrel itself, which the player casts on them, and the barrel can only be thrown at
-- Orc Sea Dogs.
UPDATE `smart_scripts` SET `event_param1` = 84514, `comment` = 'Orc Sea Dog - On SpellHit Barrel of Explosive Ale - Quest Credit'
WHERE `entryorguid` = 45196 AND `source_type` = 0 AND `id` = 1;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceGroup` = 0 AND `SourceEntry` = 84514;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 84514, 0, 0, 31, 1, 3, 45196, 0, 0, 0, 0, '', 'Barrel of Explosive Ale - Target must be an Orc Sea Dog');
