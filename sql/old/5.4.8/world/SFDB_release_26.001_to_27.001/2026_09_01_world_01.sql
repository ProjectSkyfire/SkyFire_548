-- Thazz'ril's Pick (gameobject 178087, Vale das Provações) has GO_FLAG_INTERACT_COND
-- set with no matching condition, making it permanently unclickable
UPDATE `gameobject_template` SET `flags` = 0 WHERE `entry` = 178087;
