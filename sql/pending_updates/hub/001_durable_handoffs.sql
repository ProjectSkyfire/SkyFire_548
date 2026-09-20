-- Phase 9 foundation: shared, one-time handoff consumption. Pending; CI promotion is not enabled for hub.
CREATE TABLE hub_handoff_lock (
 id tinyint unsigned NOT NULL PRIMARY KEY
) ENGINE=InnoDB;
INSERT INTO hub_handoff_lock(id) VALUES (1);
CREATE TABLE hub_handoff_tokens (
 token char(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL PRIMARY KEY,
 account int unsigned NOT NULL,
 purpose tinyint unsigned NOT NULL,
 realm int unsigned NOT NULL,
 address varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
 destination varchar(96) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
 evidence varchar(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 state tinyint unsigned NOT NULL DEFAULT 0 COMMENT '0 pending, 1 consumed, 2 revoked',
 is_latest tinyint unsigned NOT NULL DEFAULT 1,
 latest_purpose tinyint unsigned GENERATED ALWAYS AS (IF(is_latest=1,purpose,NULL)) STORED,
 expires_at datetime(6) NOT NULL,
 UNIQUE KEY latest_handoff(account,latest_purpose),
 KEY handoff_expiry(expires_at)
) ENGINE=InnoDB;
