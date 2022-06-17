-- `game_event_gameobject` contains gameobject (GUID: 39805) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39806) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39807) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39808) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39809) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39810) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39811) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39812) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39813) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 39814) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 54946) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 54947) not found in `gameobject` table.
-- `game_event_gameobject` contains gameobject (GUID: 54948) not found in `gameobject` table.
-- Scarlet monastery got reworked in pandaria so old spawns dont exist anymore for hallows end
DELETE FROM `game_event_gameobject` WHERE guid IN
(39805, 39806, 39807, 39808, 39809, 39810, 39811, 39812, 39813, 39814, 54946, 54947);