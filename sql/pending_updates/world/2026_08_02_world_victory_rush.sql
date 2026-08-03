-- Victory Rush (34428): heal 20% max HP via 118779 and consume Victorious (32216).
-- Damage is 56% of Attack Power (MoP DBC / EffectBonusMultiplier 0.56).
DELETE FROM `spell_script_names` WHERE `spell_id` = 34428 OR `ScriptName` IN ('spell_warr_victory_rush', 'spell_warr_victorious');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(34428, 'spell_warr_victory_rush');

DELETE FROM `spell_bonus_data` WHERE `entry` = 34428;
INSERT INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES
(34428, -1, -1, 0.56, -1, 'Warrior - Victory Rush');
