UPDATE creature_template SET ScriptName='npc_josiah_avery' WHERE entry=35369;
UPDATE creature_template SET ScriptName='npc_josiah_avery_trigger' WHERE entry=50415;

DELETE FROM creature_text WHERE entry=50415;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(50415, 0, 0, 'You\'ve been bitten by a worgen. It\'s probably nothing, but it sure stings a little.', 4, 0, 100, 0, 0, 0, 'Josiah Event Trigger - bitten warning');
