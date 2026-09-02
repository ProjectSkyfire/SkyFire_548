-- mod-prabowow: heirloom vendor at every new-character start position.
--
-- Only the NPC template and its spawns live here. The vendor list is built at
-- startup by modules/mod-prabowow/src/prabowow_heirloom_vendor.cpp from every
-- heirloom-quality item in Item-sparse.db2, so no npc_vendor rows are needed.
--
-- flags_extra 0x200000 = CREATURE_FLAG_EXTRA_ALL_PHASES (custom core flag):
-- Kezan, Gilneas, the Wandering Isle and Ebon Hold phase brand-new characters,
-- and a plain spawn would be invisible there.

SET @ENTRY := 900001;
SET @MODEL_SOURCE := 6740;   -- Innkeeper Allison (Stormwind): borrow her display id.
SET @GUID := 8300000;        -- spawn guids 8300001+, clear of upstream and other custom ranges.

DELETE FROM `creature` WHERE `id` = @ENTRY;
DELETE FROM `creature_template` WHERE `entry` = @ENTRY;

INSERT INTO `creature_template`
(`entry`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`,
 `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`,
 `speed_walk`, `speed_run`, `scale`, `unit_class`, `unit_flags`, `type`,
 `InhabitType`, `RegenHealth`, `flags_extra`, `ScriptName`)
SELECT @ENTRY, `modelid1`, 0, 0, 0, 'Heirloom Vendor', 'PraboWoW Quartermaster', 'Buy',
       90, 90, 4, 35, 35, 128,
       1, 1.14286, 1, 1, 0, 7,
       3, 1, 0x200000, ''
FROM `creature_template`
WHERE `entry` = @MODEL_SOURCE;

-- One vendor per distinct start position, 4 yards in front of the new
-- character and facing them. Covers every race/class incl. Death Knights,
-- Goblins, Worgen and Pandaren without hard-coding coordinates.
INSERT INTO `creature`
(`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
 `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
 `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
SELECT (@GUID := @GUID + 1), @ENTRY, `map`, 1, 0, 0, 0, 0,
       `position_x` + 4 * COS(`orientation`), `position_y` + 4 * SIN(`orientation`), `position_z`,
       MOD(`orientation` + PI(), 2 * PI()), 120, 0,
       0, 1, 0, 0, 0, 0, 0
FROM (
    SELECT DISTINCT `map`, `position_x`, `position_y`, `position_z`, `orientation`
    FROM `playercreateinfo`
) AS `start`
ORDER BY `map`, `position_x`, `position_y`;
