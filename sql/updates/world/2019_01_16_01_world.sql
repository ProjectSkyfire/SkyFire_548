UPDATE creature_equip_template SET itemEntry2=0 WHERE itemEntry2=512;

DELETE FROM creature_equip_template WHERE itemEntry1 IN
(155857, 118409, 141604);
