-- Class mask 1503 means "every class except death knight" and predates Mists, so it excludes
-- monks as well.
--
-- ChrClasses.dbc in 5.4.8 has eleven rows, ids 1-11, so the playable mask is 2047:
--
--   1 Warrior 1, 2 Paladin 2, 3 Hunter 4, 4 Rogue 8, 5 Priest 16, 6 Death Knight 32,
--   7 Shaman 64, 8 Mage 128, 9 Warlock 256, 10 Monk 512, 11 Druid 1024
--
--   all playable           2047
--   all but death knight   2047 - 32  = 2015
--   the stored value       1503       = 2015 - 512 (Monk)
--
-- 1503 is 1535 - 32, and 1535 was the playable mask before monks existed.
--
-- The intent is not in doubt: nine of the ten quests below have a death-knight-only twin at mask
-- 32 with the same title and the same RequiredRaces (13104/13105, 13682+13809/13788+13812,
-- 13789+13810/13791+13813, 13790+13811/13793+13814, 13861+13862/13863+13864), and 13104's own
-- two conditions rows are commented "Show quest 13104 if player is not a Death Knight" and
-- "Show quest 13105 if player is a Death Knight". Monks have no such twin; they are simply left
-- out of the Argent Tournament dailies and of Hellfire Fortifications.

UPDATE `conditions`
   SET `ConditionValue1` = 2015
 WHERE `ConditionTypeOrReference` = 15
   AND `ConditionValue1` = 1503
   AND `SourceTypeOrReferenceId` IN (19, 20)
   AND `SourceEntry` IN (13104, 13408, 13409);

UPDATE `quest_template`
   SET `RequiredClasses` = 2015
 WHERE `RequiredClasses` = 1503
   AND `Id` IN (13104, 13682, 13789, 13790, 13794, 13809, 13810, 13811, 13861, 13862);
