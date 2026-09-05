-- Monk melee abilities: Jab, Tiger Palm, Blackout Kick and Rising Sun Kick.
--
-- Their DBC damage effect only carries a placeholder (1, or 50 for Tiger Palm). MoP
-- keeps the real coefficient in the tooltip text -- "[1.5 * <low>]", "[3 * <low>]",
-- "[7.12 * <low>]", "[14.4 * <low>]" -- and builds the number out of weapon damage per
-- second plus attack power. The scripts below fill that in.
--
-- Jab is listed six times because the weapon override auras swap in a copy of it per
-- weapon type; the same six ids already carry spell_monk_power_strikes.

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_monk_jab', 'spell_monk_tiger_palm', 'spell_monk_rising_sun_kick');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(100780, 'spell_monk_jab'),          -- fists / no weapon
(108557, 'spell_monk_jab'),          -- staff
(115687, 'spell_monk_jab'),          -- axe
(115693, 'spell_monk_jab'),          -- mace
(115695, 'spell_monk_jab'),          -- sword
(115698, 'spell_monk_jab'),          -- polearm
(100787, 'spell_monk_tiger_palm'),
(107428, 'spell_monk_rising_sun_kick');

-- Pin the spell power coefficient at 0 on all of them: the scripts already carry the
-- attack power share, and without a row here a mana-using monk would add intellect
-- based spell power on top of a physical strike through the default coefficient.
DELETE FROM `spell_bonus_data` WHERE `entry` IN (100780, 100784, 100787, 107428, 108557, 115687, 115693, 115695, 115698);
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(100780, 0, 0, 0, 0, 'Monk - Jab (weapon damage handled by spell_monk_jab)'),
(108557, 0, 0, 0, 0, 'Monk - Jab, staff (weapon damage handled by spell_monk_jab)'),
(115687, 0, 0, 0, 0, 'Monk - Jab, axe (weapon damage handled by spell_monk_jab)'),
(115693, 0, 0, 0, 0, 'Monk - Jab, mace (weapon damage handled by spell_monk_jab)'),
(115695, 0, 0, 0, 0, 'Monk - Jab, sword (weapon damage handled by spell_monk_jab)'),
(115698, 0, 0, 0, 0, 'Monk - Jab, polearm (weapon damage handled by spell_monk_jab)'),
(100787, 0, 0, 0, 0, 'Monk - Tiger Palm (weapon damage handled by spell_monk_tiger_palm)'),
(100784, 0, 0, 0, 0, 'Monk - Blackout Kick (weapon damage handled by spell_monk_blackout_kick)'),
(107428, 0, 0, 0, 0, 'Monk - Rising Sun Kick (weapon damage handled by spell_monk_rising_sun_kick)');
