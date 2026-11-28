-- Vermillion Redoubt quest turn-in text and progression.
UPDATE `quest_template` SET `OfferRewardText`='I''m glad Lira didn''t scare you off. I won''t say her concerns aren''t warranted, but we have a common enemy. She is perhaps a bit overzealous.' WHERE `Id`=27485;
UPDATE `quest_template` SET `OfferRewardText`='I''m glad Lira didn''t scare you off. I won''t say her concerns aren''t warranted, but we have a common enemy. She is perhaps a bit overzealous.' WHERE `Id`=27486;
UPDATE `quest_template` SET `OfferRewardText`='Calen has become very trusting. It is not often that strangers are permitted within the redoubt.' WHERE `Id`=27504;
UPDATE `quest_template` SET `OfferRewardText`='We will take Grim Batol back from the Twilight''s Hammer and their allies. It may take time, but this is one charge that we cannot forsake.' WHERE `Id`=27505;
UPDATE `quest_template` SET `OfferRewardText`='I hope you''re as tough as your gear suggests. The Twilight''s Hammer have made their way within the redoubt somehow and I need someone to hunt them down.' WHERE `Id`=27564;

-- Unlock the redoubt follow-up quests after Calen accepts the report.
UPDATE `quest_template` SET `PrevQuestId`=27564 WHERE `Id` IN (27507,27508,27509);

-- Faction-specific introductions to the next quest hub follow Breach in the Defenses.
UPDATE `quest_template` SET `PrevQuestId`=27509, `RequiredRaces`=33555378 WHERE `Id`=27576;
UPDATE `quest_template` SET `PrevQuestId`=27509, `RequiredRaces`=18875469 WHERE `Id`=28101;
