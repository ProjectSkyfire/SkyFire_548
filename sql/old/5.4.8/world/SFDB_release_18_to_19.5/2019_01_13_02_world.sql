-- Disable Quest 3581: <UNUSED>
DELETE FROM disables WHERE sourceType=1 AND entry=3581;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 3581, 'Disable Quest 3581: <UNUSED>');

-- Disable Quest 1521: Call of Earth
DELETE FROM disables WHERE sourceType=1 AND entry=1521;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 1521, 'Disable Quest 1521: Call of Earth');

-- Disable Quest 6704: [DEPRECATED] <TXT> SM, RFD, ULD
DELETE FROM disables WHERE sourceType=1 AND entry=6704;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6704, 'Disable Quest 6704: [DEPRECATED] <TXT> SM, RFD, ULD');

-- Disable Quest 7363: The Human Condition
DELETE FROM disables WHERE sourceType=1 AND entry=7363;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 7363, 'Disable Quest 7363: The Human Condition');

-- Disable Quest 9382: The Fate of the Clefthoof
DELETE FROM disables WHERE sourceType=1 AND entry=9382;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9382, 'Disable Quest 9382: The Fate of the Clefthoof');

-- Disable Quest 10092: Assault on Mageddon
DELETE FROM disables WHERE sourceType=1 AND entry=10092;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10092, 'Disable Quest 10092: Assault on Mageddon');

-- Disable Quest 10217: Fossil Fuel
DELETE FROM disables WHERE sourceType=1 AND entry=10217;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10217, 'Disable Quest 10217: Fossil Fuel');
