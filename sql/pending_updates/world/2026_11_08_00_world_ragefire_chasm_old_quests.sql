-- The Ragefire Chasm rebuild replaced the Cataclysm quests of Stone Guard Kurjack and Bovaal Whitehorn, and the
-- "Enemies Below" breadcrumbs that sent players to Kurjack, with the quests of Commander Bagran. Stop offering the old
-- ones; the quest enders stay so a player who already holds one can still hand it in.
DELETE FROM `creature_queststarter` WHERE (`id`=44217 AND `quest` IN (26856, 26858)) OR (`id`=44216 AND `quest`=26862);
DELETE FROM `creature_queststarter` WHERE (`id`=39605 AND `quest`=26865) OR (`id`=36648 AND `quest`=26866) OR (`id`=10181 AND `quest`=26867);
