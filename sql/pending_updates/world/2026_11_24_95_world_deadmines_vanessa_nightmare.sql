-- The Deadmines, Vanessa's Nightmare (heroic): the Steam Valves of the trap are clickable and counted by their script, James
-- Harrington carries Calissa as a vehicle, and the spells of the event hit players only (their area targets have no condition,
-- so they also reached the other creatures around). The Harringtons and the Collapsing Icicle were level 1 placeholders:
-- James could not be attacked, Erik and Calissa were friendly to the Enraged Worgen and the Ice Shards came from a level 1
-- caster.

-- The valves come from the script now: the four spawns that were always in the room stay as a position reference only
UPDATE `creature` SET `spawnMask` = 0 WHERE `guid` IN (8203557, 8203558, 8203559, 8203560) AND `id` = 49457;

UPDATE `creature_template` SET `npcflag` = 16777216, `ScriptName` = 'npc_deadmines_steam_valve' WHERE `entry` = 49457;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 49457;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(49457, 92399, 0, 0);

UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 14, `faction_H` = 14, `unit_flags` = 32832, `baseattacktime` = 2000, `rangeattacktime` = 2000, `VehicleId` = 1403 WHERE `entry` = 49539;
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 2263, `faction_H` = 2263, `baseattacktime` = 2000, `rangeattacktime` = 2000 WHERE `entry` IN (49535, 49536);
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33587200, `baseattacktime` = 2000, `rangeattacktime` = 2000 WHERE `entry` IN (49481, 57866);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` IN (92113, 92120, 92171, 92203, 92278, 92425, 92559, 92563, 92603, 92604, 95520);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 1, 92113, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Nightmare Elixir - players only'),
(13, 1, 92120, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Vanessa''s Blackout Aura - players only'),
(13, 1, 92171, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Nightmare Flames - players only'),
(13, 7, 92203, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Ice Shards - players only'),
(13, 1, 92278, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Spark - players only'),
(13, 1, 92425, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Molten Magma - players only'),
(13, 1, 92559, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Nightmare Slow - players only'),
(13, 1, 92563, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Nightmare Aura - players only'),
(13, 1, 92603, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Nightmare''s End - players only'),
(13, 1, 92604, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Adrenaline - players only'),
(13, 1, 95520, 0, 0, 32, 0, 144, 0, 0, 0, 0, 0, '', 'Spark - players only');
