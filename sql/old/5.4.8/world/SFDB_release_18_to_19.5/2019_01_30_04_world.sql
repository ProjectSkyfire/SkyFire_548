-- Added missing page_text for The Emperor's Burden - Part 8
DELETE FROM page_text WHERE entry=4546;
INSERT INTO page_text (entry, text, next_page, WDBVerified) VALUES
(4546, 'Ten thousand years ago, Emperor Shaohao, the Last Emperor of Pandaria, used the power of these sacred waters to spare Pandaria from the devastation of the Sundering that destroyed the rest of the world.', 0, 1);
