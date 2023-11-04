UPDATE `creature_template` SET `faction_A`=2082, `faction_H`=2082, `unit_flags`=16777224, `ScriptName`='npc_eye_of_acherus' WHERE `entry`=28511;

DELETE FROM `spell_script_names` WHERE `spell_id`=51904 AND `ScriptName`='spell_q12641_death_comes_from_on_high_summon_ghouls';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (51904, 'spell_q12641_death_comes_from_on_high_summon_ghouls');

DELETE FROM `creature_text` WHERE `entry`=28511;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (28511, 0, 0, 'The Eye of Acherus launches towards its destination.', 42, 0, 100, 0, 0, 0, 'Eye of Acherus');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (28511, 1, 0, 'The Eye of Acherus is in your control.', 42, 0, 100, 0, 0, 0, 'Eye of Acherus');

UPDATE `quest_template` SET `SpecialFlags`=32, `DetailsEmote2`=396, `DetailsEmote3`=397, `DetailsEmote4`=396 WHERE `Id`=12641;
