-- Lost City of the Tol'vir: the six Wind Tunnels around the ruins had no script, so a player who clicked one only sat in its seat and Siamat's platform
-- could not be reached. The script takes the passenger to the landing zone of the platform once Husam, Lockmaw and Barim are dead.
UPDATE `creature_template` SET `ScriptName` = 'npc_wind_tunnel' WHERE `entry` = 48092 AND `AIName` = '' AND `ScriptName` = '';
