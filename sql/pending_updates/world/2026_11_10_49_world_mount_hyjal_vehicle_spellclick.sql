-- Mount Hyjal: boarding for the vehicle accessories. A creature that is an accessory of a vehicle gets into its seat through
-- the vehicle's spellclick row (spell 46598, Ride Vehicle Hardcoded, cast by the accessory); the chain vehicle of Kristoff
-- Manheim and the buzzard and firebird of the Hatchery flight had none, so Kristoff and the riders were summoned and sent
-- away again.
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (39789, 39833, 40650) AND `spell_id` = 46598;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(39789, 46598, 1, 0),
(39833, 46598, 1, 0),
(40650, 46598, 1, 0);
