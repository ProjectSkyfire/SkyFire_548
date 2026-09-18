-- Persist the origin of the account's current session key. World authentication must
-- not bypass one-time hub handoff by changing the client account alias to a username/email.
ALTER TABLE `account`
  ADD COLUMN `session_handoff_required` tinyint unsigned NOT NULL DEFAULT '0' AFTER `session_key`;
