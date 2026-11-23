-- Utgarde Keep: Ingvar's Woe Strike damages whoever heals the marked player.
-- Ported from TrinityCore 3.3.5.

DELETE FROM `spell_script_names` WHERE `spell_id` IN (42730, 59735);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(42730, 'spell_ingvar_woe_strike'),
(59735, 'spell_ingvar_woe_strike');
