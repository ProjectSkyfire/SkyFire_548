ALTER TABLE `characters`
  DROP `instance_mode_mask`,
  ADD `dungeonDifficulty` tinyint(3) unsigned NOT NULL DEFAULT '1' AFTER `instance_id`,
  ADD `raidDifficulty` tinyint(3) unsigned NOT NULL DEFAULT '3' AFTER `dungeonDifficulty`;
  
ALTER TABLE `groups`
  CHANGE `difficulty` `difficulty` tinyint(3) unsigned NOT NULL DEFAULT '1' AFTER `groupType`,
  CHANGE `raiddifficulty` `raidDifficulty` tinyint(3) unsigned NOT NULL DEFAULT '14' AFTER `difficulty`;