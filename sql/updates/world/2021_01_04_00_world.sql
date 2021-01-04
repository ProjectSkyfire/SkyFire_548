-- Quests: Lions for Lambs
UPDATE quest_template SET
RequiredRaces=RequiredRaces|1,
OfferRewardText='Excellent work, $N. You\'ve turned out to be quite an asset to this garrison. It\'s time for you to train!',
RequestItemsText='Kill the spies!',
WDBVerified='18414' WHERE Id IN
(28774, 28770, 28759, 28772, 28771, 29079, 28769, 28773, 31140);


