-- Twin Peaks (map 726): BattlegroundTP::SetupBattleground creates both flag stands, the dropped flag and the buffs itself,
-- so the permanent copies in the world table are never used: clicking the permanent flag stand does nothing, and the buff
-- copies respawn after 7200 seconds instead of 120. Nothing refers to these guids (game_event_gameobject, pool_gameobject
-- and linked_respawn were checked).
-- Per guid, created by the script at the same place:
-- guid 61 Alliance Flag stand 179830; guid 70 Horde Flag stand 179831; guid 75 dropped Horde Flag 179786;
-- guid 60 Speed Buff 179871; guid 85 Speed Buff 179899; guid 65 Food Buff 179904; guid 67 Food Buff 179906;
-- guid 62 Berserk Buff 179905; guid 68 Berserk Buff 179907.
DELETE FROM `gameobject` WHERE `map` = 726 AND `guid` IN (
    61,  -- 179830 Alliance Flag stand at 2117.64 191.68 44.05; the script's own stand is at 2118.21 191.62 44.05 (0.57 yd)
    70,  -- 179831 Horde Flag stand at 1578.34 344.05 2.42; the script's own stand is at 1578.38 344.04 2.42 (0.04 yd)
    75,  -- 179786 Horde Flag (dropped-flag object, type 26) at 2107.04 210.61 43.66 in the Alliance base; the script creates dropped flags where a carrier falls
    60,  -- 179871 Speed Buff at 2175.87 226.62 43.76; the script's own is at 2171.28 222.33 43.80 (5.7 yd)
    85,  -- 179899 Speed Buff at 1544.55 303.85 0.69; the script's own (179871) is at 1545.40 304.03 0.59 (0.9 yd)
    65,  -- 179904 Food Buff at 1951.18 383.80 -10.53; the script's own is at 1952.12 383.86 -10.29 (1.0 yd)
    67,  -- 179906 Food Buff at 1754.16 242.12 -14.13; the script's own (179904) is at 1753.96 242.09 -14.12 (0.2 yd)
    62,  -- 179905 Berserk Buff at 1932.83 226.79 -17.06; the script's own is at 1934.37 226.06 -17.04 (1.7 yd)
    68   -- 179907 Berserk Buff at 1737.57 435.85 -8.09; the script's own (179905) is at 1725.24 446.43 -7.83 (16 yd)
);
-- Buff objects stacked on one spot in the world table: same entry, same spot (0.00 yd), same spawn mask and phase, no
-- pool, event or link. Each copy is a trap object that fires its buff spell on contact, so five Food Buffs on one spot
-- fire it five times. One copy of each stack stays.
DELETE FROM `gameobject` WHERE `map` = 726 AND `guid` IN (
    66,  -- 179904 Food Buff at 1951.18 383.80 -10.53, copy of kept guid 65 (0.00 yd)
    71,  -- 179904 Food Buff at 1951.18 383.80 -10.53, copy of kept guid 65 (0.00 yd)
    73,  -- 179904 Food Buff at 1951.18 383.80 -10.53, copy of kept guid 65 (0.00 yd)
    74,  -- 179904 Food Buff at 1951.18 383.80 -10.53, copy of kept guid 65 (0.00 yd)
    72,  -- 179905 Berserk Buff at 1932.83 226.79 -17.06, copy of kept guid 62 (0.00 yd)
    69   -- 179906 Food Buff at 1754.16 242.12 -14.13, copy of kept guid 67 (0.00 yd)
);
