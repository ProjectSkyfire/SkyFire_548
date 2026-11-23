-- Furlbrow's Deed (184): the turn-in NPCs are Lieutenant Horatio Laine and the Stormwind investigators, not the Furlbrows, but
-- the quest still showed the text of Farmer Furlbrow ("Loads of thanks", "That's good news! Some ruffians stole it days ago",
-- upstream issue 1419). The reward text is the one of the Cataclysm Preservation Project 4.3.4 and the progress text the
-- one of Legends of Azeroth 5.4.8.

UPDATE `quest_template` SET `OfferRewardText` = 'This is useless. Look at the date on this letter. The Furlbrows have been squatting on the Jansen Steed for five years.$B$BThey never could quite get their wagon... fixed.', `RequestItemsText` = 'We don''t need any civilian vigilante types getting involved, kid. Leave this one to the professionals.' WHERE `Id` = 184;
