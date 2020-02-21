-- The Sunwell
-- Gruul's Lair
-- Black Temple
-- Tempest Keep
-- Serpentshrine Cavern
-- Magtheridon's Lair
-- Hyjal Past
UPDATE creature SET spawnMask=16 WHERE map IN (580,565,564,550,548,544,534);
UPDATE gameobject SET spawnMask=16 WHERE map IN (580,565,564,550,548,544,534);
