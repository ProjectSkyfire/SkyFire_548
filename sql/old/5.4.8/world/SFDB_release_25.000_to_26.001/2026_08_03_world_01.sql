-- Heroic Leap (6544): cast landing damage 52174 when the jump spline finishes.
-- Damage is 50% of Attack Power (MoP DBC / Field08 0.5).
DELETE FROM `spell_bonus_data` WHERE `entry` = 52174;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(52174, -1, -1, 0.5, -1, 'Warrior - Heroic Leap');
