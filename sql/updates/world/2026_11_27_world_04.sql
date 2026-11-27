-- Keep Shadowglen spell-practice dummies alive while preserving their spell-hit credit events (#683).
-- SmartAI action 42 uses action_param1 as a flat minimum health value.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 44614 AND `source_type` = 0 AND `id` = 7;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(44614, 0, 7, 0, 25, 0, 100, 0, 42, 1, 1, 'Training Dummy - On Reset - Set minimum health to 1');

-- Restore missing base English turn-in text; leave existing text and locales untouched.
UPDATE `quest_template`
SET `RequestItemsText` = 'Ah, you have come. Good.$B$BI am learning so much - your people have a wealth of knowledge unmatched anywhere I have seen. As much as I am humbled by that fact, I hope that I may teach you of our ways, as well.'
WHERE `Id` = 31168 AND COALESCE(`RequestItemsText`, '') = '';
UPDATE `quest_template`
SET `OfferRewardText` = 'I was warned that not all of your people would be welcoming, but I have found quite the opposite - not only have I been welcomed, but I have learned so much from you.$B$BYou seem quite promising as a young $c, and I hope I can teach you much of our ways, as your people have taught me of yours.'
WHERE `Id` = 31168 AND COALESCE(`OfferRewardText`, '') = '';
