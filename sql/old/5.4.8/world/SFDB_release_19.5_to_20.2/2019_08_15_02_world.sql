-- Delete Obsolete instance_encounter's in Ragefire Chasm
-- 430 -- Oggleflint
-- 431 -- Taragaman the Hungerer
-- 432 -- Jergosh the Invoker
-- 433 -- Bazzalan
DELETE FROM instance_encounters WHERE entry IN
(430, 431, 432, 433);
