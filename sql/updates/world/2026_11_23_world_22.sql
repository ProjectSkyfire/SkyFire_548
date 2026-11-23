-- Remove the misplaced, permanently spawned Holly Preserver in Stormwind.
-- https://www.wowhead.com/mop-classic/object=180715/holly-preserver
DELETE FROM `gameobject`
WHERE `guid` = 84991 AND `id` = 180715 AND `map` = 0;
