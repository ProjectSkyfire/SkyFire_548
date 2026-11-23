-- Remove the remaining quest-giver links for the pre-Cataclysm Morbent Fel chain.
-- 270: The Doomed Fleet; 325: Armed and Ready.
-- Preserve the quest templates and character completion history.
DELETE FROM `creature_queststarter` WHERE `quest` IN (270, 325);
DELETE FROM `creature_questender` WHERE `quest` IN (270, 325);
DELETE FROM `gameobject_queststarter` WHERE `quest` IN (270, 325);
DELETE FROM `gameobject_questender` WHERE `quest` IN (270, 325);

-- Prevent event activation from restoring obsolete quest-giver links.
DELETE FROM `game_event_creature_quest` WHERE `quest` IN (270, 325);
DELETE FROM `game_event_gameobject_quest` WHERE `quest` IN (270, 325);
