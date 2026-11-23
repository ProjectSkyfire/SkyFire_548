-- Thousand Needles: the I-Scream Cryocannon backpack (spell 88454, a hidden dummy aura with no duration) is applied
-- while a Cryocannon (items 62912 and 62913) is carried, but it is saved with the character and nothing ties it to the
-- quests that hand the item out. Limit it to the time one of the three quests is in the quest log: "Special Delivery
-- for Brivelthwerp" (28031 Alliance, 28042 Horde) and "Freezing the Pipes" (28047). It is removed when the quest is
-- turned in, failed or abandoned, and a stale copy is dropped at the next login or area change.
-- quest_start_status 10 = quest complete (1) or incomplete (3).
DELETE FROM `spell_area` WHERE `spell`=88454 AND `area`=0 AND `quest_start` IN (28031, 28042, 28047);
INSERT INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES
(88454, 0, 28031, 0, 0, 18875469, 2, 0, 10, 11),
(88454, 0, 28042, 0, 0, 33555378, 2, 0, 10, 11),
(88454, 0, 28047, 0, 0, 0, 2, 0, 10, 11);
