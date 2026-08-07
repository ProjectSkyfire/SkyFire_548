-- Langridge Shot
UPDATE `quest_template` SET `RequestItemsText`='Paxton has already loaded a charge into the cannon. All it needs is the ammunition you improvised.' WHERE `Id`=24941;
UPDATE `quest_template` SET `OfferRewardText`='The Horde is massing at the base of the hill. Breathlessly, you stuff the debris you collected into the muzzle of the cannon, step back, cover one ear, and tug on the igniter cord...' WHERE `Id`=24941;

-- Back in the Fight
UPDATE `quest_template` SET `PrevQuestId`=24941 WHERE `Id`=24944;
UPDATE `quest_template` SET `OfferRewardText`='A steady stream of the walking wounded have come up the hill to see me, $n. I trust that was your work? Well done!' WHERE `Id`=24944;

-- Re-Take the Courtyard
UPDATE `quest_template` SET `PrevQuestId`=24941 WHERE `Id`=24943;
UPDATE `quest_template` SET `OfferRewardText`='Great work! We\'ve knocked them on their heels, maybe long enough to shore up our defenses.$B$BProvided we find a way to take out their artillery...' WHERE `Id`=24943;
