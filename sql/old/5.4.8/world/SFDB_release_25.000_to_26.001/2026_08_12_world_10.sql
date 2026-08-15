DROP TEMPORARY TABLE IF EXISTS tmp_frightened_citizen;
CREATE TEMPORARY TABLE tmp_frightened_citizen AS SELECT * FROM creature_template WHERE entry=34851;
UPDATE tmp_frightened_citizen SET entry=35836, name='Frightened Citizen', ScriptName='';
DELETE FROM creature_template WHERE entry=35836;
INSERT INTO creature_template SELECT * FROM tmp_frightened_citizen;
DROP TEMPORARY TABLE tmp_frightened_citizen;

UPDATE gameobject_template SET ScriptName='go_merchant_square_door' WHERE entry=195327;
