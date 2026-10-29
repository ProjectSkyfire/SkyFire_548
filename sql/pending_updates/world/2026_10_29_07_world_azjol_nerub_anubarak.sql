-- Azjol-Nerub: Anub'arak submerge phases and adds (TrinityCore 3.3.5 data)

-- Script bindings: the adds of the submerge phases and the Impale target
UPDATE `creature_template` SET `ScriptName` = 'npc_anubarak_anub_ar_darter' WHERE `entry` = 29213 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anubarak_anub_ar_assassin' WHERE `entry` = 29214 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anubarak_anub_ar_guardian' WHERE `entry` = 29216 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anubarak_anub_ar_venomancer' WHERE `entry` = 29217 AND `ScriptName` = '' AND `AIName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_anubarak_impale_target' WHERE `entry` = 29184 AND `ScriptName` = '' AND `AIName` = '';

-- Anub'arak (normal and heroic) is immune to the same mechanics as in TrinityCore
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853239 WHERE `entry` IN (29120, 31610);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (53472, 59433, 53520) AND `ScriptName` IN ('spell_anubarak_pound', 'spell_anubarak_carrion_beetles');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(53472, 'spell_anubarak_pound'),
(59433, 'spell_anubarak_pound'),
(53520, 'spell_anubarak_carrion_beetles');
