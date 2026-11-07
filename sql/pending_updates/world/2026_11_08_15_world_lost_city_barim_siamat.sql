-- Lost City of the Tol'vir: High Prophet Barim with his Repentance, Harbinger, Soul Fragments and Blaze of the Heavens,
-- and Siamat with his Servants, Minions and Cloud Bursts.
UPDATE `creature_template` SET `ScriptName` = 'boss_high_prophet_barim' WHERE `entry` = 43612;
UPDATE `creature_template` SET `ScriptName` = 'npc_repentance_mirror' WHERE `entry` = 43817;
UPDATE `creature_template` SET `ScriptName` = 'npc_harbinger_of_darkness' WHERE `entry` = 43927;
UPDATE `creature_template` SET `ScriptName` = 'npc_lstv_soul_fragment' WHERE `entry` = 43934;
UPDATE `creature_template` SET `ScriptName` = 'npc_blaze_of_the_heavens' WHERE `entry` = 48906;
UPDATE `creature_template` SET `ScriptName` = 'boss_siamat' WHERE `entry` = 44819;
UPDATE `creature_template` SET `ScriptName` = 'npc_servant_of_siamat' WHERE `entry` IN (45259, 45268, 45269);
UPDATE `creature_template` SET `ScriptName` = 'npc_siamat_minion' WHERE `entry` = 44704;
UPDATE `creature_template` SET `ScriptName` = 'npc_cloud_burst' WHERE `entry` = 44541;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (69893, 81947, 81958, 81960, 82009, 82168, 82215, 82220, 83566, 84982) AND `ScriptName` IN ('spell_copy_melee_weapon', 'spell_repentance_player_kneel', 'spell_repentance_player_summon_mirror', 'spell_repentance_trigger_clone_spell', 'spell_repentance_pull_player', 'spell_wailing_winds', 'spell_gathered_storms');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(69893, 'spell_copy_melee_weapon'),
(81947, 'spell_repentance_player_kneel'),
(81958, 'spell_repentance_player_summon_mirror'),
(81960, 'spell_repentance_trigger_clone_spell'),
(82009, 'spell_repentance_trigger_clone_spell'),
(82168, 'spell_repentance_pull_player'),
(82215, 'spell_repentance_player_summon_mirror'),
(82220, 'spell_repentance_trigger_clone_spell'),
(83566, 'spell_wailing_winds'),
(84982, 'spell_gathered_storms');

DELETE FROM `creature_text` WHERE `entry` IN (43612, 44819);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43612, 0, 0, 'Begone infidels, you are not welcome here!', 14, 0, 100, 0, 0, 0, 'High Prophet Barim - Aggro'),
(43612, 1, 0, 'May peace find you now.', 14, 0, 100, 0, 0, 0, 'High Prophet Barim - Kill'),
(43612, 1, 1, 'The heavens take you!', 14, 0, 100, 0, 0, 0, 'High Prophet Barim - Kill'),
(43612, 2, 0, 'Kneel before me and repent!', 14, 0, 100, 0, 0, 0, 'High Prophet Barim - Repentance'),
(43612, 3, 0, 'Death is only the beginning!', 14, 0, 100, 0, 0, 0, 'High Prophet Barim - Death'),
(44819, 0, 0, 'I. AM. UNLEASHED!', 14, 0, 100, 0, 0, 0, 'Siamat - Aggro'),
(44819, 1, 0, 'Winds of the south, rise and come to your master''s aid!', 14, 0, 100, 0, 0, 0, 'Siamat - Servants'),
(44819, 1, 1, 'Suffer the storm!', 14, 0, 100, 0, 0, 0, 'Siamat - Servants'),
(44819, 2, 0, 'Cower before the tempest storm!', 14, 0, 100, 0, 0, 0, 'Siamat - Wailing Winds'),
(44819, 3, 0, 'Nothing more than dust in the wind.', 14, 0, 100, 0, 0, 0, 'Siamat - Kill'),
(44819, 4, 0, 'The sky... Beckons...', 14, 0, 100, 0, 0, 0, 'Siamat - Death');
