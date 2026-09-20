-- This file is part of Project SkyFire https://www.projectskyfire.org.
-- See LICENSE.md file for Copyright information
-- Atomic ownership for coordinated node restarts. Maintenance remains set after interruption.
ALTER TABLE hub_backup_worker ADD COLUMN restart_owner varchar(32) NOT NULL DEFAULT '';
