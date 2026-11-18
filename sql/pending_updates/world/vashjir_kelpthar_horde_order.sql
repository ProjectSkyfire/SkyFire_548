-- Kelp'thar Forest, Horde (Legionnaire Nazgrim at The Immortal Coil): Pay Attention! (27668) is offered after Rest For
-- the Weary (25941), whose objective is to speak to Nazgrim, and Helm's Deep (25946) is the step after it: the broken
-- Alliance crate (203395) ends Pay Attention! and starts Helm's Deep, so Nazgrim does not offer it as well.
UPDATE `quest_template` SET `PrevQuestId` = 25941 WHERE `Id` = 27668;
UPDATE `quest_template` SET `PrevQuestId` = 27668 WHERE `Id` = 25946;
DELETE FROM `creature_queststarter` WHERE `id` = 41711 AND `quest` = 25946;
