-- Randal Hunter accepts the riding pamphlet quest received through the mail.
-- The quest starts from its item; do not add it to his quest starters.
DELETE FROM creature_questender WHERE id = 4732 AND quest = 14079;
INSERT INTO creature_questender (id, quest) VALUES (4732, 14079);
