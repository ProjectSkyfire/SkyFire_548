-- reforgeID is written with setUInt32 (Item::SaveToDB / CHAR_REP_ITEM_INSTANCE) but was
-- declared signed, so any value past 2^31 was rejected under STRICT_TRANS_TABLES. One rejected
-- statement rolls back the entire Player::SaveToDB transaction, silently losing the character
-- save. Match the column to what the core actually binds so a bad value can never abort a save.

ALTER TABLE `item_instance` MODIFY `reforgeID` int unsigned NOT NULL DEFAULT 0;
