-- Fargo Flintlocke (44806) ends "Missing Parts" (27038) and "Twilight Shores" (28238) and starts the second "Twilight
-- Shores" (28832) at Stormwind Harbor. His template carries "Fargo Flintlocke Quest Invis A" (93307), which hides him
-- from players without a matching detection aura, and no spell_area, quest or script grants it, so the three quests
-- could not be turned in or taken. In Legends of Azeroth 5.4.8 he has no aura. The Twilight Highlands copies (45168,
-- 49252) keep theirs, as in Legends of Azeroth.

UPDATE `creature_template_addon` SET `auras`=NULL WHERE `entry`=44806;
