-- Several Western Plaguelands quest givers cannot be seen, or are seen too early. Lady Sylvanas Windrunner (44451),
-- Zen'Kiki (44863) and Thurman Grant (45165) carry the quest invisibility aura 49414 but nothing grants the matching
-- detection aura, so they are invisible to every player and the quests they give and take (Ace in the Hole, The
-- Reckoning, A Mighty Hunger, This Is Our Army and others) cannot be started or handed in. Legends of Azeroth 5.4.8 has
-- no invisibility on these three, and they are visible again here.
-- Del Gahrron is the intended case of the aura: the one in Hearthglen (44905, starts The Long Trip Home) is seen in
-- Hearthglen until the quest is rewarded (detection 1 in area 203), and the one at the Menders' Stead (44458, ends
-- the quest and gives Memories from a Lost Past) is only seen there from taking the quest on (detection 2 in area
-- 5367). The second Del Gahrron had no aura, so both stood in the world at once and the detection rows were missing.
-- Rows are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template_addon` SET `auras` = NULL WHERE `entry` IN (44451, 44863, 45165) AND `auras` = '49414';

DELETE FROM `creature_template_addon` WHERE `entry` = 44458;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(44458, 0, 0, 65536, 1, 0, '49415');

DELETE FROM `spell_area` WHERE (`spell` = 49416 AND `area` = 203 AND `quest_end` = 26957) OR (`spell` = 49417 AND `area` = 5367 AND `quest_start` = 26957);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(49416, 203, 0, 26957, 0, 0, 2, 1, 0, 11),
(49417, 5367, 26957, 0, 0, 0, 2, 1, 74, 0);
