-- Mana Gem: drop orphaned Replenish Mana script binding (never implemented).
-- Restore amount is handled in core via SpellEffectScaling + gtSpellScaling (ScalingClass -2).
DELETE FROM `spell_script_names` WHERE `spell_id` = 5405 AND `ScriptName` = 'spell_mage_replenish_mana';
