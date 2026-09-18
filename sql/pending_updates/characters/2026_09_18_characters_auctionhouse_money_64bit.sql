-- Widen the auction money columns to 64 bits.
--
-- AuctionEntry::LoadFromDB and LoadFromFieldList read buyoutprice, lastbid, startbid and deposit
-- with Field::GetUInt64(), and AuctionEntry::SaveToDB binds all four with setUInt64. The columns
-- were `int unsigned`, so on a prepared-statement result MySQL sizes those buffers at 4 bytes
-- while PreparedResultSet::GetUInt64 reinterpret_casts and reads 8. The type check that would
-- have caught it is behind #ifdef SKYFIRE_DEBUG, so a Release build reads past the value in
-- silence and every price comes back with 32 correct bits and 32 bits of the next field.
--
-- Widening rather than narrowing the reads: `int unsigned` tops out at 4,294,967,295 copper
-- (429,496 gold) against a MAX_MONEY_AMOUNT of 9,999,999,999 copper. characters.money, mail.money
-- and guild.BankMoney are already bigint unsigned; these four were missed.
--
-- No data changes - the stored values are correct and widening preserves them.

ALTER TABLE `auctionhouse`
  MODIFY `buyoutprice` BIGINT UNSIGNED NOT NULL DEFAULT 0,
  MODIFY `lastbid`     BIGINT UNSIGNED NOT NULL DEFAULT 0,
  MODIFY `startbid`    BIGINT UNSIGNED NOT NULL DEFAULT 0,
  MODIFY `deposit`     BIGINT UNSIGNED NOT NULL DEFAULT 0;
