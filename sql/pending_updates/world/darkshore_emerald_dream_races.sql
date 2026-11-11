-- Darkshore: make Malfurion Stormrage's chain (The Emerald Dream, Leaving the Dream and the quests around it)
-- Alliance only. Malfurion uses faction template 80 (Darnassus), which is hated by Horde players, so a Horde player
-- who got one of these quests from Thessera (faction 35, friendly to everyone) could never turn it in.
-- Based on Legends of Azeroth 5.4.8 (AllowableRaces 18875469 on every one of these quests).
UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `Id` IN (13573, 13575, 13577, 13579, 13581, 13583, 13585, 13586, 13587, 13588, 13902, 13940);
