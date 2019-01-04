-- Disable Quest: 9412 - <UNUSED>Crystal Flake Throat Lozenge
DELETE FROM disables WHERE sourceType=1 AND entry=9412;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9412, 'Disable Quest 9412: <UNUSED>Crystal Flake Throat Lozenge');
