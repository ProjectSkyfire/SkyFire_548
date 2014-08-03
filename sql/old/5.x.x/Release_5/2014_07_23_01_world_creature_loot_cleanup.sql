-- Loot Template Cleanup. Removed obsolete loot entries. 
-- Kobold Vermin, Harvest Golem , Two-Bit Thug, Diseased Timber Wolf, Kobold Laborer
-- Defias Smuggler, Garrick Padfoot, Defias Pathstalker, Defias Highwayman, Riverpaw Overseer
-- Blue Dragonspawn, Starving Dire Wolf, Defias Night Runner, Farmer Ray, Kobold Worker
-- Eliza <Bride of the Embalmer>, Singe, Timber Wolf, Winter Wolf, Porcine Entourage, Stitches <Gift from the Embalmer>

DELETE FROM creature_loot_template WHERE entry IN
(6, 36, 38, 69, 80, 95, 103, 121, 122, 125, 193, 213, 215, 232, 257, 314, 335, 358, 359, 390, 412);

DELETE FROM creature WHERE id IN 
(6, 36, 38, 69, 80, 95, 103, 121, 122, 125, 193, 213, 215, 232, 257, 314, 335, 358, 359, 390, 412);
