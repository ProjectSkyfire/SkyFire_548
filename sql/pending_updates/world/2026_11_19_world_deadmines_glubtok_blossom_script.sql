-- Glubtok's Fire Blossom (88129) and Frost Blossom (88169): the script that kept their heroic-only add summon off normal
-- difficulty is gone, because normal SpellInfo no longer carries effects that only heroic rows have.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (88129, 88169) AND `ScriptName` = 'spell_glubtok_blossom';
