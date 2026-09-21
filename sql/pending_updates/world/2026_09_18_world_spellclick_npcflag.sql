-- Give UNIT_NPC_FLAG_SPELLCLICK (16777216) to the spawned, non-vehicle creatures that have
-- `npc_spellclick_spells` data but no flag, and add the missing spellclick row for Flamefly.
--
-- `Player::CanSeeSpellClickOn` returns false on the missing flag before it ever reads the
-- spellclick table, and the bit is masked out of the npcflags the client is sent
-- (Unit.cpp:9604), so opcode 1015 is never fired and the rows are dead data.
-- `ObjectMgr::LoadNPCSpellClickSpells` only ever *removes* this flag, never adds it.
--
-- Vehicles are deliberately excluded: `Vehicle::Install` sets the bit at runtime and
-- `Unit::RemoveVehicleKit` clears it again, so a static flag would be wrong for them.
-- Only creatures with at least one spawn are touched.

UPDATE `creature_template`
   SET `npcflag` = `npcflag` | 16777216
 WHERE `entry` IN (
    26191, -- Dusk                                 -- 46978 Dusk Taxi
    28161, -- Chicken Escapee                      -- 51037 -> item 38483, quests 12532/12702
    30272, -- Njorndar Proto-Drake                 -- 57401 summons 30564
    35111, -- Runaway Shredder                     -- 66778 Reprogramming, credit for quest 14129
    36287, -- Cynthia                              -- 68597, credit for quest 14368
    36288, -- Ashley                               -- 68598, credit for quest 14368
    36289, -- James                                -- 68596, credit for quest 14368
    36440, -- Drowning Watchman                    -- 68735 Rescue Drowning Watchman
    36459, -- Chance                               -- 68743 Interact Dummy
    36917, -- Surface to Other Surface Transport   -- 81432 summons 43534
    38150, -- Glaive Thrower                       -- 71238 summons 37927
    38933, -- Briny Sea Cucumber                   -- 73123 -> item 52068, quest 24976
    42548, -- Muddy Crawfish                       -- 79277 -> item 57765, quest 26226
    42644, -- Smoot                                -- 79936 Slam Smoot
    42646, -- AWOL Grunt                           -- 80017 Chewing Out
    42671, -- Chief Engineer                       -- 79568 Discipline
    42673, -- Hellscream Demolisher                -- 79545 Inspection
    43032, -- Slain Cannoneer                      -- 80393, kill credit 43028, quest 26245
    43048, -- Captain Skullshatter                 -- 80446, kill credit 43027, quest 26245
    43115, -- Dormant Stonebound Elemental         -- 56685, credit for quest 26439
    43182, -- Petrified Stone Bat                  -- 84178 Petrified Gloom Bat Cover
    44126, -- War Guardian                         -- 82535, credit for quests 26791/26792
    44427, -- Crowley's Horse                      -- 56685
    44429, -- Crowley's Horse                      -- 56685
    51396, -- Stonecore Teleporter                 -- 95284 Teleport
    51397, -- Stonecore Teleporter                 -- 95285 Teleport
    53517, -- Squirming Slime Mold                 -- 99328 -> item 69911, quest 29322
    53526, -- Brightwater Snail                    -- 99357 -> item 69918, quest 29333
    53561  -- Stonebull Crayfish                   -- 99473 -> item 69977, quest 29349
   )
   AND (`npcflag` & 16777216) = 0;

-- Flamefly (48671) is missing both halves: no `npc_spellclick_spells` row and no flag.
-- Spell 90672 "Create Flamefly" is a single SPELL_EFFECT_CREATE_ITEM for item 64409 on the
-- caster, and 64409 is the only thing quests 28415 "SEVEN! YUP!" and 28491 "Not Fireflies,
-- Flameflies" ask for (7 each). No loot table anywhere lists 64409.
-- cast_flags 1 = NPC_CLICK_CAST_CASTER_CLICKER, so the clicking player is the caster.

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 48671;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(48671, 90672, 1, 0);

UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` = 48671;
