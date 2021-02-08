-- Quests: Beating Them Back!
UPDATE quest_template SET
RequiredRaces=RequiredRaces|1,
OfferRewardText='You\'ve bought us a little time, $N, but we\'ve got even bigger problems to deal with now.',
RequestItemsText='Have you beaten back the Blackrock battle worgs?',
WDBVerified='18414' WHERE Id IN
(28766, 28762, 28767, 28764, 28763, 29078, 28757, 28765, 31139);
