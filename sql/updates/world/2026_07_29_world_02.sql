-- The Future of Gnomeregan
DELETE FROM `creature_queststarter` WHERE `id`=42396 AND `quest`=31135;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(42396, 31135);

-- Meet the High Tinker
UPDATE `quest_template` SET `OfferRewardText`='It\'s good to meet you, $n. You\'ve probably heard others speaking of Operation: Gnomeregan. Let me tell you a bit about what happened and why we left the dwarven city of Ironforge.' WHERE `Id`=31137;

DELETE FROM `creature_queststarter` WHERE `id`=63238 AND `quest`=31135;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(63238, 31137);

DELETE FROM `creature_questender` WHERE `id`=42317 AND `quest`=31137;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(42317, 31137);
