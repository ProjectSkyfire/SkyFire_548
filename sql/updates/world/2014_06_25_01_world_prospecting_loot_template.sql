SET @TinRef := 13005;
SET @CATA1 := @TinRef+1;
SET @CATA2 := @TinRef+2;
SET @MIST1 := @TinRef+3;
SET @MIST2 := @TinRef+4;

-- CATA
SET @Obsidium := 53038;
SET @Elementium := 52185;
SET @Pyrite := 52183;
-- MIST
SET @Ghost := 72092;
SET @Kyparite := 72093;
SET @BTrillium := 72094;
SET @WTrillium := 72103;

-- Reference Loot Templates
DELETE FROM `reference_loot_template` WHERE `entry` BETWEEN @TinRef+1 AND @TinRef+4;
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
-- CATA Greens
(@CATA1,52177,0,1,1,1,1), -- Carnelian
(@CATA1,52178,0,1,1,1,1), -- Zephyrite
(@CATA1,52179,0,1,1,1,1), -- Alicite
(@CATA1,52180,0,1,1,1,1), -- Nightstone
(@CATA1,52181,0,1,1,1,1), -- Hessonite
(@CATA1,52182,0,1,1,1,1), -- Jasper
-- CATA Blues
(@CATA2,52190,0,1,1,1,1), -- Inferno Ruby
(@CATA2,52191,0,1,1,1,1), -- Ocean Sapphire
(@CATA2,52192,0,1,1,1,1), -- Dream Emerald
(@CATA2,52193,0,1,1,1,1), -- Ember Topaz
(@CATA2,52194,0,1,1,1,1), -- Demonseye
(@CATA2,52195,0,1,1,1,1), -- Amberjewel
-- MIST Greens
(@MIST1,76130,0,1,1,1,1), -- Tiger Opal
(@MIST1,76133,0,1,1,1,1), -- Lapis Lazuli
(@MIST1,76134,0,1,1,1,1), -- Sunstone
(@MIST1,76135,0,1,1,1,1), -- Roguestone
(@MIST1,76136,0,1,1,1,1), -- Pandarian Garnet
(@MIST1,76137,0,1,1,1,1), -- Alexandrite
-- MIST Blues
(@MIST2,76131,0,1,1,1,1), -- Primordial Ruby
(@MIST2,76138,0,1,1,1,1), -- River's Hearth
(@MIST2,76139,0,1,1,1,1), -- Wild Jade
(@MIST2,76140,0,1,1,1,1), -- Vermilion Onyx
(@MIST2,76141,0,1,1,1,1), -- Imperial Amethyst
(@MIST2,76142,0,1,1,1,1); -- Sun's Radiance

-- --------------------------------------------------------
-- Prospecting Loot Templates
DELETE FROM `prospecting_loot_template` WHERE `entry` IN (@Obsidium,@Elementium,@Pyrite,@Ghost,@Kyparite,@BTrillium,@WTrillium);
INSERT INTO `prospecting_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
-- Obsidium Ore
(@Obsidium,1,95,1,1,-@CATA1,1), -- One from CATA Greens
(@Obsidium,2,05,1,1,-@CATA2,1), -- One from CATA Blues
-- Elementium Ore
(@Elementium,1,85,1,1,-@CATA1,2), -- Two from CATA Greens
(@Elementium,2,15,1,1,-@CATA2,1), -- One from CATA Blues
-- Pyrite Ore
(@Pyrite,52327,100,1,0,1,3), -- Volatile Earth
(@Pyrite,1,75,1,1,-@CATA1,2), -- Two from CATA Greens
(@Pyrite,2,25,1,1,-@CATA2,1), -- Two from CATA Blues
-- Ghost Iron Ore
(@Ghost,90407,80,1,0,1,2), -- Sparkling Shard
(@Ghost,1,95,1,1,-@MIST1,1), -- One from MIST Greens
(@Ghost,2,05,1,1,-@MIST2,1), -- One from MIST Blues
-- Kyparit
(@Kyparite,90407,100,1,0,1,2), -- Sparkling Shard
(@Kyparite,1,95,1,1,-@MIST1,1), -- One from MIST Greens
(@Kyparite,2,05,1,1,-@MIST2,1), -- One from MIST Blues
-- Black Trillium Ore
(@BTrillium,90407,100,1,0,1,2), -- Sparkling Shard
(@BTrillium,1,75,1,1,-@MIST1,2), -- Two from MIST Greens
(@BTrillium,2,25,1,1,-@MIST2,1), -- One from MIST Blues
-- White Trillium Ore
(@WTrillium,90407,100,1,0,1,2), -- Sparkling Shard
(@WTrillium,1,75,1,1,-@MIST1,2), -- Two from MIST Greens
(@WTrillium,2,25,1,1,-@MIST2,1); -- One from MIST Blues

