-- Quests with a questgiver and nobody to hand them in to.
--
-- 153 quests in this database have a `creature_queststarter` or `gameobject_queststarter` row
-- and no row in `creature_questender` or `gameobject_questender`. With nothing registered the
-- client draws no turn-in marker and the intended NPC offers nothing, which reads in game as an
-- invisible or mis-phased questgiver.
--
-- This update covers the 20 of them whose ender can be named from data already in this
-- database, is spawned, and is not contradicted by the quest's own turn-in POI. The evidence
-- for each row is one or more of:
--
--   giver         the quest's registered questgiver is the ender (a return-to-me quest)
--   turn-in name  quest_template.QuestTurnTargetName is the ender's name
--   POI           the quest's own turn-in POI (quest_poi.objIndex = -1) is on the ender's spawn
--
-- Rows deliberately left out: any ender with no `creature_template`/`gameobject_template` row,
-- any ender with no spawn (the link would be correct and still do nothing), and five quests -
-- 26154, 26551, 27707, 27934, 31771 - whose turn-in POI is 346 to 1249 yards from the giver, so
-- "the giver takes it back" is not safe to assume for them.
--
-- Additions to two link tables only. The DELETE in front of each INSERT is there to make a
-- re-run a no-op.

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN
 ((35086,14146),(37106,24502),(44084,26810),(44799,27040),(46071,27768),(18481,29674),
  (18481,29675),(55438,29745),(56327,29924),(58710,30478),(29611,30987),(39605,31012),
  (16802,32329));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(35086, 14146), -- Defend the Gates!            -> Labor Captain Grabbit   (giver, turn-in POI 47 yd)
(37106, 24502), -- Necessary Roughness          -> Coach Crosscheck        (giver, turn-in POI 0 yd)
(44084, 26810), -- Eliminate the Outcast        -> Maywiki                 (giver)
(44799, 27040), -- Decryption Made Easy         -> Explorer Mowi           (giver)
(46071, 27768), -- Step Two: The Bloodletter    -> Lord Itharius           (giver, QuestTurnTargetName)
(18481, 29674), -- Unbound Darkness             -> A'dal                   (QuestTurnTargetName)
(18481, 29675), -- Hey There Dalliah            -> A'dal                   (QuestTurnTargetName)
(55438, 29745), -- The Sprites' Plight          -> Outcast Sprite          (giver)
(56327, 29924), -- Kill Kher Shan               -> Nibi Nibi               (giver)
(58710, 30478), -- A Gift For Jogu              -> Jogu the Drunk          (turn-in POI 0.4 yd)
(29611, 30987), -- Joining the Alliance         -> King Varian Wrynn       (turn-in POI 0.2 yd)
(39605, 31012), -- Joining the Horde            -> Garrosh Hellscream      (turn-in POI 12 yd)
(16802, 32329); -- Get My Results!              -> Lor'themar Theron       (turn-in POI 0.7 yd)

DELETE FROM `gameobject_questender` WHERE (`id`, `quest`) IN
 ((206335,27693),(206336,27709),(206335,27885),(206336,27886),(209845,29888),(213767,31314),
  (215844,31779));
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES
(206335, 27693), -- The Warden's Game           -> Stone Slab              (giver)
(206336, 27709), -- The Sentinel's Game         -> Marble Slab             (giver)
(206335, 27885), -- The Warden's Game           -> Stone Slab              (giver)
(206336, 27886), -- The Sentinel's Game         -> Marble Slab             (giver)
(209845, 29888), -- Seek Out the Lorewalker     -> Mouthwatering Brew      (turn-in POI 9.9 yd)
(213767, 31314), -- Old Man Thistle's Treasure  -> Hidden Treasure         (turn-in POI 0.5 yd)
(215844, 31779); -- The Darkness Within         -> Flagpole                (turn-in POI 0.5 yd)
