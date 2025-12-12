-- Fix Achievement Criteria Data errors

-- 1. Remove entries with unsupported data type 12
-- Data type 12 is not defined in AchievementCriteriaDataType enum (jumps from 11 to 13)
-- These entries cause validation errors and are ignored by the server
DELETE FROM `achievement_criteria_data` WHERE `type` = 12;

-- 2. Remove entry with unsupported data type 6
-- Data type 6 is not defined in AchievementCriteriaDataType enum (jumps from 5 to 7)
-- Entry 7703 has data type 6 which is not supported
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` = 7703 AND `type` = 6;

-- 3. Remove entries with non-existing criteria IDs
-- These criteria IDs do not exist in the Criteria.dbc file for MoP 5.4.8
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (
    100, 102, 140, 166, 176, 177, 221, 222, 223, 224, 225, 416, 417, 418, 420, 524, 525, 526, 527, 528, 529, 530, 531, 532, 534, 538, 539, 540, 541, 542, 544, 547, 551, 552, 594, 595, 596, 598, 599, 601, 602, 603, 604, 605, 606, 607, 1826, 2357, 3680, 3738, 4439, 4501, 4502, 5621, 5622, 5623, 5624, 5625, 5626, 5627, 5628, 5629, 5630, 5631, 5746, 5820, 6440, 6441, 6445, 6758, 7365, 7608, 7609, 7860, 9120, 10289, 10314, 10417, 11418, 11419, 11478, 11479, 11498, 11500, 11902, 11903, 11959, 12064, 12065, 12199, 12200, 12202, 12204, 12205, 12207, 12228, 12232, 12234, 12740, 12741, 12742, 12744, 12747, 12748, 12749, 12750, 12827, 12828, 12990, 12991, 13089, 13092, 13093, 13105, 13107, 13108
);

