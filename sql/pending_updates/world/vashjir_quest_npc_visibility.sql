-- Vashj'ir: quest NPCs that start invisible. The base data gives them an invisibility aura that only a "see" aura from the
-- 4.3.4 phase scenes can reveal, and nothing in this database applies those auras, so a player never sees Erunak
-- Stonespeaker (the end of "Call of Duty"), Captain Samir, Adarrah, Budd, Fiasco Sizzlegrin or Mack Fearsen. The 5.4.8 data
-- has none of these auras on them, so they are removed (the other auras of the NPC stay).
UPDATE `creature_template_addon` SET `auras` = '74706' WHERE `entry` = 36915;
UPDATE `creature_template_addon` SET `auras` = '76143 59562' WHERE `entry` = 39663;
UPDATE `creature_template_addon` SET `auras` = NULL WHERE `entry` IN (39667, 39669, 40918, 41618);
UPDATE `creature_template_addon` SET `auras` = '76483' WHERE `entry` = 39883;
UPDATE `creature_template_addon` SET `auras` = '75511' WHERE `entry` = 39911;
UPDATE `creature_template_addon` SET `auras` = '76143' WHERE `entry` = 40983;
UPDATE `creature_template_addon` SET `auras` = '77678 76143' WHERE `entry` = 41548;
UPDATE `creature_template_addon` SET `auras` = '59562' WHERE `entry` = 46458;
