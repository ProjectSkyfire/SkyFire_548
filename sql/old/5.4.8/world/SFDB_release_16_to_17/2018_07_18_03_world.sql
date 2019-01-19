DROP TABLE IF EXISTS phase_definitions;

DROP TABLE IF EXISTS terrain_phase_info;
CREATE TABLE terrain_phase_info(  
  Id INT(10) unsigned NOT NULL,
  TerrainSwapMap INT(10) unsigned NOT NULL,
  Comment VARCHAR(255),
  PRIMARY KEY (Id, TerrainSwapMap)
);

DROP TABLE IF EXISTS terrain_swap_defaults;
CREATE TABLE terrain_swap_defaults(  
  MapId INT(10) unsigned NOT NULL,
  TerrainSwapMap INT(10) unsigned NOT NULL,
  Comment VARCHAR(255),
  PRIMARY KEY (MapId, TerrainSwapMap)
);
DELETE FROM terrain_swap_defaults WHERE MapId IN (1, 654);
INSERT INTO terrain_swap_defaults (MapId, TerrainSwapMap, Comment) VALUES 
(1, 719, 'Mount Hyjal default terrain'),
(654, 638, 'Gilneas default terrain');

DROP TABLE IF EXISTS terrain_worldmap;
CREATE TABLE terrain_worldmap(  
  TerrainSwapMap INT(10) unsigned NOT NULL,
  WorldMapArea INT(10) unsigned NOT NULL,
  Comment VARCHAR(255),
  PRIMARY KEY (TerrainSwapMap, WorldMapArea)
);
DELETE FROM terrain_worldmap WHERE TerrainSwapMap IN (638, 655, 719, 656);
INSERT INTO terrain_worldmap (TerrainSwapMap, WorldMapArea, Comment) VALUES 
(638, 545, 'Gilneas'),
(655, 678, 'Gilneas_terrain1'),
(719, 683, 'Hyjal_terrain1'),
(656, 679, 'Gilneas_terrain2');

DROP TABLE IF EXISTS phase_area;
CREATE TABLE phase_area (
  AreaId int(10) unsigned NOT NULL,
  PhaseId int(10) unsigned NOT NULL,
  Comment varchar(255) DEFAULT NULL,
  PRIMARY KEY (AreaId,PhaseId)
);

ALTER TABLE creature
	ADD COLUMN phaseId INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER spawnMask,
	ADD COLUMN phaseGroup INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER phaseId,
	DROP COLUMN phaseMask;

ALTER TABLE gameobject
   ADD COLUMN phaseId INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER spawnMask,
	ADD COLUMN phaseGroup INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER phaseId,
	DROP COLUMN phaseMask;
