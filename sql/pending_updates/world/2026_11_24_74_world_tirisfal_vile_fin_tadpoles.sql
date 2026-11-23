-- Planting the Seed of Fear (24999) asks for 12 Vile Fin Tadpoles frightened. In Legends of Azeroth 5.4.8 the tadpole
-- can be clicked (npcflag 16777216, spell 73134 Tadpole Frightening Aura cast on itself), the click gives the quest
-- credit to the player, the tadpole is frightened by the aura, dies after three stacks and despawns. The tadpoles
-- could not be clicked here and the old script waited for a frightening that nothing caused.

UPDATE `creature_template` SET `npcflag` = 16777216 WHERE `entry` = 38937;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 38937;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38937, 73134, 0, 0);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38937) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38937, 0, 0, 1, 73, 0, 100, 1, 0, 0, 0, 0, 0, 41, 15000, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'q24999 - triger despawn'),
(38937, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 33, 38937, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'q24999 - instant questcredit'),
(38937, 0, 2, 0, 23, 0, 100, 1, 73133, 3, 10000, 12000, 0, 37, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'q24999 - die if 3 stuck aura');
