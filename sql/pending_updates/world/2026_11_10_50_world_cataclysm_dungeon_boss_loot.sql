-- Boss loot of the Halls of Origination, the Vortex Pinnacle and the Lost City of the Tol'vir. The tables held
-- every normal and heroic item as an independent roll of about 10%, so about a third of the kills dropped
-- nothing and normal kills could drop heroic items; the Vortex Pinnacle bosses had no table at all. Normal and
-- heroic items now sit in their own tables with one guaranteed item from each group, as in Legends of Azeroth
-- 5.4.8. The heroic tables belong to the heroic templates. Based on Legends of Azeroth 5.4.8.

DELETE FROM `creature_loot_template` WHERE `entry` IN (
    39378, 39425, 39428, 39587, 39731, 39732, 39788, 43612, 43614, 43873, 43874, 43875, 43876, 43878, 43879, 44577,
    44819, 48710, 48714, 48776, 48815, 48902, 48932, 48951, 49045, 49262
);
DELETE FROM `reference_loot_template` WHERE `entry` IN (393780, 436120, 448190);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39378, 56098, 0, 1, 2, 1, 1),
(39378, 56099, 0, 1, 2, 1, 1),
(39378, 56100, 0, 1, 1, 1, 1),
(39378, 56101, 0, 1, 1, 1, 1),
(39378, 56102, 0, 1, 1, 1, 1),
(39378, 56104, 0, 1, 2, 1, 1),
(39378, 56105, 0, 1, 2, 1, 1),
(39378, 56106, 0, 1, 1, 1, 1),
(39378, 56107, 0, 1, 2, 1, 1),
(39378, 56108, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39425, 55886, 0, 1, 1, 1, 1),
(39425, 55887, 0, 1, 1, 1, 1),
(39425, 55888, 0, 1, 1, 1, 1),
(39425, 55889, 0, 1, 1, 1, 1),
(39425, 55890, 0, 1, 1, 1, 1),
(39425, 71637, 0.4043, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39428, 56093, 0, 1, 1, 1, 1),
(39428, 56094, 0, 1, 1, 1, 1),
(39428, 56095, 0, 1, 1, 1, 1),
(39428, 56096, 0, 1, 1, 1, 1),
(39428, 56097, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39587, 55992, 0, 1, 1, 1, 1),
(39587, 55993, 0, 1, 1, 1, 1),
(39587, 55994, 0, 1, 1, 1, 1),
(39587, 55995, 0, 1, 1, 1, 1),
(39587, 55996, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39731, 55997, 0, 1, 1, 1, 1),
(39731, 55998, 0, 1, 1, 1, 1),
(39731, 55999, 0, 1, 1, 1, 1),
(39731, 56000, 0, 1, 1, 1, 1),
(39731, 56001, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39732, 57861, 0, 1, 1, 1, 1),
(39732, 57862, 0, 1, 1, 1, 1),
(39732, 57863, 0, 1, 1, 1, 1),
(39732, 57864, 0, 1, 1, 1, 1),
(39732, 57865, 0, 1, 1, 1, 1),
(39732, 71635, 0.522, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(39788, 57855, 0, 1, 1, 1, 1),
(39788, 57856, 0, 1, 1, 1, 1),
(39788, 57857, 0, 1, 1, 1, 1),
(39788, 57858, 0, 1, 1, 1, 1),
(39788, 57860, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43612, 55861, 0, 1, 1, 1, 1),
(43612, 55862, 0, 1, 1, 1, 1),
(43612, 55863, 0, 1, 1, 1, 1),
(43612, 55864, 0, 1, 1, 1, 1),
(43612, 55865, 0, 1, 1, 1, 1),
(43612, 71715, 0.3485, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43614, 55866, 0, 1, 1, 1, 1),
(43614, 55867, 0, 1, 1, 1, 1),
(43614, 55868, 0, 1, 1, 1, 1),
(43614, 55869, 0, 1, 1, 1, 1),
(43614, 55870, 0, 1, 1, 1, 1),
(43614, 71636, 1.1335, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43873, 55835, 0, 1, 1, 1, 1),
(43873, 55838, 0, 1, 1, 1, 1),
(43873, 55839, 0, 1, 1, 1, 1),
(43873, 55840, 0, 1, 1, 1, 1),
(43873, 55841, 0, 1, 1, 1, 1),
(43873, 63040, 0.7, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43874, 56361, 0, 1, 1, 1, 1),
(43874, 56362, 0, 1, 1, 1, 1),
(43874, 56363, 0, 1, 1, 1, 1),
(43874, 56364, 0, 1, 1, 1, 1),
(43874, 56365, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43875, 55842, 0, 1, 2, 1, 1),
(43875, 55844, 0, 1, 2, 1, 1),
(43875, 55845, 0, 1, 1, 1, 1),
(43875, 55846, 0, 1, 1, 1, 1),
(43875, 55847, 0, 1, 2, 1, 1),
(43875, 55848, 0, 1, 2, 1, 1),
(43875, 55849, 0, 1, 2, 1, 1),
(43875, 55850, 0, 1, 2, 1, 1),
(43875, 55851, 0, 1, 1, 1, 1),
(43875, 55852, 0, 1, 1, 1, 1),
(43875, 55853, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43876, 52078, 100, 1, 0, 1, 1),
(43876, 52506, -100, 1, 0, 1, 1),
(43876, 56366, 0, 1, 1, 1, 1),
(43876, 56367, 0, 1, 2, 1, 1),
(43876, 56368, 0, 1, 2, 1, 1),
(43876, 56369, 0, 1, 2, 1, 1),
(43876, 56370, 0, 1, 1, 1, 1),
(43876, 56371, 0, 1, 2, 1, 1),
(43876, 56372, 0, 1, 1, 1, 1),
(43876, 56373, 0, 1, 1, 1, 1),
(43876, 56374, 0, 1, 2, 1, 1),
(43876, 56375, 0, 1, 2, 1, 1),
(43876, 56376, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43878, 55830, 0, 1, 1, 1, 1),
(43878, 55831, 0, 1, 1, 1, 1),
(43878, 55832, 0, 1, 1, 1, 1),
(43878, 55833, 0, 1, 1, 1, 1),
(43878, 55834, 0, 1, 1, 1, 1),
(43878, 65660, -100, 1, 0, 1, 1),
(43878, 71715, 0.3682, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(43879, 56356, 0, 1, 1, 1, 1),
(43879, 56357, 0, 1, 1, 1, 1),
(43879, 56358, 0, 1, 1, 1, 1),
(43879, 56359, 0, 1, 1, 1, 1),
(43879, 56360, 0, 1, 1, 1, 1),
(43879, 65660, -100, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44577, 55856, 0, 1, 1, 1, 1),
(44577, 55857, 0, 1, 1, 1, 1),
(44577, 55858, 0, 1, 1, 1, 1),
(44577, 55859, 0, 1, 1, 1, 1),
(44577, 55860, 0, 1, 1, 1, 1),
(44577, 71638, 0.5176, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(44819, 55871, 0, 1, 2, 1, 1),
(44819, 55872, 0, 1, 2, 1, 1),
(44819, 55873, 0, 1, 1, 1, 1),
(44819, 55874, 0, 1, 1, 1, 1),
(44819, 55875, 0, 1, 1, 1, 1),
(44819, 55876, 0, 1, 2, 1, 1),
(44819, 55877, 0, 1, 1, 1, 1),
(44819, 55878, 0, 1, 2, 1, 1),
(44819, 55879, 0, 1, 2, 1, 1),
(44819, 55880, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48710, 56412, 0, 1, 1, 1, 1),
(48710, 56413, 0, 1, 1, 1, 1),
(48710, 56414, 0, 1, 1, 1, 1),
(48710, 56415, 0, 1, 1, 1, 1),
(48710, 56416, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48714, 56422, 0, 1, 1, 1, 1),
(48714, 56423, 0, 1, 1, 1, 1),
(48714, 56424, 0, 1, 1, 1, 1),
(48714, 56425, 0, 1, 1, 1, 1),
(48714, 56426, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48776, 57871, 0, 1, 1, 1, 1),
(48776, 57872, 0, 1, 1, 1, 1),
(48776, 57873, 0, 1, 1, 1, 1),
(48776, 57874, 0, 1, 1, 1, 1),
(48776, 57875, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48815, 52078, 100, 1, 0, 1, 1),
(48815, 56427, 0, 1, 1, 1, 1),
(48815, 56428, 0, 1, 2, 1, 1),
(48815, 56429, 0, 1, 2, 1, 1),
(48815, 56430, 0, 1, 1, 1, 1),
(48815, 56431, 0, 1, 1, 1, 1),
(48815, 56432, 0, 1, 1, 1, 1),
(48815, 56433, 0, 1, 1, 1, 1),
(48815, 56434, 0, 1, 2, 1, 1),
(48815, 56435, 0, 1, 2, 1, 1),
(48815, 56436, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48902, 57866, 0, 1, 1, 1, 1),
(48902, 57867, 0, 1, 1, 1, 1),
(48902, 57868, 0, 1, 1, 1, 1),
(48902, 57869, 0, 1, 1, 1, 1),
(48902, 57870, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48932, 56379, 0, 1, 1, 1, 1),
(48932, 56380, 0, 1, 1, 1, 1),
(48932, 56381, 0, 1, 1, 1, 1),
(48932, 56382, 0, 1, 1, 1, 1),
(48932, 56383, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(48951, 56384, 0, 1, 1, 1, 1),
(48951, 56385, 0, 1, 1, 1, 1),
(48951, 56386, 0, 1, 1, 1, 1),
(48951, 56387, 0, 1, 1, 1, 1),
(48951, 56388, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(49045, 56389, 0, 1, 1, 1, 1),
(49045, 56390, 0, 1, 1, 1, 1),
(49045, 56391, 0, 1, 1, 1, 1),
(49045, 56392, 0, 1, 1, 1, 1),
(49045, 56393, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(49262, 56407, 0, 1, 1, 1, 1),
(49262, 56408, 0, 1, 1, 1, 1),
(49262, 56409, 0, 1, 1, 1, 1),
(49262, 56410, 0, 1, 1, 1, 1),
(49262, 56411, 0, 1, 1, 1, 1);
UPDATE `creature_template` SET `lootid` = `entry` WHERE `entry` IN (
    39378, 39425, 39428, 39587, 39731, 39732, 39788, 43612, 43614, 43873, 43874, 43875, 43876, 43878, 43879, 44577,
    44819, 48710, 48714, 48776, 48815, 48902, 48932, 48951, 49045, 49262
) AND `lootid` = 0;
