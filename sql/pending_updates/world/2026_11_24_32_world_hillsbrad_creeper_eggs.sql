-- Glorious Harvest (28114) and Past Their Prime (28115): the Creeper Eggs (47203) around each Infested Bear (47204) are
-- clicked to collect them, and the bear is brought down to 1 percent health to get its credit. The eggs and the bears
-- were spawned but had no click flag, no click row and almost no script, so the eggs could not be collected and the bears
-- gave no credit. Added as in Legends of Azeroth 5.4.8: the click flag on both, the click rows (the egg creates the
-- item with spell 88695, the bear takes the player as a rider with spell 46598), the SmartAI of both (an egg dies when
-- collected and tells the bear next to it; the bear stays at 1 health, gives the credit, plays dead and dies once eight
-- eggs near it are gone), the egg-sparkle aura of the bear and its second unit flag. The bear's existing Sundering Swipe
-- row keeps id 0, the new rows follow it.

UPDATE `creature_template` SET `npcflag` = 16777216, `AIName` = 'SmartAI' WHERE `entry` = 47203;
UPDATE `creature_template` SET `npcflag` = 16777216, `unit_flags2` = 2048 WHERE `entry` = 47204;
UPDATE `creature_template_addon` SET `auras` = '87966' WHERE `entry` = 47204;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (47203, 47204);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(47203, 88695, 2, 0),
(47204, 46598, 1, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` = 47203 AND `source_type` = 0 AND `id` IN (0, 1, 2, 3, 4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47203, 0, 0, 0, 73, 0, 100, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Creeper Egg - On Spellclick - Die'),
(47203, 0, 1, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 67, 47203, 30000, 30000, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Creeper Egg - On Data Set 0 1 - Create Timed Event'),
(47203, 0, 2, 3, 59, 0, 100, 0, 47203, 0, 0, 0, 0, 51, 0, 0, 0, 0, 0, 0, 19, 47204, 3, 0, 0, 0, 0, 0, 'Creeper Egg - On Timed Event Triggered - Kill Infested Bear'),
(47203, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Creeper Egg - Linked To Id 0 - Despawn - Linked To Id 2 - Despawn'),
(47203, 0, 4, 5, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 47204, 3, 0, 0, 0, 0, 0, 'Creeper Egg - On Death - Set Data 0 1'),
(47203, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Creeper Egg - Linked To Id 4 - Despawn');

DELETE FROM `smart_scripts` WHERE `entryorguid` = 47204 AND `source_type` = 0 AND `id` IN (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47204, 0, 1, 2, 11, 0, 100, 0, 0, 0, 0, 0, 0, 42, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - On Respawn - Set Invincibility Hp 1'),
(47204, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 0 - Remove UnitFlag 768'),
(47204, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 114371, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 1 - Remove Aura Permanent Feign Death'),
(47204, 0, 4, 5, 2, 0, 100, 1, 0, 1, 0, 0, 0, 33, 47204, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Between 0-1% Health - Quest Credit'),
(47204, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 3 - Evade'),
(47204, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 11, 47203, 3, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 4 - Set Data 0 1'),
(47204, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 5 - Set UnitFlag 768'),
(47204, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 114371, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 6 - Cast Permanent Feign Death'),
(47204, 0, 9, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - Linked To Id 4 - On Death - Evade'),
(47204, 0, 10, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 116, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - On Data Set 0 1 - Set Counter 1 1'),
(47204, 0, 11, 0, 77, 0, 100, 0, 1, 8, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Infested Bear - On Counter 1 8 - Die');
