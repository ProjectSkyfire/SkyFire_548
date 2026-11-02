-- The Blood Furnace: three of the four script-managed creatures cannot be targeted, so the
-- instance cannot be completed on normal difficulty.
--
--   17377 Keli'dan the Breaker  unit_flags 33587776 = 0x02008240
--   17380 Broggok               unit_flags 33587264 = 0x02008040
--   17398 Nascent Fel Orc       unit_flags 33587264 = 0x02008040
--   17381 The Maker             unit_flags    32832 = 0x00008040   <- works
--
-- The Maker is the only encounter in the instance that works, and the only one of the four
-- without UNIT_FLAG_NOT_SELECTABLE (0x02000000). The heroic entries do not carry it either:
-- 18601 Broggok (1) is 834, 18607 Keli'dan the Breaker (1) and 18612 Nascent Fel Orc (1) are 0.
--
-- Nothing clears it at runtime. All three release paths in the instance use a different set of
-- flags entirely:
--
--   boss_broggok.cpp:123             RemoveFlag(IMMUNE_TO_PC | IMMUNE_TO_NPC | NON_ATTACKABLE)
--   boss_kelidan_the_breaker.cpp:140 RemoveFlag(IMMUNE_TO_PC | IMMUNE_TO_NPC | NON_ATTACKABLE)
--   instance_blood_furnace.cpp:393   RemoveFlag(IMMUNE_TO_PC | IMMUNE_TO_NPC | NON_ATTACKABLE)
--
-- so a released prisoner or boss loses the three flags the script set and keeps the one the
-- client reads. Broggok's encounter deadlocks on it rather than merely reading wrong: the lever
-- opens a cell, the Nascent Fel Orcs charge, and because they cannot be killed no prisoner
-- death ever arrives to open the next cell, so his door never opens.
--
-- Only 0x02000000 is cleared. Keli'dan keeps IMMUNE_TO_NPC, all three keep 0x8040, and the
-- flags the scripts set at runtime are untouched.

UPDATE `creature_template` SET `unit_flags` = `unit_flags` & ~0x02000000
WHERE `entry` IN (17377, 17380, 17398);
