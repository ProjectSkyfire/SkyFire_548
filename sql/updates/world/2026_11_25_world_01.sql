-- Innkeepers, stable masters and NPCs that are both trainer and vendor lacked the gossip flag (1) that the 5.4.8 data
-- sets next to their service flags. Without it the client skips the gossip menu and opens the first service directly,
-- so Cheng Dawnscrive only showed his goods and never offered to make the inn the home location.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1 WHERE `entry` IN (5814, 18752, 30308, 32411, 32418, 42873, 42963, 43378, 43699, 43771, 44006, 44190, 44219, 49688, 49747, 53409, 55684, 56777, 57313, 57619, 59582, 62867, 62868, 62873, 62874, 62878, 62882, 62883, 63016, 65220, 66230, 66236, 66241, 66242, 66243, 66246, 66248, 66249, 66250);
