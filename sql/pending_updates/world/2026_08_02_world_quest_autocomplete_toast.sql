-- Tuning the Gnomecorder (26512): Lakeshire Graveyard explore credit
-- AreaTrigger 6034 from retail sniff; needs exploration special flag + involvedrelation.
UPDATE quest_template SET SpecialFlags = SpecialFlags | 2 WHERE Id = 26512;
UPDATE quest_objective SET Amount = 1 WHERE QuestId = 26512 AND Type = 10 AND Amount = 0;
DELETE FROM areatrigger_involvedrelation WHERE id = 6034 AND quest = 26512;
INSERT INTO areatrigger_involvedrelation (id, quest) VALUES (6034, 26512);
