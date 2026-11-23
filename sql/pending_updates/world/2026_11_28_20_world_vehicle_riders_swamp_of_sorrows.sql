-- Swamp of Sorrows: the Stonard Kodo Beasts (46748) carry a Stonard Wardrummer (46749) and the Marshtide Steeds (46840)
-- a Marshtide Cleric (46841) on seat 0 as their vehicle accessory in Legends of Azeroth 5.4.8, but neither had an
-- accessory row or a spellclick row, so no rider was ever seated and each stood loose above its mount. Add the
-- accessories and the Legends of Azeroth click row (46598, cast flags 1, user type 0); the mounts get no click flag
-- there either.
-- Each loose rider has the orientation of its mount, stands over the saddle (0.25 yards across and 2.61 high on a steed,
-- 2.6 across and 3.95 high on a kodo) in phase 0 and spawn mask 1. Their entries are never summoned (no summon action,
-- summon group, script or spell names them), they are not pooled, in an event, formation, addon, path or linked
-- respawn, and no smart script row sits on their guids. Legends of Azeroth lists the same loose riders (unverified,
-- VerifiedBuild 0, so weaker evidence) next to the same accessory rows. With the accessory in place the loose copy is
-- the second rider on the seat, so it goes. Wardrummer 150657, 3.2 yards from kodo 150652 with another orientation and
-- no height offset, is a separate footman and stays.

DELETE FROM `vehicle_template_accessory` WHERE `entry` IN (46748, 46840);
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(46748, 46749, 0, 1, 'Stonard Kodo Beast', 6, 30000),
(46840, 46841, 0, 1, 'Marshtide Steed', 6, 30000);

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (46748, 46840);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(46748, 46598, 1, 0),
(46840, 46598, 1, 0);

-- Kodo guid and the loose Wardrummer guid it carries (same orientation): 150652/150651, 150663/150664, 150670/150669,
-- 150685/150686, 150691/150692, 150696/150697.
DELETE FROM `creature` WHERE `guid` IN (150651, 150664, 150669, 150686, 150692, 150697) AND `id` = 46749;

-- Steed guid and the loose Cleric guid it carries (same orientation): 150515/150514, 150547/150546, 150612/150611,
-- 150629/150630, 150656/150655.
DELETE FROM `creature` WHERE `guid` IN (150514, 150546, 150611, 150630, 150655) AND `id` = 46841;
