-- Southern Barrens: the Triumph War Gryphons (37978) carry a Wildhammer Mercenary (37977) on seat 0 as their vehicle
-- accessory in Legends of Azeroth 5.4.8, but the gryphons had no spellclick row, which our accessory loader needs, so no
-- Mercenary was ever seated: five stood loose at their gryphons and the sixth gryphon had no rider. Add the accessory and
-- the Legends of Azeroth click row (46598, cast flags 1, user type 0); the gryphons get no click flag there either.
-- The five loose Mercenaries are 1.57 yards from a gryphon with the same orientation, phase and spawn mask. They are the
-- only spawns of the entry (Legends of Azeroth, the only reference, has none), are not pooled, in an event, formation,
-- addon, path or linked respawn, and are not summoned by a script or summon group. The smart script of the entry only
-- casts in combat and stays with the entry.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (37978);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(37978, 37977, 0, 0, 'Triumph War Gryphon', 6, 30000);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (37978);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(37978, 46598, 1, 0);

-- Gryphon guid and the loose Mercenary guid it carries (1.57 yards apart, same orientation):
-- Entry 37977 carried by 37978, as vehicle/rider: 153544/153545, 153551/153552, 153554/153555, 153556/153557,
-- 153580/153581.
DELETE FROM `creature` WHERE `guid` IN (153545, 153552, 153555, 153557, 153581) AND `id` = 37977;
