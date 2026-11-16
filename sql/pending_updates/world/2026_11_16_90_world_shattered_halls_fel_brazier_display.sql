-- Shattered Halls: the Fel Brazier spell focus (181679) used display 1287, which the 5.4.8 client data does not have,
-- so its two spawns were refused at startup. Display 299 as in Legends of Azeroth 5.4.8. Safe to run twice.
UPDATE `gameobject_template` SET `displayId` = 299 WHERE `entry` = 181679;
