-- Stonetalon Mountains: the Krom'gar Incinerators (34353) carry an Oil Canister (34345) on seat 0 as their vehicle
-- accessory in Legends of Azeroth 5.4.8, but the incinerators had no spellclick row, which our accessory loader needs, so
-- no canister was ever seated and each one stood loose at its incinerator. Add the accessory and the Legends of Azeroth
-- click row (46598, cast flags 0, user type 0); the incinerators get no click flag there either, they are hostile.
-- All 18 canisters are 0.54 to 0.62 yards from an incinerator with the same orientation, phase and spawn mask. The entry
-- has no other spawn (Legends of Azeroth, the only reference, has none either), is not pooled, in an event, formation,
-- addon, path, linked respawn or smart script, and is not summoned by a script or summon group.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (34353);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(34353, 34345, 0, 1, 'Krom''gar Incinerator - Oil Canister (Seat 0)', 8, 0);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (34353);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(34353, 46598, 0, 0);

-- Incinerator guid and the loose Oil Canister guid it carries (0.54 to 0.62 yards apart, same orientation):
-- Entry 34345 carried by 34353, as vehicle/rider: 215754/215755, 215756/215757, 215770/215771, 215772/215773,
-- 215774/215775, 215776/215777, 215859/215860, 215876/215877, 215880/215881, 215899/215900, 215919/215920,
-- 216027/216028, 216034/216035, 216041/216042, 216053/216054, 216055/216056, 216057/216058, 216189/216190.
DELETE FROM `creature` WHERE `guid` IN (215755, 215757, 215771, 215773, 215775, 215777, 215860, 215877, 215881,
    215900, 215920, 216028, 216035, 216042, 216054, 216056, 216058, 216190) AND `id` = 34345;
