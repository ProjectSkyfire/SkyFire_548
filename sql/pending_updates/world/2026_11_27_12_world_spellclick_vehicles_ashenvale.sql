-- Ashenvale: the Watch Wind Riders (34160) carry Hellscream's Hellion (34163) on seat 0 as their vehicle accessory in
-- Legends of Azeroth 5.4.8, but the wind riders had no spellclick row, which our accessory loader needs, so no Hellion was
-- ever seated and each one stood loose at its wind rider. Add the accessory and the Legends of Azeroth click row (46598,
-- cast flags 1, user type 0); the wind riders get no click flag there either, they are not meant to be ridden by players.
-- All 27 Hellions are 1.21 yards from a wind rider with the same orientation, phase and spawn mask. The entry has no
-- other spawn (Legends of Azeroth, the only reference, has none either), is not pooled, in an event, formation, addon,
-- path, linked respawn or smart script, and is not summoned by a script or summon group.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (34160);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(34160, 34163, 0, 1, 'Watch Wind Rider - Hellscream''s Hellion (Seat 0)', 8, 0);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (34160);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(34160, 46598, 1, 0);

-- Wind rider guid and the loose Hellion guid it carries (1.21 yards apart, same orientation):
-- Entry 34163 carried by 34160, as vehicle/rider: 177883/177917, 177884/177918, 177885/177919, 177886/177920,
-- 177887/177921, 177888/177922, 177889/177923, 177890/177924, 177891/177925, 177894/177928, 177896/177930,
-- 177897/177931, 177898/177932, 177899/177933, 177901/177935, 177902/177936, 177904/177938, 177906/177940,
-- 177907/177941, 177908/177942, 177910/177944, 177911/177945, 177912/177946, 177913/177947, 177914/177948,
-- 177915/177949, 177916/177950.
DELETE FROM `creature` WHERE `guid` IN (177917, 177918, 177919, 177920, 177921, 177922, 177923, 177924, 177925,
    177928, 177930, 177931, 177932, 177933, 177935, 177936, 177938, 177940, 177941, 177942, 177944, 177945, 177946,
    177947, 177948, 177949, 177950) AND `id` = 34163;
