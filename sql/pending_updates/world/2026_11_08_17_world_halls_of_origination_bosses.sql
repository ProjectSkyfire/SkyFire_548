-- Halls of Origination: Isiset, Ammunae, Setesh and Rajh with their images, familiars, pods, spores, portal, void
-- creatures and solar winds.
UPDATE `creature_template` SET `ScriptName` = 'boss_isiset' WHERE `entry` = 39587;
UPDATE `creature_template` SET `ScriptName` = 'npc_isiset_astral_rain' WHERE `entry` = 39720;
UPDATE `creature_template` SET `ScriptName` = 'npc_isiset_celestial_call' WHERE `entry` = 39721;
UPDATE `creature_template` SET `ScriptName` = 'npc_isiset_veil_of_sky' WHERE `entry` = 39722;
UPDATE `creature_template` SET `ScriptName` = 'npc_isiset_astral_familiar' WHERE `entry` = 39795;
UPDATE `creature_template` SET `ScriptName` = 'boss_ammunae' WHERE `entry` = 39731;
UPDATE `creature_template` SET `ScriptName` = 'npc_ammunae_seedling_pod' WHERE `entry` = 40550;
UPDATE `creature_template` SET `ScriptName` = 'npc_ammunae_spore' WHERE `entry` = 40585;
UPDATE `creature_template` SET `ScriptName` = 'boss_setesh' WHERE `entry` = 39732;
UPDATE `creature_template` SET `ScriptName` = 'npc_setesh_chaos_portal' WHERE `entry` = 41055;
UPDATE `creature_template` SET `ScriptName` = 'npc_setesh_void_sentinel' WHERE `entry` = 41208;
UPDATE `creature_template` SET `ScriptName` = 'npc_setesh_void_seeker' WHERE `entry` = 41371;
UPDATE `creature_template` SET `ScriptName` = 'boss_rajh' WHERE `entry` = 39378;
UPDATE `creature_template` SET `ScriptName` = 'npc_rajh_solar_wind' WHERE `entry` = 39634;

DELETE FROM `spell_script_names` WHERE (`spell_id`, `ScriptName`) IN ((74137, 'spell_isiset_supernova_dis'), (76670, 'spell_isiset_supernova_dmg'));
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(74137, 'spell_isiset_supernova_dis'),
(76670, 'spell_isiset_supernova_dmg');

DELETE FROM `creature_text` WHERE `entry` IN (39587, 39731, 39732, 39378);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39378, 0, 0, 'Defilers! Wretches! Fiends! Begone from here!', 14, 0, 100, 0, 0, 18911, 'Rajh'),
(39378, 1, 0, 'Can you feel it? The blessed warmth of the sun?', 14, 0, 100, 0, 0, 18912, 'Rajh'),
(39378, 2, 0, 'I send you to your deity.', 14, 0, 100, 0, 0, 18914, 'Rajh to Player'),
(39378, 2, 1, 'I will take this life as an offering!', 14, 0, 100, 0, 0, 18913, 'Rajh to Player'),
(39378, 3, 0, 'Blazing rays of light, take me!', 14, 0, 100, 0, 0, 18910, 'Rajh to Player'),
(39378, 3, 1, 'I take this life as an offering!', 14, 0, 100, 0, 0, 18914, 'VO_HO_Rajh_Slay02'),
(39587, 0, 0, 'Eons of darkness... by your hand.', 14, 0, 100, 0, 0, 18842, 'VO_HO_Isiset_Death'),
(39587, 0, 1, 'My luster... wanes.', 14, 0, 100, 0, 0, 18844, 'VO_HO_Isiset_Event01'),
(39587, 1, 0, 'Gaze to the heavens! What do you see?', 14, 0, 100, 0, 0, 18843, 'VO_HO_Isiset_Engage'),
(39587, 2, 0, 'Bask in my radiance!', 14, 0, 100, 0, 0, 18845, 'VO_HO_Isiset_Event02'),
(39587, 3, 0, 'The glimmer of your life, extinguished.', 14, 0, 100, 0, 0, 18846, 'VO_HO_Isiset_Slay01'),
(39587, 3, 1, 'Insignificant!', 14, 0, 100, 0, 0, 18847, 'VO_HO_Isiset_Slay02'),
(39731, 0, 0, 'This chamber will flourish with your life energy!', 14, 0, 100, 0, 0, 18571, 'Ammunae - Aggro'),
(39731, 1, 0, 'Your life, UNLEASHED!', 14, 0, 100, 0, 0, 18572, 'Ammunae - Rampant Growth'),
(39731, 2, 0, 'The cycle continues...', 14, 0, 100, 0, 0, 18569, 'Ammunae - Death'),
(39731, 3, 0, 'Wither away!', 14, 0, 100, 0, 0, 18573, 'Ammunae - Slay 1'),
(39731, 3, 1, 'Waste of energy.', 14, 0, 100, 0, 0, 18574, 'Ammunae - Slay 2'),
(39732, 0, 0, 'Yes! Harness... your... hatred.', 14, 0, 100, 0, 0, 18552, 'VO_HO_Setesh_Death'),
(39732, 1, 0, 'You fear that which you cannot control. But can you control your fear?', 14, 0, 100, 0, 0, 18553, 'VO_HO_Setesh_Engage'),
(39732, 2, 0, 'Embrace the end!', 14, 0, 100, 0, 0, 18555, 'VO_HO_Setesh_Slay01'),
(39732, 2, 1, 'Do you understand now?', 14, 0, 100, 0, 0, 18556, 'VO_HO_Setesh_Slay02');
