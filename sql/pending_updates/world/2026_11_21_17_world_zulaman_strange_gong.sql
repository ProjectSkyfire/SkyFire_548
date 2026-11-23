-- The Strange Gong at the entrance of Zul'Aman opens the Massive Gate and starts the speed run timer.
UPDATE `gameobject_template` SET `ScriptName` = 'go_strange_gong' WHERE `entry` = 187359 AND `ScriptName` = '';
