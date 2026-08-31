-- Store the realm the player last clicked so world handoff can match the Join key.
ALTER TABLE `account_authnet_session`
  ADD COLUMN `selected_realm_id` int unsigned NOT NULL DEFAULT 0 AFTER `realm_field`,
  ADD KEY `idx_authnet_session_selected_realm` (`selected_realm_id`);

UPDATE `account_authnet_session`
SET `selected_realm_id` = `realm_field`
WHERE `selected_realm_id` = 0
  AND `realm_field` <> 0;
