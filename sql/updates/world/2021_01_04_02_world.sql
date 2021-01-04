-- Quests: Join the Battle!
UPDATE quest_template SET
RequiredRaces=RequiredRaces|1,
Flags=Flags|65536|262144|524288|134217728,
OfferRewardText='It\'s true, we were ambushed. I don\'t dare send any more soldiers out there and risk losing them too. I need a volunteer. Someone willing to risk their life!',
WDBVerified='18414' WHERE Id IN
(28789, 28785, 28780, 28787, 28786, 29080, 28784, 28788, 31143);

DELETE FROM creature_queststarter  WHERE quest IN
(28789, 28785, 28780, 28787, 28786, 29080, 28784, 28788, 31143);

INSERT INTO creature_queststarter (id, quest) VALUES 
(197, 28780),
(197, 28784),
(197, 28785),
(197, 28786),
(197, 28787),
(197, 28788),
(197, 28789),
(197, 29080),
(197, 31143),
(198, 28784),
(375, 28786),
(459, 28788),
(911, 28789),
(915, 28787),
(925, 28785),
(43278, 28780),
(63258, 31143);
