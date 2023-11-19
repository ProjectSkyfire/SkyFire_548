UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=56739;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=56739;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
(56739, 46598, 0);

DELETE FROM `vehicle_template_accessory` WHERE `entry`=56739;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(56739, 54131, 0, 0, 'Hozen Hanging bunny - Fe Feng Hozen', 6, 30000);

UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=54131;
DELETE FROM `smart_scripts` WHERE `entryorguid`=54131;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(54131, 0, 0, 0, 1, 0, 100, 1, 500, 500, 500, 500, 0, 11, 46598, 3, 0, 0, 0, 0, 19, 56739, 5, 0, 0, 0, 0, 0, 'Fe Feng Hozen - OOC once - Cast Ride Vehicle Hardcoded on Hozen Hanging bunny'),
(54131, 0, 1, 0, 0, 0, 100, 1, 500, 500, 500, 500, 0, 11, 46598, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Fe Feng Hozen - IC once - Cast Ride Vehicle Hardcoded on self');

DELETE FROM `creature` WHERE `guid` IN (224774, 224554, 224553, 224574, 224570, 224666, 224658, 224673, 224660, 224688, 224580, 224548, 224542, 224715, 224714, 224692, 224675, 224707, 224705, 224783, 224784, 224779, 224786, 224718);

UPDATE `creature` SET `unit_flags`=256|512|262144|536870912 WHERE `guid` IN
(224557, 224564, 224562, 224560, 224638, 224563, 224566);
