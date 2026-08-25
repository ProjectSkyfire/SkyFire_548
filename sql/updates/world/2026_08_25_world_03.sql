-- Quest 14005 The Vengeance of Elune: Heart of Corruption pocket spawns + scripts.
-- Pocket (map 1, ~40 69) was empty; corridor/eastern spur already present — only
-- insert inside this bounding box. Tarindrella grants Vengeance on accept;
-- Bough absorbs Tidal Pull / Starfall / Moonlight then explodes (C++).

-- Avoid double-spawns: clear only the Heart pocket for these entries.
DELETE FROM `creature` WHERE `map`=1 AND `id` IN (34517, 34521, 34522, 34524, 34525)
  AND `position_x` BETWEEN 9080 AND 9170 AND `position_y` BETWEEN 1755 AND 1865;

INSERT INTO `creature`
(`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
 `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
 `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8202600, 34517, 1, 1, 0, 0, 0, 0, 9136.04, 1765.81, 1319.39, 0, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202601, 34524, 1, 1, 0, 0, 0, 0, 9102.93, 1772.87, 1324.5, 5.42797, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202602, 34525, 1, 1, 0, 0, 0, 0, 9120.2, 1775.58, 1321.9, 6.05629, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202603, 34525, 1, 1, 0, 0, 0, 0, 9124.65, 1777.34, 1321.28, 4.06662, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202604, 34524, 1, 1, 0, 0, 0, 0, 9103.89, 1778.31, 1324.25, 5.55015, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202605, 34525, 1, 1, 0, 0, 0, 0, 9122.14, 1778.87, 1321.63, 4.79965, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202606, 34524, 1, 1, 0, 0, 0, 0, 9099.82, 1781.42, 1326.01, 5.42797, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202607, 34524, 1, 1, 0, 0, 0, 0, 9107.43, 1781.46, 1323.37, 5.42797, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202608, 34522, 1, 1, 0, 0, 0, 0, 9103.31, 1781.89, 1324.65, 5.2709, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202609, 34524, 1, 1, 0, 0, 0, 0, 9106.95, 1785.78, 1323.74, 5.3058, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202610, 34525, 1, 1, 0, 0, 0, 0, 9124.18, 1827.71, 1328.66, 1.06465, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202611, 34525, 1, 1, 0, 0, 0, 0, 9129.18, 1830.08, 1328.54, 1.32645, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202612, 34525, 1, 1, 0, 0, 0, 0, 9124.38, 1831.77, 1328.62, 0.942478, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202613, 34524, 1, 1, 0, 0, 0, 0, 9140.63, 1835.12, 1327.86, 2.32129, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202614, 34522, 1, 1, 0, 0, 0, 0, 9146.16, 1835.6, 1327.68, 2.60054, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202615, 34522, 1, 1, 0, 0, 0, 0, 9114.52, 1835.88, 1328.18, 0.383972, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202616, 34524, 1, 1, 0, 0, 0, 0, 9121.84, 1836.86, 1328.3, 0.593412, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202617, 34524, 1, 1, 0, 0, 0, 0, 9146.56, 1840.94, 1327.14, 2.94961, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202618, 34524, 1, 1, 0, 0, 0, 0, 9118.99, 1841.19, 1327.6, 0.20944, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202619, 34525, 1, 1, 0, 0, 0, 0, 9109.45, 1841.29, 1327.59, 0.0872665, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202620, 34524, 1, 1, 0, 0, 0, 0, 9141.28, 1841.81, 1326.81, 2.93215, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202621, 34524, 1, 1, 0, 0, 0, 0, 9122.16, 1842.75, 1327.26, 0.122173, 300, 0, 0, 1, 0, 0, 0, 0, 0),
(8202622, 34521, 1, 1, 0, 0, 0, 0, 9132.51, 1843.83, 1327.02, 4.32842, 60, 0, 0, 1, 0, 0, 0, 0, 0),
(8202623, 34525, 1, 1, 0, 0, 0, 0, 9106.83, 1844.28, 1327.59, 6.24828, 300, 0, 0, 1, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_tarindrella' WHERE `entry`=1992;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_bough_of_corruption' WHERE `entry`=34521;

DELETE FROM `spell_script_names` WHERE `spell_id`=65797 AND `ScriptName`='spell_q14005_vengeance_of_elune_starfall';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(65797, 'spell_q14005_vengeance_of_elune_starfall');

-- Bough dies via Suicide (7); ability damage does not clear m_PlayerDamageReq.
-- C++ grants kill credit. Turn-in RewardSpellCast 66166 clears aura 65602.
DELETE FROM `spell_script_names` WHERE `spell_id`=66166 AND `ScriptName`='spell_q14005_cancel_vengeance_of_elune';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66166, 'spell_q14005_cancel_vengeance_of_elune');
