-- Quest 24939 Run Out the Guns: Northwatch Shore Battery vehicle + Rageroar Rowboat destroy credit.
-- Sniff flow: board 38754 -> vehicle spell 72741 Fire the Battery -> impact 72755 ->
-- boat casts Explosion / Eject / Breath / Rowboat Sinks -> kill credit 38747 x10.
-- Three boats spawn offshore, stay map-active so waypoints run out of view, path to shore,
-- wait; on destroy they despawn in place and respawn (~5s) at offshore start.

-- ---------------------------------------------------------------------------
-- Templates
-- ---------------------------------------------------------------------------
UPDATE `creature_template` SET
    `AIName` = '',
    `ScriptName` = '',
    `npcflag` = (`npcflag` | 0x01000000),
    `VehicleId` = 651,
    `spell1` = 72741
WHERE `entry` = 38754;

-- Display 23258 only (328 is trigger bunny). AIR + disable-gravity (via SAI), no hover bytes
-- (hover + WP Z=0 sank the mesh underwater at path end).
UPDATE `creature_template` SET
    `AIName` = 'SmartAI',
    `ScriptName` = '',
    `VehicleId` = 650,
    `modelid1` = 23258,
    `modelid2` = 0,
    `modelid3` = 0,
    `modelid4` = 0,
    `unit_flags2` = 2099200,
    `InhabitType` = 4,
    `HoverHeight` = 0,
    `faction_A` = 2211,
    `faction_H` = 2211
WHERE `entry` = 38747;

DELETE FROM `creature_template_addon` WHERE `entry` = 38747;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(38747, 0, 0, 0, 1, 0, '72697');

-- Sea Dogs as vehicle passengers (spellclick required for InstallAccessory).
-- Vehicle 650 has three seats (7557/7558/7559).
DELETE FROM `vehicle_template_accessory` WHERE `entry` = 38747;
INSERT INTO `vehicle_template_accessory`
(`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(38747, 38661, 0, 1, 'Rageroar Rowboat - Rageroar Sea Dog', 8, 0),
(38747, 38661, 1, 1, 'Rageroar Rowboat - Rageroar Sea Dog', 8, 0),
(38747, 38661, 2, 1, 'Rageroar Rowboat - Rageroar Sea Dog', 8, 0);

DELETE FROM `vehicle_accessory` WHERE `guid` IN (155557, 155566, 155570);
INSERT INTO `vehicle_accessory`
(`guid`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(155557, 38661, 0, 1, 'Rageroar Rowboat 155557 - Sea Dog', 8, 0),
(155557, 38661, 1, 1, 'Rageroar Rowboat 155557 - Sea Dog', 8, 0),
(155557, 38661, 2, 1, 'Rageroar Rowboat 155557 - Sea Dog', 8, 0),
(155566, 38661, 0, 1, 'Rageroar Rowboat 155566 - Sea Dog', 8, 0),
(155566, 38661, 1, 1, 'Rageroar Rowboat 155566 - Sea Dog', 8, 0),
(155566, 38661, 2, 1, 'Rageroar Rowboat 155566 - Sea Dog', 8, 0),
(155570, 38661, 0, 1, 'Rageroar Rowboat 155570 - Sea Dog', 8, 0),
(155570, 38661, 1, 1, 'Rageroar Rowboat 155570 - Sea Dog', 8, 0),
(155570, 38661, 2, 1, 'Rageroar Rowboat 155570 - Sea Dog', 8, 0);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 38747;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38747, 46598, 1, 0);

-- ---------------------------------------------------------------------------
-- Battery boarding + quest gate
-- ---------------------------------------------------------------------------
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 38754;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(38754, 46598, 1, 0);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` = 38754 AND `SourceEntry` = 46598;
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 38754, 46598, 0, 0, 9, 0, 24939, 0, 0, 0, 0, 0, '', 'Northwatch Shore Battery spellclick requires quest 24939 taken'),
(18, 38754, 46598, 0, 0, 8, 0, 24939, 0, 0, 1, 0, 0, '', 'Northwatch Shore Battery spellclick blocked if quest 24939 rewarded');

-- ---------------------------------------------------------------------------
-- Rowboat SmartAI
-- ---------------------------------------------------------------------------
DELETE FROM `smart_scripts` WHERE `entryorguid` = 38747 AND `source_type` = 0;
INSERT INTO `smart_scripts`
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`,
 `event_param1`, `event_param2`, `event_param3`, `event_param4`,
 `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`,
 `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- MUST set active or waypoints freeze until a player loads the spawn grid (riders also look like they "port" on approach).
(38747, 0, 0, 8, 11, 0, 100, 0, 0, 0, 0, 0, 75, 72697, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - On Respawn - Add aura I Am a Rowboat (Head)'),
(38747, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 48, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Set active (path out of view)'),
(38747, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 60, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Disable gravity (float on water)'),
(38747, 0, 1, 2, 8, 0, 100, 1, 72755, 0, 0, 0, 103, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - On Spellhit - Set rooted'),
(38747, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 11, 62987, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Cast Explosion'),
(38747, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 11, 50630, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Cast Eject All Passengers'),
(38747, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 11, 72692, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Cast I Am a Rowboat (Breath)'),
(38747, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 11, 72710, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Cast Rowboat Sinks'),
-- Invoker = impact caster (player or shore battery). Vehicle targets credit their passenger.
-- Old closest-player@100y failed while boats were still ~160y offshore.
(38747, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 33, 38747, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Kill credit to spell invoker'),
(38747, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Rageroar Rowboat - Link - Despawn in 2 seconds (respawn ~5s total)');

-- ---------------------------------------------------------------------------
-- Freestanding Sea Dogs duplicate vehicle passengers
-- ---------------------------------------------------------------------------
DELETE FROM `creature_addon` WHERE `guid` IN (155558,155559,155560,155567,155568,155569,155571,155572,155573);
DELETE FROM `creature` WHERE `guid` IN (155558,155559,155560,155567,155568,155569,155571,155572,155573);

-- ---------------------------------------------------------------------------
-- Paths start at the first travel point (not the spawn) so respawn immediately moves shoreward.
-- Surface Z -0.5. Last point long delay = park at shore.
-- ---------------------------------------------------------------------------
DELETE FROM `waypoint_data` WHERE `id` BETWEEN 38747001 AND 38747014;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(38747002, 1, -1950.0, -3835.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747002, 2, -1985.0, -3810.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747002, 3, -2009.9, -3800.85, -0.5, 0, 600000, 1, 0, 100, 0),
(38747003, 1, -1945.0, -3845.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747003, 2, -1970.0, -3810.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747003, 3, -1978.21, -3786.03, -0.5, 0, 600000, 1, 0, 100, 0),
(38747004, 1, -1935.0, -3825.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747004, 2, -1970.0, -3800.0, -0.5, 0, 0, 1, 0, 100, 0),
(38747004, 3, -1996.78, -3783.02, -0.5, 0, 600000, 1, 0, 100, 0);

-- Offshore spawns, 5s respawn, not selectable
UPDATE `creature` SET
    `modelid` = 0,
    `MovementType` = 2,
    `unit_flags` = 33587968,
    `position_x` = -1918.635,
    `position_y` = -3873.168,
    `position_z` = -0.5,
    `orientation` = 2.595023,
    `spawntimesecs` = 3
WHERE `guid` = 155557 AND `id` = 38747;
UPDATE `creature` SET
    `modelid` = 0,
    `MovementType` = 2,
    `unit_flags` = 33587968,
    `position_x` = -1912.818,
    `position_y` = -3888.986,
    `position_z` = -0.5,
    `orientation` = 2.701938,
    `spawntimesecs` = 3
WHERE `guid` = 155566 AND `id` = 38747;
UPDATE `creature` SET
    `modelid` = 0,
    `MovementType` = 2,
    `unit_flags` = 33587968,
    `position_x` = -1900.064,
    `position_y` = -3863.575,
    `position_z` = -0.5,
    `orientation` = 1.805945,
    `spawntimesecs` = 3
WHERE `guid` = 155570 AND `id` = 38747;

DELETE FROM `creature_addon` WHERE `guid` IN (155557, 155566, 155570) OR `guid` BETWEEN 8200500 AND 8200512;
DELETE FROM `creature` WHERE `guid` BETWEEN 8200500 AND 8200512;

INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(155557, 38747002, 0, 0, 1, 0, '72697'),
(155566, 38747003, 0, 0, 1, 0, '72697'),
(155570, 38747004, 0, 0, 1, 0, '72697');

INSERT INTO `creature`
(`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
 `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
 `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8200500, 38754, 1, 1, 0, 0, 0, 0, -2018.172, -3754.094, 6.358092, 5.358161, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8200501, 38754, 1, 1, 0, 0, 0, 0, -2012.181, -3748.368, 2.770374, 5.375614, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8200502, 38754, 1, 1, 0, 0, 0, 0, -2007.352, -3740.085, 5.157124, 5.026548, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8200503, 38754, 1, 1, 0, 0, 0, 0, -1998.889, -3738.745, 5.70387, 5.113815, 300, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `creature_queststarter` WHERE `quest` = 24939 AND `id` = 3454;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES (3454, 24939);
DELETE FROM `creature_questender` WHERE `quest` = 24939 AND `id` = 3454;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (3454, 24939);

DELETE FROM `quest_objective` WHERE `questId` = 24939 AND `id` = 267290;
INSERT INTO `quest_objective` (`questId`, `id`, `index`, `type`, `objectId`, `amount`, `flags`, `description`) VALUES
(24939, 267290, 0, 0, 38747, 10, 0, 'Rageroar Rowboat Destroyed');
