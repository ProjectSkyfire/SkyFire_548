-- After the Crusade (26930, scout the Scarlet Crusade camp east of Felstone Field) and Unusual Behavior... Even For
-- Gnolls (27152, investigate the gnoll hideout on the trail to Hearthglen) each have an explore objective, but no
-- area trigger was linked to the quest, so the objective was never completed. The triggers are 6213 and 6238, both
-- in AreaTrigger.dbc inside the described places. The quests get the explore special flag the core would otherwise
-- add at load.
-- Relations are as in Legends of Azeroth 5.4.8.

DELETE FROM `areatrigger_involvedrelation` WHERE `id` IN (6213, 6238);
INSERT INTO `areatrigger_involvedrelation` (`id`, `quest`) VALUES
(6213, 26930),
(6238, 27152);

UPDATE `quest_template` SET `SpecialFlags` = `SpecialFlags` | 2 WHERE `Id` IN (26930, 27152);
