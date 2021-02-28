SET @SERPENT1 := 57445;
SET @SERPENT2 := 65335;
SET @RIDER := 57444;
SET @SPELL := 125095;

DELETE FROM `creature` WHERE guid IN (8156467, 8156487, 8163951, 8163958, 8163967, 8163988);
DELETE FROM `creature` WHERE guid IN (8163990, 8164004, 8164006, 8164590, 8164598, 8164600);
DELETE FROM `creature` WHERE guid IN (8164651, 8168222, 8168337, 8168491, 8168556, 8168563);
DELETE FROM `creature` WHERE guid IN (8168563, 8168565, 8168568, 8168570, 8168572, 8176719);

UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (@SERPENT1, @SERPENT2);

DELETE FROM `vehicle_template_accessory` WHERE `seat_id`=0 AND `entry` IN (@SERPENT1, @SERPENT2);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(@SERPENT1, @RIDER, 0, 0, 'War Serpent - Serpent Rider', 6, 30000),
(@SERPENT2, @RIDER, 0, 0, 'War Serpent - Serpent Rider', 6, 30000);

DELETE FROM `npc_spellclick_spells` WHERE `spell_id`=@SPELL AND `npc_entry` IN (@SERPENT1, @SERPENT2);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(@SERPENT1, @SPELL, 0, 0),
(@SERPENT2, @SPELL, 0, 0);
