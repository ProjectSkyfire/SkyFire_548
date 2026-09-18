INSERT INTO `hub_managed_services`
  (`service_key`, `name`, `executable_path`, `config_path`, `working_directory`, `enabled`)
VALUES
  ('world', 'World Server', 'worldserver', 'worldserver.conf', '.', 1)
ON DUPLICATE KEY UPDATE
  `name` = VALUES(`name`),
  `executable_path` = VALUES(`executable_path`),
  `config_path` = VALUES(`config_path`),
  `enabled` = VALUES(`enabled`);
