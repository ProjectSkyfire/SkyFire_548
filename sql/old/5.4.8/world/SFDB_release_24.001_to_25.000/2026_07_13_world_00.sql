-- Fix skull/"??" portrait and missing aggro on tameable quest beasts.
-- type_flags 2147483649 = CREATURE_TYPEFLAGS_TAMEABLE (1) | CREATURE_TYPEFLAGS_UNK31 (0x80000000)
-- The QUEST_BOSS flag forces a skull portrait regardless of actual level.
-- Keep TAMEABLE (1); set dynamicflags 4 (UNIT_DYNFLAG_TRACK_UNIT) to match other STV rares.

UPDATE `creature_template`
SET `type_flags` = 1,
    `dynamicflags` = 4
WHERE `entry` IN (
    728,   -- Bhag'thera
    731,   -- King Bangalash
    1225,  -- Ol' Sooty
    1961,  -- Mangeclaw
    1994,  -- Githyiss the Vile
    3281,  -- Sarkoth
    7319,  -- Lady Sathrah
    7977,  -- Gammerita
    9622,  -- U'cha
    10737, -- Shy-Rotam
    10806, -- Ursius
    14661, -- Stinglasher
    18257, -- Gutripper
    18707, -- Torgos
    21515, -- Trachela
    23873, -- Goreclaw the Ravenous
    26838, -- Dreadtalon
    39385, -- Screamslash
    40581, -- Gargantapid
    41420, -- Horrorjaw
    45450, -- The Lone Hunter
    47053, -- Duskfang
    47687, -- Winna's Kitten
    50044, -- Hell-Hoot
    56357, -- Lupello
    57421, -- Mothran
    59037, -- Kung Din
    60203, -- Nahassa
    60401, -- Snapclaw
    63767, -- Chasheen
    65612  -- Windward Saber
);
