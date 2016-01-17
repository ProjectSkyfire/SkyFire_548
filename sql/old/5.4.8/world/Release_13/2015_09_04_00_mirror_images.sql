DELETE FROM `spell_script_names` WHERE `spell_id` IN (45206, 45205, 41054);
INSERT INTO `spell_script_names` VALUES
(45206, 'spell_gen_clone_weapon'),
(45205, 'spell_gen_clone_weapon_aura'),
(41054, 'spell_gen_clone_weapon_aura');

UPDATE `creature_template` SET `scriptname`='npc_pet_mage_mirror_image' WHERE `entry` IN (31216, 47243, 47244);
