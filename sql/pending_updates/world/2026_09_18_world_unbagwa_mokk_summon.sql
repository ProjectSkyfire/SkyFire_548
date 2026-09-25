-- Quest 26597 "Stranglethorn Fever": Witch Doctor Unbagwa (1449) never summons Mokk.
--
-- Fin Fizracket (2486) gives 26597, which asks the player to have Unbagwa summon Mokk the
-- Savage and bring back the Heart of Mokk (2797). Unbagwa's side of that is a talk-to quest,
-- 26598 "The Heart of Mokk", and a SMART_EVENT_REWARD_QUEST row that runs action list 144900
-- to summon Enraged Silverback Gorilla (1511), Konda (1516) and Mokk the Savage (1514).
--
-- Two links are missing:
--
--   * 26598 has `creature_questender` (1449, 26598) and no quest starter of any kind, so it
--     can never be picked up and Unbagwa offers nothing.
--   * Unbagwa's reward-quest row listens for quest 349 - the pre-Cataclysm "Stranglethorn
--     Fever", zone 33, objective text "temp text 02 - log" - which has no quest starter
--     either. Its comment names a Scarlet Zealot rather than Unbagwa.
--
-- Everything downstream is already correct: action list 144900 summons all three at
-- (-13743.7, -23.97, 45.14), about 8 yards from Unbagwa's spawn, and Mokk carries item 2797
-- at chance -100.

DELETE FROM `creature_queststarter` WHERE `id` = 1449 AND `quest` = 26598;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(1449, 26598);

UPDATE `smart_scripts` SET
    `event_param1` = 26598,
    `comment` = 'Witch Doctor Unbagwa - On Quest ''The Heart of Mokk'' Rewarded - Start Action List'
WHERE `entryorguid` = 1449 AND `source_type` = 0 AND `id` = 0 AND `event_type` = 20;
