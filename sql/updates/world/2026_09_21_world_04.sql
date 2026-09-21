-- Quest 10078 "Laying Waste to the Unwanted": let the Flaming Torch reach a bunny inside
-- its tower.
--
-- The four Invis Horde Siege Engine bunnies sit inside the tower structures, 13 to 18 yards
-- above the spot the torch is thrown from. Spell 32979's implicit target is
-- TARGET_UNIT_NEARBY_ENTRY, and Spell::CheckEffectTarget falls through to a plain
-- IsWithinLOSInMap for that case, so the trace from outside the tower to a target inside it
-- fails and the spell finds nothing. 32979's AttributesEx2 is 0, so the
-- CAN_TARGET_NOT_IN_LOS attribute is genuinely absent and the core is enforcing a real
-- requirement.
--
-- `disables` sourceType 0 (SPELL) flag 64 (SPELL_DISABLE_LOS) is the existing escape hatch for
-- exactly this; 26.002 already has five rows of the same shape (16378, 31696, 32205, 45949,
-- 53038). The branch that reads it is only reachable with no unit context, so the spell is not
-- disabled for players or creatures.

DELETE FROM `disables` WHERE `sourceType` = 0 AND `entry` = 32979;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(0, 32979, 64, '', '', 'Ignore LOS for Flaming Torch - quest 10078 bunnies sit inside the towers');
