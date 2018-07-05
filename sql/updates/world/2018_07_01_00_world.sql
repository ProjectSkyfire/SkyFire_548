-- Fixes npc 53714 and 57873 for quest "The Lesson of the Sandy Fist" 29406 Wandering Isle
UPDATE creature_template SET ScriptName = "npc_training_target", WDBVerified=18414 WHERE entry IN (53714,57873);
UPDATE creature SET spawntimesecs = 30 WHERE id IN  (53714,57873);
