DROP TABLE `daily_players_reports`;
CREATE TABLE IF NOT EXISTS `daily_players_reports` (
  `guid` int unsigned NOT NULL DEFAULT 0,
  `creation_time` int unsigned NOT NULL DEFAULT 0,
  `average` float NOT NULL DEFAULT 0,
  `total_reports` bigint unsigned NOT NULL DEFAULT 0,
  `speed_reports` bigint unsigned NOT NULL DEFAULT 0,
  `fly_reports` bigint unsigned NOT NULL DEFAULT 0,
  `jump_reports` bigint unsigned NOT NULL DEFAULT 0,
  `waterwalk_reports` bigint unsigned NOT NULL DEFAULT 0,
  `teleportplane_reports` bigint unsigned NOT NULL DEFAULT 0,
  `climb_reports` bigint unsigned NOT NULL DEFAULT 0,
  `time_maniputation_reports` bigint unsigned NOT NULL DEFAULT 0,
  `gravity_reports` bigint unsigned NOT NULL DEFAULT 0,
  `teleport_reports` bigint unsigned NOT NULL DEFAULT 0,
   `bg_start_reports` bigint unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE `players_reports_status`;
CREATE TABLE IF NOT EXISTS `players_reports_status` (
  `guid` int unsigned NOT NULL DEFAULT 0,
  `creation_time` int unsigned NOT NULL DEFAULT 0,
  `average` float NOT NULL DEFAULT 0,
  `total_reports` bigint unsigned NOT NULL DEFAULT 0,
  `speed_reports` bigint unsigned NOT NULL DEFAULT 0,
  `fly_reports` bigint unsigned NOT NULL DEFAULT 0,
  `jump_reports` bigint unsigned NOT NULL DEFAULT 0,
  `waterwalk_reports` bigint unsigned NOT NULL DEFAULT 0,
  `teleportplane_reports` bigint unsigned NOT NULL DEFAULT 0,
  `climb_reports` bigint unsigned NOT NULL DEFAULT 0,
  `time_maniputation_reports` bigint unsigned NOT NULL DEFAULT 0,
  `gravity_reports` bigint unsigned NOT NULL DEFAULT 0,
  `teleport_reports` bigint unsigned NOT NULL DEFAULT 0,
   `bg_start_reports` bigint unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

