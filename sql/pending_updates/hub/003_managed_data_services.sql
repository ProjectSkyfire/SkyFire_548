-- This file is part of Project SkyFire https://www.projectskyfire.org.
-- See LICENSE.md file for Copyright information
-- 0 = native auth/world, 3 = bundled Python mapserver, 4 = bundled Python characterserver.
SET @service_kind_sql = IF(EXISTS(SELECT 1 FROM information_schema.columns WHERE table_schema=DATABASE()
    AND table_name='hub_managed_services' AND column_name='service_kind'), 'DO 0',
    'ALTER TABLE hub_managed_services ADD COLUMN service_kind TINYINT UNSIGNED NOT NULL DEFAULT 0');
PREPARE service_kind_update FROM @service_kind_sql;
EXECUTE service_kind_update;
DEALLOCATE PREPARE service_kind_update;
SET @cluster_key_sql = IF(EXISTS(SELECT 1 FROM information_schema.columns WHERE table_schema=DATABASE()
    AND table_name='hub_managed_services' AND column_name='cluster_key'), 'DO 0',
    'ALTER TABLE hub_managed_services ADD COLUMN cluster_key VARCHAR(64) NOT NULL DEFAULT ''''');
PREPARE cluster_key_update FROM @cluster_key_sql;
EXECUTE cluster_key_update;
DEALLOCATE PREPARE cluster_key_update;
