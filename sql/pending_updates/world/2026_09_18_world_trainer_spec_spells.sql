-- Remove class-trainer spells that SpecializationSpells.dbc restricts to a subset of the
-- class's specialisations.
--
-- The core already grants these on its own. GetSpellsForLevels (DBCStores.cpp:1164) walks
-- sSpecializationSpellsMap for the player's active specialisation and hands back every spell whose
-- SpellLevels.dbc value is in range; Player::GiveLevel, Player::_LoadSpells and the talent-spec
-- handler all learn that list. Player.cpp:4639 removes the same list again when a specialisation
-- is reset, so a spell bought at a trainer for another spec is taken away on the next spec change.
--
-- Selling them at a trainer is therefore both redundant and wrong:
--
--   * GetTrainerSpellState has no specialisation check, so the list is offered to every player of
--     the class. Immolate is Destruction-only and carries RemovesSpellId 172 Corruption, so an
--     Affliction warlock can be sold a spell that replaces one of their own core abilities;
--     Incinerate replaces Shadow Bolt the same way, and Cobra Shot replaces Steady Shot.
--   * The reqlevel values are pre-Mists and disagree with the DBC in both directions --
--     Incinerate 64 against 10, Holy Radiance 83 against 28, Backstab 18 against 40,
--     Agony 12 against 36, Enrage 22 against 76.
--
-- 67 rows across 10 trainer lists, which 605 creature templates reference, 497 of them spawned.
-- Every npc_trainer row whose spell appears in SpecializationSpells.dbc is spec-restricted, so no
-- row that covers all of a class's specialisations is touched here.
--
-- This is the same cleanup upstream applied to playercreateinfo_spell in 2026_08_31_world_01.

-- One DELETE keyed on (entry, spell) so the statement is written once; the pairs below
-- carry the trainer level vs DBC level and the owning specialisation per spell.
DELETE FROM `npc_trainer` WHERE (`entry`, `spell`) IN (
-- Warrior (200001)
  (200001, 1160),  -- Demoralizing Shout       trainer level 52  DBC level 56  Protection
  (200001, 1464),  -- Slam                     trainer level 44  DBC level 18  Arms
  (200001, 1680),  -- Whirlwind                trainer level 36  DBC level 26  Arms/Fury
  (200001, 2565),  -- Shield Block             trainer level 28  DBC level 18  Protection
  (200001, 6572),  -- Revenge                  trainer level 40  DBC level 30  Protection
  (200001, 7384),  -- Overpower                trainer level 22  DBC level 30  Arms
  (200001, 86346), -- Colossus Smash           trainer level 81  DBC level 81  Arms/Fury
-- Paladin (200002)
  (200002, 635),   -- Holy Light               trainer level 7   DBC level 34  Holy
  (200002, 879),   -- Exorcism                 trainer level 18  DBC level 46  Retribution
  (200002, 2812),  -- Denounce                 trainer level 28  DBC level 20  Holy
  (200002, 20164), -- Seal of Justice          trainer level 64  DBC level 70  Retribution
  (200002, 26573), -- Consecration             trainer level 24  DBC level 34  Protection
  (200002, 54428), -- Divine Plea              trainer level 44  DBC level 46  Holy
  (200002, 82326), -- Divine Light             trainer level 62  DBC level 54  Holy
  (200002, 82327), -- Holy Radiance            trainer level 83  DBC level 28  Holy
  (200002, 84963), -- Inquisition              trainer level 81  DBC level 81  Retribution
-- Hunter (200003)
  (200003, 34026), -- Kill Command             trainer level 10  DBC level 10  Beast Mastery
  (200003, 77767), -- Cobra Shot               trainer level 81  DBC level 81  Beast Mastery/Survival
-- Rogue (200004)
  (200004, 53),    -- Backstab                 trainer level 18  DBC level 40  Subtlety
  (200004, 32645), -- Envenom                  trainer level 54  DBC level 20  Assassination
-- Priest (200005)
  (200005, 527),   -- Purify                   trainer level 26  DBC level 22  Discipline/Holy
  (200005, 596),   -- Prayer of Healing        trainer level 44  DBC level 46  Discipline/Holy
  (200005, 2050),  -- Heal                     trainer level 16  DBC level 28  Discipline/Holy
  (200005, 2060),  -- Greater Heal             trainer level 38  DBC level 34  Discipline/Holy
  (200005, 2944),  -- Devouring Plague         trainer level 28  DBC level 21  Shadow
  (200005, 8092),  -- Mind Blast               trainer level 9   DBC level 21  Shadow
  (200005, 14914), -- Holy Fire                trainer level 18  DBC level 18  Discipline/Holy
  (200005, 64843), -- Divine Hymn              trainer level 78  DBC level 78  Holy
  (200005, 73510), -- Mind Spike               trainer level 81  DBC level 44  Shadow
-- Death Knight (200006)
  (200006, 49020), -- Obliterate               trainer level 61  DBC level 58  Frost
  (200006, 56222), -- Dark Command             trainer level 65  DBC level 58  Blood
  (200006, 56815), -- Rune Strike              trainer level 67  DBC level 65  Blood
  (200006, 85948), -- Festering Strike         trainer level 64  DBC level 62  Unholy
-- Shaman (200007)
  (200007, 331),   -- Healing Wave             trainer level 7   DBC level 20  Restoration
  (200007, 1535),  -- Fire Nova                trainer level 28  DBC level 44  Enhancement
  (200007, 8232),  -- Windfury Weapon          trainer level 32  DBC level 30  Enhancement
  (200007, 51505), -- Lava Burst               trainer level 34  DBC level 34  Elemental/Restoration
  (200007, 51730), -- Earthliving Weapon       trainer level 54  DBC level 30  Restoration
  (200007, 77472), -- Greater Healing Wave     trainer level 68  DBC level 60  Restoration
-- Mage (200008)
  (200008, 116),   -- Frostbolt                trainer level 7   DBC level 12  Frost
  (200008, 2948),  -- Scorch                   trainer level 26  DBC level 48  Fire
  (200008, 5143),  -- Arcane Missiles          trainer level 3   DBC level 24  Arcane
  (200008, 30451), -- Arcane Blast             trainer level 20  DBC level 10  Arcane
-- Warlock (200009)
  (200009, 348),   -- Immolate                 trainer level 3   DBC level 12  Destruction
  (200009, 980),   -- Agony                    trainer level 12  DBC level 36  Affliction
  (200009, 1120),  -- Drain Soul               trainer level 10  DBC level 19  Affliction
  (200009, 1949),  -- Hellfire                 trainer level 30  DBC level 22  Demonology
  (200009, 5740),  -- Rain of Fire             trainer level 18  DBC level 21  Affliction/Destruction
  (200009, 6353),  -- Soul Fire                trainer level 20  DBC level 13  Demonology
  (200009, 27243), -- Seed of Corruption       trainer level 72  DBC level 60  Affliction
  (200009, 29722), -- Incinerate               trainer level 64  DBC level 10  Destruction
  (200009, 74434), -- Soulburn                 trainer level 10  DBC level 19  Affliction
-- Druid (200011)
  (200011, 1079),  -- Rip                      trainer level 54  DBC level 20  Feral/Guardian
  (200011, 2782),  -- Remove Corruption        trainer level 24  DBC level 22  Balance/Feral/Guardian
  (200011, 2912),  -- Starfire                 trainer level 8   DBC level 10  Balance
  (200011, 5217),  -- Tiger's Fury             trainer level 24  DBC level 10  Feral
  (200011, 5221),  -- Shred                    trainer level 46  DBC level 16  Feral
  (200011, 5229),  -- Enrage                   trainer level 22  DBC level 76  Guardian
  (200011, 6785),  -- Ravage                   trainer level 22  DBC level 54  Feral/Guardian
  (200011, 8936),  -- Regrowth                 trainer level 12  DBC level 18  Restoration
  (200011, 16864), -- Omen of Clarity          trainer level 20  DBC level 38  Feral
  (200011, 33763), -- Lifebloom                trainer level 64  DBC level 36  Restoration
  (200011, 50464), -- Nourish                  trainer level 8   DBC level 12  Restoration
  (200011, 52610), -- Savage Roar              trainer level 76  DBC level 18  Feral
  (200011, 88747), -- Wild Mushroom            trainer level 85  DBC level 84  Balance
  (200011, 88751), -- Wild Mushroom: Detonate  trainer level 85  DBC level 84  Balance
  (200011, 106839) -- Skull Bash               trainer level 22  DBC level 64  Feral/Guardian
);
