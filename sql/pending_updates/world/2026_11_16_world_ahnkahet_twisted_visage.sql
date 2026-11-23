-- The Twisted Visages of Herald Volazj had no AI and only meleed. Bind the script that gives them the abilities of
-- the class and specialization of the player they copy.
UPDATE `creature_template` SET `ScriptName` = 'npc_twisted_visage' WHERE `entry` = 30625 AND `ScriptName` = '';
