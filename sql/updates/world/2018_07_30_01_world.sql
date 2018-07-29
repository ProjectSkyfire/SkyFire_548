-- Grand Expedition Yak - Vendor Addons
DELETE FROM creature_template_addon WHERE entry IN (62821,62822);
INSERT INTO creature_template_addon (entry, path_id, mount, bytes1, bytes2, emote, auras) VALUES
(62821, 0, 0, 0, 0, 0, '123236'),
(62822, 0, 0, 0, 0, 0, '123240');

-- Grand Expedition Yak - Spell Click Spell
DELETE FROM npc_spellclick_spells WHERE npc_entry=62809;
INSERT INTO npc_spellclick_spells (npc_entry, spell_id, cast_flags, user_type) VALUES
(62809, 122729, 1, 0);

-- Grand Expedition Yak - Vehicle Template
DELETE FROM vehicle_template_accessory entry=62809;
INSERT INTO vehicle_template_accessory (entry, accessory_entry, seat_id, minion, description, summontype, summontimer) VALUES
(62809, 62821, 0, 1, 'Grand Expedition Yak - Transmogrifier', 6, 30000),
(62809, 62822, 1, 1, 'Grand Expedition Yak - Traveling Trader', 6, 30000);
