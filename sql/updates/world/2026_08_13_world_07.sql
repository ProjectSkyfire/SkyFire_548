DELETE FROM gameobject WHERE guid=90379;
DELETE FROM game_event_gameobject WHERE guid=90379;
UPDATE gameobject_template SET data0=0 WHERE entry=195430;

UPDATE gameobject_template SET data2=0 WHERE entry=195621;
