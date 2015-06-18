-- Human starting zone updates
DELETE FROM creature_text WHERE entry=49869 AND groupid=1 LIMIT 5;
INSERT INTO creature_text VALUES
(49869,1,0,"Come, monsters! We will crush you!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,1,"I'll kill a hundred more of your battle worgs!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,2,"Look alive, men! There are orcs and worgs about!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,3,"Your filthy dogs won't be enough!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,4,"Your worgs are no match for the might of Stormwind!",14,0,100,0,0,0,"Stormwind Infantry - Yell");

