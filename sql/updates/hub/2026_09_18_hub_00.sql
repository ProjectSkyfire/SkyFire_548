-- Persistent routing policy is independent of the ephemeral service registry.
CREATE TABLE IF NOT EXISTS `hub_cluster_policy` (
  `node_key` varchar(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `name` varchar(100) NOT NULL,
  `capabilities` int unsigned NOT NULL DEFAULT '0',
  `admin_state` tinyint unsigned NOT NULL DEFAULT '0' COMMENT '0 enabled, 1 draining, 2 disabled',
  `updated_by` varchar(64) NOT NULL,
  `updated_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`node_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
