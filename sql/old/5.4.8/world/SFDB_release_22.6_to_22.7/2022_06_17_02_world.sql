-- Table `instance_encounters` has an invalid encounter id 445, skipped!
-- Old Scarlet Monastery - Bloodmage Thalnos
DELETE FROM instance_encounters WHERE entry=445;

-- Table `instance_encounters` has an encounter 447 (Arcanist Doan) marked as final for invalid dungeon id 165, skipped!
DELETE FROM instance_encounters WHERE entry=447;

-- Table `instance_encounters` has an encounter 793 (Hakkar) marked as final for invalid dungeon id 42, skipped!
-- Old Zul'Gurub
DELETE FROM instance_encounters WHERE entry=793;

-- Table `instance_encounters` has an invalid spell (entry 110158) linked to the encounter 1271 (Murozond), skipped!
DELETE FROM instance_encounters WHERE entry=1271;

-- Table `instance_encounters` has an invalid spell (entry 111533) linked to the encounter 1299 (Madness of Deathwing), skipped!
DELETE FROM instance_encounters WHERE entry=1299;