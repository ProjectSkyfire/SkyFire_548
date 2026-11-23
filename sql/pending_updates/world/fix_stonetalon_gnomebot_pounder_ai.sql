-- Gnomebot Pounder (35203) is a vehicle creature without an AI name, so it was given the passive vehicle AI: it never
-- attacked, never evaded and kept the player who hit it in combat. It is a kill target of Mr. D's Wild Ride (26028).
UPDATE `creature_template` SET `AIName` = 'AggressorAI' WHERE `entry` = 35203 AND `AIName` = '';
