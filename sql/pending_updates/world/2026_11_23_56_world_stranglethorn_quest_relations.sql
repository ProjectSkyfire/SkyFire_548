-- Northern Stranglethorn and the Cape of Stranglethorn: quest givers and enders that Legends of Azeroth 5.4.8 has and the
-- data here lacked. Eliminate the Outcast (26551) had no ender at all, so it could not be handed in once Zanzil was dead.
-- The other rows let ten quests also be taken up where the quest before them ends, and let the Captain Stillwater's
-- Charts and the Barrel of Doublerum start the quests (26650, 26678) they already end. Every NPC and object has a spawn here.

DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((773, 26740), (2464, 26417), (2496, 26611), (2497, 26305), (42811, 26386), (42812, 26350), (43095, 26404), (43098, 26495), (43504, 26624), (44099, 26826));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(773, 26740),
(2464, 26417),
(2496, 26611),
(2497, 26305),
(42811, 26386),
(42812, 26350),
(43095, 26404),
(43098, 26495),
(43504, 26624),
(44099, 26826);

DELETE FROM `creature_questender` WHERE `id` = 43098 AND `quest` = 26551;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(43098, 26551);

DELETE FROM `gameobject_queststarter` WHERE (`id`, `quest`) IN ((204450, 26650), (204578, 26678));
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES
(204450, 26650),
(204578, 26678);
