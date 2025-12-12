-- Fix creature_texts error - remove non-existent sound 11198

-- Entry 19220, Group 1, ID 1 has Sound 11198 which does not exist in MoP SoundEntries DBC
-- Setting sound to 0 (no sound) to fix the error
UPDATE `creature_text` SET `sound` = 0 WHERE `entry` = 19220 AND `groupid` = 1 AND `id` = 1 AND `sound` = 11198;

