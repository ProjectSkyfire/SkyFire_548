-- The Black Morass: the entrance requires the quest "Return to Andormu" (10285, access_requirement) and
-- "The Black Morass" (10296) follows it, but nobody started or ended 10285, so neither the quest chain nor the
-- instance could be reached: SkyFire had moved the Old Hillsbrad part of the chain to 29598 and 29599 but left
-- 10285 behind the old quest 10284. Legends of Azeroth 5.4.8 has the chain: 29599 Escape from Durnholde, then
-- 10285 (Erozion gives it, Andormu takes it), 10296, 10297, and 10298 Hero of the Brood (Sa'at gives it,
-- Andormu takes it). TrinityCore 3.3.5 has the same NPCs for 10285 and 10298.
UPDATE `quest_template` SET `PrevQuestId` = 29599 WHERE `Id` = 10285 AND `PrevQuestId` = 10284;

DELETE FROM `creature_queststarter` WHERE (`id` = 18723 AND `quest` = 10285) OR (`id` = 20201 AND `quest` = 10298);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(18723, 10285),
(20201, 10298);

DELETE FROM `creature_questender` WHERE `id` = 20130 AND `quest` IN (10285, 10298);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(20130, 10285),
(20130, 10298);
