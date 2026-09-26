-- This file is part of Project SkyFire https://www.projectskyfire.org.
-- See LICENSE.md file for Copyright information
-- Opt-in social persistence foundation. Does not migrate or enable legacy writers.
CREATE TABLE character_social_owners (
 realm INT UNSIGNED NOT NULL,
 domain VARCHAR(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 epoch BIGINT UNSIGNED NOT NULL,
 node VARCHAR(64) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 revision BIGINT UNSIGNED NOT NULL DEFAULT 0,
 PRIMARY KEY(realm,domain)
) ENGINE=InnoDB;
CREATE TABLE character_social_retired (
 realm INT UNSIGNED NOT NULL,
 domain VARCHAR(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 PRIMARY KEY(realm,domain,instance)
) ENGINE=InnoDB;
CREATE TABLE character_social_records (
 realm INT UNSIGNED NOT NULL,
 domain VARCHAR(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 record_key VARBINARY(192) NOT NULL,
 revision BIGINT UNSIGNED NOT NULL,
 document MEDIUMBLOB NULL,
 PRIMARY KEY(realm,domain,record_key)
) ENGINE=InnoDB;
CREATE TABLE character_social_receipts (
 realm INT UNSIGNED NOT NULL,
 domain VARCHAR(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 request_id CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 instance CHAR(32) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 digest CHAR(64) CHARACTER SET ascii NOT NULL,
 revision BIGINT UNSIGNED NOT NULL,
 PRIMARY KEY(realm,domain,request_id)
) ENGINE=InnoDB;
CREATE TABLE character_social_outbox (
 realm INT UNSIGNED NOT NULL,
 domain VARCHAR(16) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
 revision BIGINT UNSIGNED NOT NULL,
 record_key VARBINARY(192) NOT NULL,
 document MEDIUMBLOB NULL,
 actor BIGINT UNSIGNED NOT NULL,
 PRIMARY KEY(realm,domain,revision)
) ENGINE=InnoDB;
