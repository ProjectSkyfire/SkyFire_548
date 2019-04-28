-- Remove Obsolete spell_pet_auras
DELETE FROM spell_pet_auras WHERE spell IN
(19028, 19578, 35691, 35692, 35693, 56314, 56314, 23785, 23822, 23823, 23824, 23825,
 23785, 23822, 23823, 23824, 23825, 23785, 23822, 23823, 23824, 23825, 23785, 23822,
 23823, 23824, 23825, 23785, 23822, 23823, 23824, 23825, 34460);
DELETE FROM spell_pet_auras WHERE aura IN
(57452, 57484);

-- Remove Obsolete spell_scripts
DELETE FROM spell_scripts WHERE id IN
(34448, 34452);

-- Remove Obsolete player_factionchange_spells
DELETE FROM player_factionchange_spells WHERE horde_id=53736;

-- Remove Obsolete player_factionchange_achievement
DELETE FROM player_factionchange_achievement WHERE alliance_id IN
(970, 1681, 1684, 1692, 1707, 2797);
DELETE FROM player_factionchange_achievement WHERE horde_id IN
(1360, 1168, 1170, 1173, 1274, 1784, 1657, 1677, 1680, 2776, 2145, 2195, 3656);
