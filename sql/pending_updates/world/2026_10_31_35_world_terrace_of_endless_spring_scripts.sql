-- Terrace of Endless Spring: bind the bosses, adds, spells and Lei Shi's Get Away area triggers to their scripts,
-- as in Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `ScriptName` = 'boss_protector_kaolan' WHERE `entry` = 60583;
UPDATE `creature_template` SET `ScriptName` = 'boss_ancient_regail' WHERE `entry` = 60585;
UPDATE `creature_template` SET `ScriptName` = 'boss_ancient_asani' WHERE `entry` = 60586;
UPDATE `creature_template` SET `ScriptName` = 'npc_corrupting_waters' WHERE `entry` = 60621;
UPDATE `creature_template` SET `ScriptName` = 'npc_cleansing_water' WHERE `entry` = 60646;
UPDATE `creature_template` SET `ScriptName` = 'npc_pure_light_terrace' WHERE `entry` = 60788;
UPDATE `creature_template` SET `ScriptName` = 'npc_minion_of_fear' WHERE `entry` = 60885;
UPDATE `creature_template` SET `ScriptName` = 'npc_coalesced_corruption' WHERE `entry` = 60886;
UPDATE `creature_template` SET `ScriptName` = 'npc_defiled_ground' WHERE `entry` = 60906;
UPDATE `creature_template` SET `ScriptName` = 'npc_minion_of_fear_controller' WHERE `entry` = 60957;
UPDATE `creature_template` SET `ScriptName` = 'boss_sha_of_fear' WHERE `entry` = 60999;
UPDATE `creature_template` SET `ScriptName` = 'npc_dread_spawn' WHERE `entry` = 61003;
UPDATE `creature_template` SET `ScriptName` = 'npc_terror_spawn' WHERE `entry` = 61034;
UPDATE `creature_template` SET `ScriptName` = 'npc_sha_of_fear_bowman' WHERE `entry` IN (61038, 61042, 61046);
UPDATE `creature_template` SET `ScriptName` = 'boss_tsulong' WHERE `entry` = 62442;
UPDATE `creature_template` SET `ScriptName` = 'npc_sunbeam' WHERE `entry` = 62849;
UPDATE `creature_template` SET `ScriptName` = 'npc_unstable_sha' WHERE `entry` = 62919;
UPDATE `creature_template` SET `ScriptName` = 'npc_embodied_terror' WHERE `entry` = 62969;
UPDATE `creature_template` SET `ScriptName` = 'npc_fright_spawn' WHERE `entry` = 62977;
UPDATE `creature_template` SET `ScriptName` = 'boss_lei_shi' WHERE `entry` = 62983;
UPDATE `creature_template` SET `ScriptName` = 'npc_animated_protector' WHERE `entry` = 62995;
UPDATE `creature_template` SET `ScriptName` = 'npc_lei_shi_hidden' WHERE `entry` = 63099;
UPDATE `creature_template` SET `ScriptName` = 'npc_lei_shi_corrupted_protector' WHERE `entry` = 63275;
UPDATE `creature_template` SET `ScriptName` = 'npc_light_of_day' WHERE `entry` = 63337;
UPDATE `creature_template` SET `ScriptName` = 'npc_dark_of_night' WHERE `entry` = 63346;
UPDATE `creature_template` SET `ScriptName` = 'npc_parasitoid_sha' WHERE `entry` = 64346;
UPDATE `creature_template` SET `ScriptName` = 'npc_apparition_of_fear' WHERE `entry` = 64368;
UPDATE `creature_template` SET `ScriptName` = 'npc_night_terrors' WHERE `entry` = 64390;
UPDATE `creature_template` SET `ScriptName` = 'npc_night_terror_summon' WHERE `entry` = 64393;
UPDATE `creature_template` SET `ScriptName` = 'npc_enchanted_plant' WHERE `entry` = 64443;
UPDATE `creature_template` SET `ScriptName` = 'npc_terrace_springtender_ashani' WHERE `entry` = 64846;
UPDATE `creature_template` SET `ScriptName` = 'npc_sha_globe' WHERE `entry` = 65691;
UPDATE `creature_template` SET `ScriptName` = 'npc_apparition_of_terror' WHERE `entry` = 66100;
UPDATE `creature_template` SET `ScriptName` = 'npc_lei_shi_reflection' WHERE `entry` = 71095;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (119414, 125786, 117866, 117283, 119108, 118191, 117230, 123740, 119887, 117988, 125843, 122768, 120047, 119983, 117955, 123461, 123467, 129378, 120438, 120193, 123244, 123233, 120629, 123610, 107145, 111850, 118040, 118053, 118054, 118055, 118064, 118077, 118004, 118005, 118007, 118008, 125736, 125745, 129147, 119593, 119692, 119693, 129075, 125697, 123705, 123712, 120458, 129189, 119985, 120455, 120519, 122789, 122789, 117905, 120221, 123018, 123012, 120285, 123742, 122775, 122438, 122453, 122952, 122855, 123011, 122881, 122907);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(119414, 'spell_breath_of_fear'),
(125786, 'spell_breath_of_fear_fear'),
(117866, 'spell_champion_of_light'),
(117283, 'spell_cleansing_waters_regen'),
(119108, 'spell_conjure_terror_spawn'),
(118191, 'spell_corrupted_essence'),
(117230, 'spell_corrupted_waters_empowered'),
(123740, 'spell_dark_of_night_fixate'),
(119887, 'spell_death_blossom'),
(117988, 'spell_defiled_ground_damage'),
(125843, 'spell_dread_shadows_damage'),
(122768, 'spell_dread_shadows_malus'),
(120047, 'spell_dread_spray'),
(119983, 'spell_dread_spray_stackable'),
(117955, 'spell_expelled_corruption'),
(123461, 'spell_get_away'),
(123467, 'spell_get_away_dmg'),
(129378, 'spell_heroic_fading_light'),
(120438, 'spell_heroic_sha_spine'),
(120193, 'spell_heroic_transition_screen_eff'),
(123244, 'spell_hide'),
(123233, 'spell_hide_stacks'),
(120629, 'spell_huddle_in_terror'),
(123610, 'spell_lei_shi_disperse'),
(107145, 'spell_light_wall_active'),
(111850, 'spell_lightning_prison'),
(118040, 'spell_lightning_storm_aura'),
(118053, 'spell_lightning_storm_aura'),
(118054, 'spell_lightning_storm_aura'),
(118055, 'spell_lightning_storm_aura'),
(118064, 'spell_lightning_storm_aura'),
(118077, 'spell_lightning_storm_aura'),
(118004, 'spell_lightning_storm_damage'),
(118005, 'spell_lightning_storm_damage'),
(118007, 'spell_lightning_storm_damage'),
(118008, 'spell_lightning_storm_damage'),
(125736, 'spell_night_terrors_missile'),
(125745, 'spell_night_terrors_periodic'),
(129147, 'spell_ominous_cackle_morph'),
(119593, 'spell_ominous_caclke_target'),
(119692, 'spell_ominous_caclke_target'),
(119693, 'spell_ominous_caclke_target'),
(129075, 'spell_penetrating_bolt'),
(125697, 'spell_player_vehicle_aura'),
(123705, 'spell_scary_fog_dot'),
(123712, 'spell_scary_fog_stacks'),
(120458, 'spell_sha_emerge'),
(129189, 'spell_sha_globe_regen'),
(119985, 'spell_sha_of_fear_dread_spray_fear'),
(120455, 'spell_sha_submerge'),
(120519, 'spell_sha_waterspout'),
(122789, 'spell_sunbeam'),
(122789, 'spell_sunbeam_aura'),
(117905, 'spell_superior_corrupted_essence'),
(120221, 'spell_teleport_to_dread_expanse'),
(123018, 'spell_terrorize_periodic_player'),
(123012, 'spell_terrorize_tsulong'),
(120285, 'spell_transfer_light'),
(123742, 'spell_tsulong_dark_of_night'),
(122775, 'spell_tsulong_nightmares'),
(122438, 'spell_tsulong_sha_regen'),
(122453, 'spell_tsulong_sha_regen'),
(122952, 'spell_tsulong_summon_sha_periodic_effect'),
(122855, 'spell_tsulong_sun_breath'),
(123011, 'spell_tsulong_terrorize'),
(122881, 'spell_tsulong_unstable_bolt'),
(122907, 'spell_tsulong_unstable_bolt_sacrifice');

DELETE FROM `spell_areatrigger_template` WHERE `Entry` IN (408, 409);
INSERT INTO `spell_areatrigger_template` (`Entry`, `Flags`, `CollisionType`, `Radius`, `ScaleX`, `ScaleY`, `ScriptName`) VALUES
(408, 0, 1, 40, 0, 0, 'sat_leishi_get_away'),
(409, 0, 1, 40, 0, 0, 'sat_leishi_get_away');


-- Their texts, from Legends of Azeroth 5.4.8.
DELETE FROM `creature_text` WHERE `entry` IN (60583, 60585, 60586, 60999, 62442, 62983, 71095);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(60583, 0, 0, 'The terrace was not meant for your kind!', 14, 0, 100, 0, 0, 28369, 'Protector Kaolan - TALK_INTRO'),
(60583, 1, 0, 'My strength grows!', 14, 0, 100, 0, 0, 28371, 'Protector Kaolan - TALK_KAOLAN_DIES_FIRST_ASANI - TALK_KAOLAN_DIES_FIRST_REGAIL'),
(60583, 2, 0, 'I will not be controlled!', 14, 0, 100, 0, 0, 28372, 'Protector Kaolan - TALK_EXPEL_CORRUPTION'),
(60583, 3, 0, 'None can match my strength!', 14, 0, 100, 0, 0, 28373, 'Protector Kaolan - TALK_KAOLAN_DIES_SECOND_ASANI - TALK_KAOLAN_DIES_SECOND_REGAIL'),
(60583, 4, 0, 'Weakling!', 14, 0, 100, 0, 0, 28374, 'Protector Kaolan - TALK_KAOLAN_SLAY'),
(60583, 4, 1, 'Begone!', 14, 0, 100, 0, 0, 28375, 'Protector Kaolan - TALK_KAOLAN_SLAY'),
(60583, 5, 0, 'We... are... endless...', 14, 0, 100, 0, 0, 28370, 'Protector Kaolan - TALK_KAOLAN_DEATH'),
(60585, 0, 0, 'A storm approaches.', 14, 0, 100, 0, 0, 28391, 'Elder Regail - TALK_REGAIL_AGGRO'),
(60585, 1, 0, 'I command the elements to destroy you!', 14, 0, 100, 0, 0, 28394, 'Elder Regail - TALK_REGAIL_DIES_FIRST_KAOLAN - TALK_REGAIL_DIES_FIRST_ASANI'),
(60585, 2, 0, 'You cannot escape the storm!', 14, 0, 100, 0, 0, 28395, 'Elder Regail - TALK_LIGHTNING_STORM'),
(60585, 3, 0, 'Your fear will consume you!', 14, 0, 100, 0, 0, 28393, 'Elder Regail - TALK_REGAIL_DIES_SECOND_ASANI - TALK_REGAIL_DIES_SECOND_KAOLAN'),
(60585, 4, 0, 'How dare you trespass here!', 14, 0, 100, 0, 0, 28396, 'Elder Regail - TALK_REGAIL_SLAY'),
(60585, 4, 1, 'Shocking.', 14, 0, 100, 0, 0, 28397, 'Elder Regail - TALK_REGAIL_SLAY'),
(60585, 5, 0, 'The terrace... must not... fall...', 14, 0, 100, 0, 0, 28392, 'Elder Regail - TALK_REGAIL_DEATH'),
(60586, 0, 0, 'The endless waters will cleanse you from this place!', 14, 0, 100, 0, 0, 28380, 'Elder Asani - TALK_ASANI_AGGRO'),
(60586, 1, 0, 'The tide cannot be stopped!', 14, 0, 100, 0, 0, 28383, 'Elder Asani - TALK_ASANI_DIES_FIRST_KAOLAN - TALK_ASANI_DIES_FIRST_REGAIL'),
(60586, 2, 0, 'The waters will consume you!', 14, 0, 100, 0, 0, 28382, 'Elder Asani - TALK_CORRUPTED_WATERS'),
(60586, 3, 0, 'Drown in fear!', 14, 0, 100, 0, 0, 28487, 'Elder Asani - TALK_ASANI_DIES_SECOND_KAOLAN - TALK_ASANI_DIES_SECOND_REGAIL'),
(60586, 3, 1, 'Drown in fear!', 14, 0, 100, 0, 0, 28487, 'Elder Asani - TALK_ASANI_DIES_SECOND_KAOLAN - TALK_ASANI_DIES_SECOND_REGAIL'),
(60586, 4, 0, 'You are unfit to stand before me.', 14, 0, 100, 0, 0, 28385, 'Elder Asani - TALK_ASANI_SLAY'),
(60586, 4, 1, 'Flee, child.', 14, 0, 100, 0, 0, 28386, 'Elder Asani - TALK_ASANI_SLAY'),
(60586, 5, 0, 'The Sha... must be... stopped...', 14, 0, 100, 0, 0, 28381, 'Elder Asani - TALK_ASANI_DEATH'),
(60999, 0, 0, 'Your lights will go out. The darkness will envelop you. And you will fear the shadows that move within it.', 14, 0, 100, 0, 0, 28402, 'Sha of Fear - SAY_INTRO'),
(60999, 1, 0, 'You will know fear!', 14, 0, 100, 0, 0, 28398, 'Sha of Fear - SAY_AGGRO'),
(60999, 2, 0, 'Fleeeee!', 14, 0, 100, 0, 0, 28404, 'Sha of Fear - SAY_BREATH_OF_FEAR'),
(60999, 3, 0, 'Drown in terror.', 14, 0, 100, 0, 0, 28403, 'Sha of Fear - SAY_SLAY'),
(60999, 4, 0, 'You think it is so easy to vanquish your fears?', 14, 0, 100, 0, 0, 28400, 'Sha of Fear - SAY_PHASE_2'),
(60999, 5, 0, 'Dread rises! It seeks the light. It hungers.', 14, 0, 100, 0, 0, 28401, 'Sha of Fear - SAY_SUBMERGE'),
(60999, 6, 0, 'Huddle in terror!', 14, 0, 100, 0, 0, 28405, 'Sha of Fear - SAY_HUDDLE'),
(62442, 3, 0, 'No... the waters... I must... resist... I shall not... fear...', 14, 0, 100, 0, 0, 29359, 'Tsulong - SAY_INTRO'),
(62442, 4, 0, 'You do not belong here! The waters must be protected... I will cast you out, or slay you!', 14, 0, 100, 0, 0, 29350, 'Tsulong - SAY_AGGRO'),
(62442, 5, 0, 'Flee this place... or die!', 14, 0, 100, 0, 0, 29365, 'Tsulong - SAY_NIGHTMARES'),
(62442, 5, 1, 'Overwhelming... fear!', 14, 0, 100, 0, 0, 29364, 'Tsulong - SAY_NIGHTMARES'),
(62442, 6, 0, 'Night... becomes day!', 14, 0, 100, 0, 0, 29356, 'Tsulong - SAY_NIGHT_TO_DAY'),
(62442, 6, 1, 'The sun... shines through...', 14, 0, 100, 0, 0, 29357, 'Tsulong - SAY_NIGHT_TO_DAY'),
(62442, 6, 2, 'Once more... the dawn...', 14, 0, 100, 0, 0, 29358, 'Tsulong - SAY_NIGHT_TO_DAY'),
(62442, 7, 0, 'Darkness grows...', 14, 0, 100, 0, 0, 29354, 'Tsulong - SAY_DAY_TO_NIGHT'),
(62442, 7, 1, 'Day falls to night...', 14, 0, 100, 0, 0, 29353, 'Tsulong - SAY_DAY_TO_NIGHT'),
(62442, 7, 2, 'Light fades...', 14, 0, 100, 0, 0, 29355, 'Tsulong - SAY_DAY_TO_NIGHT'),
(62442, 8, 0, 'No...!', 14, 0, 100, 0, 0, 29362, 'Tsulong - SAY_SLAY_DAY'),
(62442, 8, 1, 'Forgive me!', 14, 0, 100, 0, 0, 29363, 'Tsulong - SAY_SLAY_DAY'),
(62442, 9, 0, 'Die in darkness.', 14, 0, 100, 0, 0, 29360, 'Tsulong - SAY_SLAY_NIGHT'),
(62442, 9, 1, 'The night surrounds you...', 14, 0, 100, 0, 0, 29361, 'Tsulong - SAY_SLAY_NIGHT'),
(62442, 10, 0, 'I thank you, strangers. I have been freed.', 14, 0, 100, 0, 0, 29352, 'Tsulong - SAY_DEATH'),
(62442, 11, 0, 'Protect... the waters...', 14, 0, 100, 0, 0, 29351, 'Tsulong - SAY_WIPE'),
(62983, 1, 0, 'Wh-what are you doing here?! G-go away!', 14, 0, 100, 0, 0, 29328, 'Lei Shi - AGGRO'),
(62983, 2, 0, 'I\'m hiding until you leave!', 41, 0, 100, 0, 0, 29333, 'Lei Shi - HIDE'),
(62983, 2, 1, 'I don\'t want to see you anymore!', 41, 0, 100, 0, 0, 29334, 'Lei Shi - HIDE'),
(62983, 3, 0, 'Get AWAY!', 14, 0, 100, 0, 0, 29331, 'Lei Shi - GET AWAY'),
(62983, 3, 1, 'Stay away from me!', 14, 0, 100, 0, 0, 29332, 'Lei Shi - GET AWAY'),
(62983, 4, 0, 'S-sorry!', 14, 0, 100, 0, 0, 29329, 'Lei Shi - SLAY'),
(62983, 4, 1, 'It\'s your fault it happened!', 14, 0, 100, 0, 0, 29330, 'Lei Shi - SLAY'),
(62983, 5, 0, 'Wh-what are you doing here?! G-go away!', 14, 0, 100, 0, 0, 29328, 'Lei Shi - INTRO'),
(62983, 6, 0, 'I... ah... oh! Did I...? Was I...? It was... so... cloudy.', 14, 0, 100, 0, 0, 29325, 'Lei Shi - DEFEAT'),
(62983, 7, 0, 'I have to go now.', 14, 0, 100, 0, 0, 29326, 'Lei Shi - OUTRO_1'),
(62983, 8, 0, 'I have to make the water not cloudy anymore. Will... will you help?', 14, 0, 100, 0, 0, 29327, 'Lei Shi - OUTRO_2'),
(71095, 0, 0, 'Lei Shi\'s reflection ripples ominously.', 41, 0, 100, 0, 0, 0, 'Leishi - Intro 1'),
(71095, 1, 0, 'Her reflection ripples a second time.', 41, 0, 100, 0, 0, 0, 'leishi - Intro 2'),
(71095, 2, 0, '%s appears!', 41, 0, 100, 0, 0, 0, 'Leishi - Intro Done');

-- The raid's instance script.
UPDATE `instance_template` SET `script` = 'instance_terrace_of_endless_spring' WHERE `map` = 996;
