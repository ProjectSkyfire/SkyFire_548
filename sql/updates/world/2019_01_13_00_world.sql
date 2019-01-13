-- delete lfg dungeon entrances for obsolete dungeons.
DELETE FROM lfg_entrances WHERE dungeonId IN (18, 165);
