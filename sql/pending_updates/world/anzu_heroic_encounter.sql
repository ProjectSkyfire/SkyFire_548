-- Anzu: permanent heroic spawn and encounter support (#1524, #1525, #1526).
-- Spawn contribution: BrewingCoder, PR #1525. Coordinates verified against
-- legacy summon event 14797; use literals so importing does not depend on it.
-- Heroic is difficulty 2 in this core, so its spawn mask is 4.
UPDATE `creature` SET `spawnMask` = 4 WHERE `id` = 23035 AND `map` = 556;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
    `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
    `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
SELECT 23035, 556, 4, 0, 0, 0, 0,
    -78.3603, 288.525, 26.4832, 3.21359, 86400, 0,
    0, 0, 0, 0, 0, 0, 0
WHERE NOT EXISTS (SELECT 1 FROM `creature` WHERE `id` = 23035 AND `map` = 556);

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_brood_of_anzu' WHERE `entry` = 23132;

-- MoP Classic encounter quotes: https://www.wowhead.com/mop-classic/npc=23035/anzu
-- Only replace the groups consumed by boss_anzu; preserve other text groups.
DELETE FROM `creature_text` WHERE `entry` = 23035 AND `groupid` IN (0, 1);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23035, 0, 0, 'Awaken, my children and assist your master!', 14, 0, 100, 0, 0, 0, 'Anzu - Summon brood'),
(23035, 1, 0, 'Yes... cast your precious little spells, ak-a-ak!', 15, 0, 100, 0, 0, 0, 'Anzu - Spell Bomb'),
(23035, 1, 1, 'Your magics shall be your undoing... ak-a-ak...', 15, 0, 100, 0, 0, 0, 'Anzu - Spell Bomb'),
(23035, 1, 2, 'Your spells... ke-kaw... are weak magics... easy to turn against you...', 15, 0, 100, 0, 0, 0, 'Anzu - Spell Bomb');
