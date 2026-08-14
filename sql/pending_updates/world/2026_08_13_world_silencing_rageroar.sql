-- Quest 24956 Silencing Rageroar
-- Kill 4x Rageroar Lieutenant (38659) and loot Rageroar's Helm (52036) from Karga Rageroar (38663).
-- Encounter starts on player approach (OOC LoS ~5yd): Bastoon shields Karga with Shadow Barrier (72975),
-- plays dialogue, then engages. Karga stays immune/invincible until Bastoon dies, then attacks.
-- Bastoon must stay REACT_AGGRESSIVE so OOC LoS can fire (grid LoS skips passive NPCs); PACIFIED
-- blocks Attack() until the timed RP finishes.
-- Timed actionlists cannot use event_type 61 links (FindLinkedEvent only searches mEvents) — use delays.
-- Reset: phase 1 = waiting for LoS, phase 2 = RP/combat started. OOC timed list so a failed
-- AttackStart (player left) can still run a follow-up Evade; evade/reset clears the timed list.

-- Quest texts
UPDATE `quest_template` SET
    `RequestItemsText`='Is he dead? Do you have his ... helm?',
    `OfferRewardText`='<Tolliver grasps the helmet reverently with quivering hands.>$B$BRageroar''s battleships intercepted our vessel as we fled from Gilneas. We were to be carried here as trophies to the Warchief. As we neared the shore, some of us fought back. Rageroar - he began killing indiscriminately - howling with laughter. Our ship capsized and he ordered his men to fire at us as we drowned. Few of us made it to land.$B$BAfterwards, I learned that he wore my brother''s scalp as... as a trophy.$B$B<He closes his eyes.>',
    `Flags2`=8
WHERE `Id`=24956;

-- Templates: combat stats, loot link, SmartAI
-- Karga unit_flags 0x8300 = UNK_15|IMMUNE_TO_PC|IMMUNE_TO_NPC
UPDATE `creature_template` SET
    `AIName`='SmartAI',
    `lootid`=38663,
    `mindmg`=109,
    `maxdmg`=187,
    `attackpower`=15,
    `dmg_multiplier`=1,
    `unit_flags`=33536,
    `dynamicflags`=0,
    `skinloot`=0
WHERE `entry`=38663;

-- unit_flags 0x28300 = UNK_15|IMMUNE_TO_PC|IMMUNE_TO_NPC|PACIFIED
UPDATE `creature_template` SET
    `AIName`='SmartAI',
    `mindmg`=96,
    `maxdmg`=165,
    `attackpower`=13,
    `dmg_multiplier`=1.1,
    `unit_flags`=164608,
    `dynamicflags`=0,
    `skinloot`=0
WHERE `entry`=38826;

-- Melee sheath so dagger shows when engaged (bytes2 sheath = 1)
DELETE FROM `creature_template_addon` WHERE `entry`=38826;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(38826, 0, 0, 0, 1, 0, NULL);

UPDATE `creature_template` SET
    `mindmg`=109,
    `maxdmg`=187,
    `attackpower`=15,
    `lootid`=38659
WHERE `entry`=38659;

-- Spawn: real HP, default equipment, no override flags.
-- Open-world linked_respawn is instance-only, so Bastoon is the master timer (5 min).
-- Karga uses a long delay so loot corpse-decay cannot make him respawn first; Bastoon's
-- On Respawn SmartAI forces Karga up with him.
UPDATE `creature` SET
    `curhealth`=2012,
    `curmana`=0,
    `dynamicflags`=0,
    `unit_flags`=0,
    `equipment_id`=1,
    `spawntimesecs`=604800
WHERE `guid`=149552 AND `id`=38663;

UPDATE `creature` SET
    `curhealth`=773,
    `curmana`=811,
    `dynamicflags`=0,
    `unit_flags`=0,
    `equipment_id`=1,
    `spawntimesecs`=300
WHERE `guid`=149553 AND `id`=38826;

-- Quest helm only (wipe any junk rows on this loot id)
DELETE FROM `creature_loot_template` WHERE `entry`=38663;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(38663, 52036, -100, 1, 0, 1, 1);

-- Dialogue (sniff + encounter lines)
DELETE FROM `creature_text` WHERE `entry` IN (38663, 38826);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38663, 0, 0, 'Bastoon! Humiliate the interlopers.', 12, 0, 100, 0, 0, 0, 'Karga Rageroar'),
(38663, 1, 0, 'BAH! Your shadows frighten no one, Bastoon. This one is mine.', 12, 0, 100, 0, 0, 0, 'Karga Rageroar'),
(38663, 2, 0, 'No - no! This land... is ours...', 12, 0, 100, 0, 0, 0, 'Karga Rageroar'),
(38826, 0, 0, 'Oke doke, boss. I will finish this.', 12, 0, 100, 0, 0, 0, 'Hexmaster Bastoon'),
(38826, 1, 0, 'It be time to squeal, little piggy!', 12, 0, 100, 0, 0, 0, 'Hexmaster Bastoon');

-- SmartAI
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38663, 38826) AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (3882600) AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- Hexmaster Bastoon reset/evade: waiting phase, stop RP, immune/pacified, soft-reset Karga
-- 131840 = IMMUNE_TO_PC|IMMUNE_TO_NPC|PACIFIED (do NOT use LOOTING 0x400 — client kneel/loot pose)
(38826, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Set Phase 1'),
(38826, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Clear Timed Actionlist'),
(38826, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 131840, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Set Immune PC/NPC and Pacified'),
(38826, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 1024, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Remove Looting Flag'),
(38826, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Set React Aggressive'),
(38826, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Reset - Set Data 3 1 on Karga Rageroar'),
(38826, 0, 6, 7, 7, 0, 100, 0, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Set Phase 1'),
(38826, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Clear Timed Actionlist'),
(38826, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 18, 131840, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Set Immune PC/NPC and Pacified'),
(38826, 0, 9, 10, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 1024, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Remove Looting Flag'),
(38826, 0, 10, 11, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Set React Aggressive'),
(38826, 0, 11, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Evade - Set Data 3 1 on Karga Rageroar'),
-- OOC LoS ~5 yards in phase 1 only: enter phase 2, shield Karga, start OOC RP
(38826, 0, 12, 13, 10, 1, 100, 0, 0, 5, 0, 0, 1, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On OOC LoS (Phase 1) - Set Phase 2'),
(38826, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 2, 1, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On OOC LoS - Set Data 2 1 on Karga Rageroar'),
(38826, 0, 14, 15, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 72975, 0, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On OOC LoS - Cast Shadow Barrier on Karga Rageroar'),
(38826, 0, 15, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 80, 3882600, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On OOC LoS - Run Script (OOC)'),
(38826, 0, 16, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Death - Set Data 1 1 on Karga Rageroar'),
-- When Bastoon respawns, force Karga (guid 149552) to respawn too (open-world linked_respawn is instance-only)
(38826, 0, 17, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 0, 0, 0, 10, 149552, 38663, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - On Respawn - Respawn Karga Rageroar'),
-- Timed RP (OOC timers): dialogue, engage; if AttackStart fails and still OOC, Evade restores encounter
(3882600, 9, 0, 0, 0, 0, 100, 0, 2100, 2100, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Say Line 0'),
(3882600, 9, 1, 0, 0, 0, 100, 0, 3250, 3250, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Say Line 1'),
(3882600, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 132352, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Remove Immune PC, Pacified, Looting'),
(3882600, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Clear Emote State'),
(3882600, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 40, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Set Sheath Melee'),
(3882600, 9, 5, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Set React Aggressive'),
(3882600, 9, 6, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Attack Start Invoker'),
-- If still OOC after AttackStart (player left), restore waiting state (OOC timers skip while in combat)
(3882600, 9, 7, 0, 0, 0, 100, 0, 500, 500, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Set Phase 1 if still OOC'),
(3882600, 9, 8, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 18, 131840, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Restore Immune/Pacified if still OOC'),
(3882600, 9, 9, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 19, 1024, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Remove Looting if still OOC'),
(3882600, 9, 10, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Set React Aggressive if still OOC'),
(3882600, 9, 11, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 45, 3, 1, 0, 0, 0, 0, 19, 38663, 40, 0, 0, 0, 0, 0, 'Hexmaster Bastoon - Script - Soft-reset Karga if still OOC'),
-- Karga Rageroar reset: immune, passive, invincible, clear corpse bits
(38663, 0, 0, 1, 25, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Set Immune to PC and NPC'),
(38663, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Set React Passive'),
(38663, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 42, 0, 100, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Set Invincibility 100%'),
(38663, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 96, 33, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Remove DynFlags LOOTABLE|DEAD'),
(38663, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 19, 67108864, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Remove UNIT_FLAG_SKINNABLE'),
(38663, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 113, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Reset - Restore Full Health'),
(38663, 0, 6, 7, 7, 0, 100, 0, 0, 0, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Evade - Set Immune to PC and NPC'),
(38663, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Evade - Set React Passive'),
(38663, 0, 8, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 42, 0, 100, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Evade - Set Invincibility 100%'),
(38663, 0, 9, 0, 38, 0, 100, 0, 2, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 2 1 - Say Line 0'),
-- Soft reset when Bastoon aborts/evades mid-encounter
(38663, 0, 10, 11, 38, 0, 100, 0, 3, 1, 0, 0, 0, 18, 768, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 3 1 - Set Immune to PC and NPC'),
(38663, 0, 11, 12, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 3 1 - Set React Passive'),
(38663, 0, 12, 13, 61, 0, 100, 0, 0, 0, 0, 0, 0, 42, 0, 100, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 3 1 - Set Invincibility 100%'),
(38663, 0, 13, 14, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 72975, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 3 1 - Remove Aura Shadow Barrier'),
(38663, 0, 14, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 113, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 3 1 - Restore Full Health'),
-- Unlock after Bastoon dies
(38663, 0, 15, 16, 38, 0, 100, 0, 1, 1, 0, 0, 0, 19, 256, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Remove Immune to PC'),
(38663, 0, 16, 17, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 72975, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Remove Aura Shadow Barrier'),
(38663, 0, 17, 18, 61, 0, 100, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Clear Invincibility'),
(38663, 0, 18, 19, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Set React Aggressive'),
(38663, 0, 19, 20, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Say Line 1'),
(38663, 0, 20, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 21, 50, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - On Data 1 1 - Attack Closest Player'),
(38663, 0, 21, 0, 2, 0, 100, 1, 0, 25, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Karga Rageroar - Between 0-25% Health - Say Line 2');
