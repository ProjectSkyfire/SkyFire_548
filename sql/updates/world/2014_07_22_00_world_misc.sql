-- Master Shang Xi Spawn
DELETE FROM creature WHERE guid=1808;
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(1808, 53566, 860, 1, 1, 1462.004, 3465.594, 181.5808, 2.852041, 120, 0, 0); -- Master Shang Xi

-- Master Shang Xi NpcFlag Gossip + Quest giver
UPDATE creature_template SET npcflag=npcflag|1|2 WHERE entry=53566;

-- Master Shang Xi Quest Enders
DELETE FROM creature_questender WHERE id=53566; 
INSERT INTO creature_questender (id, quest) VALUES 
(53566, 30039), -- Much to Learn
(53566, 30040), -- Much to Learn
(53566, 30041), -- Much to Learn
(53566, 30042), -- Much to Learn
(53566, 30043), -- Much to Learn
(53566, 30044), -- Much to Learn
(53566, 30045), -- Much to Learn
(53566, 30027), -- The Lesson of the Iron Bough
(53566, 30033), -- The Lesson of the Iron Bough
(53566, 30034), -- The Lesson of the Iron Bough
(53566, 30035), -- The Lesson of the Iron Bough
(53566, 30036), -- The Lesson of the Iron Bough
(53566, 30037), -- The Lesson of the Iron Bough
(53566, 30038), -- The Lesson of the Iron Bough
(53566, 29406), -- The Lesson of the Sandy Fist
(53566, 29524), -- The Lesson of Stifled Pride
(53566, 29408), -- The Lesson of the Burning Scroll
(53566, 29409); -- The Disciple's Challenge

-- Master Shang Xi Quest Starters
DELETE FROM creature_queststarter WHERE id=53566;
INSERT INTO creature_queststarter (id, quest) VALUES 
(53566, 30027), -- The Lesson of the Iron Bough
(53566, 30033), -- The Lesson of the Iron Bough
(53566, 30034), -- The Lesson of the Iron Bough
(53566, 30035), -- The Lesson of the Iron Bough
(53566, 30036), -- The Lesson of the Iron Bough
(53566, 30037), -- The Lesson of the Iron Bough
(53566, 30038), -- The Lesson of the Iron Bough
(53566, 29406), -- The Lesson of the Sandy Fist
(53566, 29524), -- The Lesson of Stifled Pride
(53566, 29408), -- The Lesson of the Burning Scroll
(53566, 29409), -- The Disciple's Challenge
(53566, 29410); -- Aysa of the Tushui

-- Master Shang Xi's Staff Spawn
DELETE FROM gameobject WHERE guid=145;
INSERT INTO gameobject (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(145, 211336, 860, 1, 1, 1461.488, 3464.919, 181.6296, 1.742691, 0, 0, 0, 1, 120, 255, 1);

-- Master Shang Xi's Staff Template
DELETE FROM gameobject_template WHERE entry=211336;
INSERT INTO gameobject_template (entry, type, displayId, name, IconName, castBarCaption, unk1, data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, data31, size, unkInt32, WDBVerified) VALUES
(211336, 5, 11429, 'Master Shang Xi''s Staff', '', '', '', 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.75, 175, 18414);

-- Master Shang Xi's Barrel Spawn
DELETE FROM gameobject WHERE guid=146;
INSERT INTO gameobject (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(146, 211337, 860, 1, 1, 1461.142, 3464.874, 181.6214, 3.810972, 0, 0, 0, 1, 120, 255, 1);

-- Master Shang Xi's Barrel Template
DELETE FROM gameobject_template WHERE entry=211337;
INSERT INTO gameobject_template (entry, type, displayId, name, IconName, castBarCaption, unk1, data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, data31, size, unkInt32, WDBVerified) VALUES
(211337, 5, 11430, 'Master Shang Xi''s Barrel', '', '', '', 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.45, 176, 18414);

-- Edict of Temperance Spawn
DELETE FROM gameobject WHERE guid=147;
INSERT INTO gameobject (guid, id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(147, 210986, 860, 1, 1, 1421.908, 3377.979, 192.9827, 5.979019, 0, 0, 0, 1, 120, 255, 1);

-- Edict of Temperance Template
DELETE FROM gameobject_template WHERE entry=210986;
INSERT INTO gameobject_template (entry, type, displayId, name, IconName, castBarCaption, unk1, data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, data16, data17, data18, data19, data20, data21, data22, data23, data24, data25, data26, data27, data28, data29, data30, data31, size, unkInt32, WDBVerified) VALUES
(210986, 10, 10879, 'Edict of Temperance', '', '', '', 1634, 29408, 0, 3000, 0, 1, 0, 0, 0, 0, 114580, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 15642, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.6, 172, 18414);
