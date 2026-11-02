-- Six quests whose credit bunny is hit by the quest spell and never grants credit.
--
-- In every case the spell reaches the bunny -- each one already has `conditions` source type 13
-- restricting the spell's implicit target to exactly these entries -- but no `smart_scripts` row
-- anywhere calls SMART_ACTION_CALL_KILLEDMONSTER (33) for them, and the bunnies are faction 35
-- NOT_SELECTABLE dummies that cannot be killed for credit.
--
-- target_type 7 (SMART_TARGET_ACTION_INVOKER) throughout: SMART_EVENT_SPELLHIT passes the caster
-- as invoker (SmartAI.cpp:617) and SMART_EVENT_JUST_SUMMONED passes the summoner
-- (SmartAI.cpp:647), so the credit goes to the player who used the item. target 0 or 1 would send
-- it through me->GetLootRecipient() (SmartScript.cpp:868-878), which is NULL on a bunny nothing
-- ever tags. 253 of the 302 existing action-33 rows already use target 7.

-- --------------------------------------------------------------------------------------------
-- 10895 "Zeth'Gor Must Burn!" -- item 31739 casts 36374 "Summon Smoke Beacon", whose
-- SPELL_EFFECT_DUMMY is restricted to these four tower markers by conditions 13/36374.
-- --------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts`
 WHERE `source_type` = 0 AND `entryorguid` IN (21182, 22401, 22402, 22403) AND `id` = 3;

-- --------------------------------------------------------------------------------------------
-- 9805 "Blessing of Incineratus" -- item 24467 casts 31927 "Living Fire", conditions 13/31927.
-- 10233 "Torching Sunfury Hold" -- item 28550 casts 34526 "Burn Blood Elf Equipment",
-- conditions 13/34526. The torch drops from 20221 Sunfury Flamekeeper at 100%.
-- --------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts`
 WHERE `source_type` = 0 AND `entryorguid` IN (18110, 18142, 18143, 18144, 19723, 19724) AND `id` = 1;

-- --------------------------------------------------------------------------------------------
-- 11715 "Fueling the Project" -- item 34975 casts 45990 "Collect Oil", conditions 13/45990.
-- The Oil Pool has only its two cosmetic ON_RESPAWN rows, so using the collector does nothing
-- at all. The pool is consumed on use, so the credit links to a delayed despawn.
-- --------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts`
 WHERE `source_type` = 0 AND `entryorguid` = 25781 AND `id` IN (2, 3);

-- --------------------------------------------------------------------------------------------
-- 12859 "This Just In: Fire Still Hot!" -- item 41131 casts 55037 "Fire Extinguisher",
-- conditions 13/55037. The Hut Fire despawns on the hit and records nothing. Rows are
-- renumbered so the credit runs before the despawn: smart_scripts is loaded ORDER BY id.
-- --------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts`
 WHERE `source_type` = 0 AND `entryorguid` = 29692 AND `id` IN (1, 2);

INSERT INTO `smart_scripts`
  (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,
   `event_param1`,`event_param2`,`event_param3`,`event_param4`,`event_param5`,
   `action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,
   `target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`)
VALUES
-- 10895
(21182, 0, 3, 0, 8, 0, 100, 0, 36374, 0, 0, 0, 0, 33, 21182, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Zeth''Gor Quest Credit Marker, Tower South - On Spellhit Summon Smoke Beacon - Quest Credit'),
(22401, 0, 3, 0, 8, 0, 100, 0, 36374, 0, 0, 0, 0, 33, 22401, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Zeth''Gor Quest Credit Marker, Tower North - On Spellhit Summon Smoke Beacon - Quest Credit'),
(22402, 0, 3, 0, 8, 0, 100, 0, 36374, 0, 0, 0, 0, 33, 22402, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Zeth''Gor Quest Credit Marker, Tower Forge - On Spellhit Summon Smoke Beacon - Quest Credit'),
(22403, 0, 3, 0, 8, 0, 100, 0, 36374, 0, 0, 0, 0, 33, 22403, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Zeth''Gor Quest Credit Marker, Tower Foothill - On Spellhit Summon Smoke Beacon - Quest Credit'),
-- 9805
(18110, 0, 1, 0, 8, 0, 100, 0, 31927, 0, 0, 0, 0, 33, 18110, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Windyreed Quest Credit (Big Hut) - On Spellhit Living Fire - Quest Credit'),
(18142, 0, 1, 0, 8, 0, 100, 0, 31927, 0, 0, 0, 0, 33, 18142, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Windyreed Quest Credit (Hut 01) - On Spellhit Living Fire - Quest Credit'),
(18143, 0, 1, 0, 8, 0, 100, 0, 31927, 0, 0, 0, 0, 33, 18143, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Windyreed Quest Credit (Hut 02) - On Spellhit Living Fire - Quest Credit'),
(18144, 0, 1, 0, 8, 0, 100, 0, 31927, 0, 0, 0, 0, 33, 18144, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Windyreed Quest Credit (Hut 03) - On Spellhit Living Fire - Quest Credit'),
-- 10233
(19723, 0, 1, 0, 8, 0, 100, 0, 34526, 0, 0, 0, 0, 33, 19723, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Invis BE Ballista - On Spellhit Burn Blood Elf Equipment - Quest Credit'),
(19724, 0, 1, 0, 8, 0, 100, 0, 34526, 0, 0, 0, 0, 33, 19724, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Invis BE Tent - On Spellhit Burn Blood Elf Equipment - Quest Credit'),
-- 11715
(25781, 0, 2, 3, 8, 0, 100, 0, 45990, 0, 0, 0, 0, 33, 25781, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Oil Pool - On Spellhit Collect Oil - Quest Credit'),
(25781, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 10000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Oil Pool - On Spellhit Collect Oil - Despawn In 10000 ms'),
-- 12859
(29692, 0, 1, 2, 8, 0, 100, 0, 55037, 0, 0, 0, 0, 33, 29692, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
 'Hut Fire - On Spellhit Fire Extinguisher - Quest Credit'),
(29692, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 'Hut Fire - On Spellhit Fire Extinguisher - Despawn Instant');

-- --------------------------------------------------------------------------------------------
-- 25446 "Frogs Away!" -- the credit row for 40188 Attuned Frog is present and correct in every
-- field except target_type, which is 0. Item 53637 summons the frog, so SMART_EVENT_JUST_SUMMONED
-- has the player as invoker; target 0 routes it through the never-set loot recipient instead.
-- --------------------------------------------------------------------------------------------
UPDATE `smart_scripts` SET `target_type` = 7
 WHERE `source_type` = 0 AND `entryorguid` = 40188
   AND `id` = 0 AND `event_type` = 54 AND `action_type` = 33 AND `action_param1` = 40218;
