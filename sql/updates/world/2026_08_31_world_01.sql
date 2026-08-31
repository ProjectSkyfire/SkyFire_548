-- Remove class spells from playercreateinfo_spell that are not true level-1
-- (or death knight level-55) starters. Levels are from SpellLevels.dbc.
-- Spec/mastery/pushback/high-level abilities were incorrectly granted at creation.

DELETE FROM `playercreateinfo_spell` WHERE `Spell` IN (
  -- Warrior
  32215,  -- Victorious State (level 5)

  -- Hunter
  82928,  -- Aimed Shot! (level 10, Master Marksman proc)

  -- Priest
  77486,  -- Mastery: Shadowy Recall (level 80, Shadow mastery)
  84733,  -- Divine Focus (level 10)
  88684,  -- Holy Word: Serenity (level 20)
  88685,  -- Holy Word: Sanctuary (Holy ability, not a starter)
  101062, -- Flash Heal (level 7, hidden)

  -- Mage
  79684,  -- Arcane Missiles (level 24; Note wrongly said Offensive State)

  -- Warlock
  86213,  -- Soul Swap Exhale (Soul Swap talent helper, not a starter)
  89420,  -- Drain Life (level 6)

  -- Shaman
  89920,  -- Ancestral Focus (level 10, Elemental/Restoration)

  -- Druid
  79577,  -- Eclipse / Eclipse Mastery Driver (level 10, Balance)
  84736,  -- Nature's Focus (level 16, Restoration)
  84738   -- Celestial Focus (level 14, Balance)
);

-- Left intentionally (no SpellLevels gate / class passives or true starters):
-- Warrior/Paladin/Hunter/Rogue/Monk starters, DK 55 kit, Frostfire Bolt,
-- Focused Aim (87324), Suppression (87330), Paladin pushback (20208),
-- racials, weapon/armor skills, languages.
