-- Outland (TBC) dungeons: register the in-instance graveyard for each dungeon so
-- death releases the player INSIDE the instance (MoP 5.4.8 behaviour) instead of
-- at an outdoor graveyard requiring a ghost re-entry.
--
-- Each WorldSafeLocs id below already exists in WorldSafeLocs.dbc on the dungeon's
-- own map; only the game_graveyard_zone link to the dungeon's ghost zone was
-- missing, so ObjectMgr::GetClosestGraveyard had no in-instance candidate and fell
-- back outside. Ghost zones are the top-level AreaTable area for each dungeon map.
-- Fixes #1522.
--
-- map  dungeon            ghost_zone  worldsafeloc
-- 540  Shattered Halls    3714        3718
-- 542  Blood Furnace      3713        3719
-- 543  Hellfire Ramparts  3562        3717
-- 545  Steamvault         3715        3739
-- 546  Underbog           3716        3738
-- 547  Slave Pens         3717        3740
-- 552  The Mechanar       3848        3766
-- 553  The Botanica       3847        3765
-- 554  The Arcatraz       3849        3767
-- 555  Shadow Labyrinth   3789        3752
-- 556  Sethekk Halls      3791        3751
-- 557  Mana-Tombs         3792        3750
-- 558  Auchenai Crypts    3790        3749
DELETE FROM `game_graveyard_zone` WHERE `id` IN (3717,3718,3719,3738,3739,3740,3749,3750,3751,3752,3765,3766,3767);
INSERT INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`) VALUES
(3718, 3714, 0),
(3719, 3713, 0),
(3717, 3562, 0),
(3739, 3715, 0),
(3738, 3716, 0),
(3740, 3717, 0),
(3766, 3848, 0),
(3765, 3847, 0),
(3767, 3849, 0),
(3752, 3789, 0),
(3751, 3791, 0),
(3750, 3792, 0),
(3749, 3790, 0);
