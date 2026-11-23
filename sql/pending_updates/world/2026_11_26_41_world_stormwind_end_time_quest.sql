-- "The End Time" (30095) had no starter. Its text is spoken by Nozdormu's servant ("my master ... I can help you to
-- travel there"), and in Legends of Azeroth 5.4.8 Coridormi (52408), who already stands in the Mage Quarter and offers
-- "A Legendary Engagement" (29132), starts it. Nozdormu (54476) ends it, as before.

DELETE FROM `creature_queststarter` WHERE `quest`=30095;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(52408, 30095);
