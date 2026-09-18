-- Preserve node role for world maintenance policies across hub restarts.
ALTER TABLE `hub_cluster_policy`
  ADD COLUMN `service_type` tinyint unsigned NOT NULL DEFAULT '1' COMMENT '1 auth, 2 world';
