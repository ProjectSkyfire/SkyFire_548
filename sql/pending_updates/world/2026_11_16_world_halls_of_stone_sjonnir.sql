-- Sjonnir the Ironshaper: periodic summon auras, frenzy emote, and the abilities of his summoned dwarves and troggs.
-- The Forged Iron Trogg was friendly (faction 35) and never fought.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (50789, 50792, 50801, 50824, 59858, 59859);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(50789, 'spell_sjonnir_periodic_summon'),
(50792, 'spell_sjonnir_periodic_summon'),
(50801, 'spell_sjonnir_periodic_summon'),
(50824, 'spell_sjonnir_periodic_summon'),
(59858, 'spell_sjonnir_periodic_summon'),
(59859, 'spell_sjonnir_periodic_summon');

DELETE FROM `creature_text` WHERE `entry` = 27978 AND `groupid` = 3;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(27978, 3, 0, '%s goes into a frenzy!', 16, 0, 100, 0, 0, 0, 'Sjonnir The Ironshaper - Frenzy');

UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16 WHERE `entry` IN (27979, 31390) AND `faction_A` = 35;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (27979, 27982) AND `AIName` = '';

DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (27979, 27982);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(27979, 0, 0, 0, 0, 0, 100, 0, 3000, 5000, 3000, 5000, 0, 11, 50900, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Forged Iron Trogg - In Combat - Cast Lightning Shock'),
(27982, 0, 0, 0, 0, 0, 100, 0, 10000, 10000, 6000, 6000, 0, 11, 50895, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Forged Iron Dwarf - In Combat - Cast Lightning Tether');
