-- Mount Hyjal: the Alliance and Horde versions of the Shrine of Goldrinn quests could be taken by both sides
-- quest_template: RequiredRaces as in the 5.4.8 data (Alliance races and Alliance Pandaren 18875469, Horde races and
-- Horde Pandaren 33555378)
UPDATE `quest_template` SET `RequiredRaces`=18875469 WHERE `Id` IN (25268,25271,25273,25278,25280,25352,25353,25611,25618,25623);
UPDATE `quest_template` SET `RequiredRaces`=33555378 WHERE `Id` IN (25269,25270,25272,25277,25279,25354,25355,25612,25617,25624);
