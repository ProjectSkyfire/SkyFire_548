-- Restore supported gossip options with missing type or NPC flags.
-- Based on the missing-option audit in sardi-dev's PR #1579.
-- Use explicit menu/option pairs: an icon alone does not establish an action.
-- Service entries have matching NPC flags and service data; navigation entries
-- have an existing destination menu or point of interest. Custom-script menus,
-- ambiguous duplicate text, and options without an implemented action are excluded.
-- In particular, do not enable 4150/1 (engineering unlearn), 9157/1 and 9157/3
-- (Ahune queue), or 12103/0 and 12103/1 (swim tonic) as plain gossip.
-- Preserve text, conditions, actions, and any option already repaired elsewhere.


-- Sovik - Hello Sovik, I wish to browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 980 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Hello Sovik, I wish to browse your goods.';
-- Captain Garran Vimes - What have you heard of the Shady Rest Inn?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 1161 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 1162 AND `ActionPoiID` = 0 AND `OptionText` = 'What have you heard of the Shady Rest Inn?';
-- Krog - The Burning Inn.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 1186 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 1187 AND `ActionPoiID` = 0 AND `OptionText` = 'The Burning Inn.';
-- Durnan Furcutter - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 1262 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Innkeeper Karakul, Innkeeper Darg Bloodclaw, Gholah - Make this inn your home.
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536 WHERE `MenuID` = 1290 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 5 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Make this inn your home.';
-- Innkeeper Keldamyr - What can I do at an inn?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 1293 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 1221 AND `ActionPoiID` = 0 AND `OptionText` = 'What can I do at an inn?';
-- Frezza - Where is the zeppelin now?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 1969 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 8765 AND `ActionPoiID` = 0 AND `OptionText` = 'Where is the zeppelin now?';
-- Krinkle Goodsteel - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 2242 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Idriana - I would like to check my deposit box.
UPDATE `gossip_menu_option` SET `OptionType` = 9, `OptionNpcflag` = 131072 WHERE `MenuID` = 2404 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 6 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to check my deposit box.';
-- Razor Hill Grunt - The bank
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3280 AND `ActionPoiID` = 0 AND `OptionText` = 'The bank';
-- Razor Hill Grunt - The wind rider master
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3281 AND `ActionPoiID` = 0 AND `OptionText` = 'The wind rider master';
-- Razor Hill Grunt - The inn
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3282 AND `ActionPoiID` = 403 AND `OptionText` = 'The inn';
-- Razor Hill Grunt - The stable master
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 3 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 4901 AND `ActionPoiID` = 0 AND `OptionText` = 'The stable master';
-- Razor Hill Grunt - A class trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 4 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3283 AND `ActionPoiID` = 0 AND `OptionText` = 'A class trainer';
-- Razor Hill Grunt - A profession trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3285 AND `OptionID` = 5 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3284 AND `ActionPoiID` = 0 AND `OptionText` = 'A profession trainer';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - The bank
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3311 AND `ActionPoiID` = 0 AND `OptionText` = 'The bank';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - The wind rider master
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3312 AND `ActionPoiID` = 0 AND `OptionText` = 'The wind rider master';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - The inn
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3313 AND `ActionPoiID` = 419 AND `OptionText` = 'The inn';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - The stable master
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 3 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 4903 AND `ActionPoiID` = 420 AND `OptionText` = 'The stable master';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - A class trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 4 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3329 AND `ActionPoiID` = 0 AND `OptionText` = 'A class trainer';
-- Brave Ironhorn, Brave Dawneagle, Brave Darksky, Brave Rockhorn, Brave Cloudmane - A profession trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3331 AND `OptionID` = 5 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 3330 AND `ActionPoiID` = 0 AND `OptionText` = 'A profession trainer';
-- Gorn One Eye - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 3625 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Carlin Redpath - Carlin, do you have the extended Annals of Darrowshire?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3864 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12023 AND `ActionPoiID` = 0 AND `OptionText` = 'Carlin, do you have the extended Annals of Darrowshire?';
-- Carlin Redpath - Are you interested in joining our caravan?  We're headed to Light's Hope Chapel.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 3864 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12026 AND `ActionPoiID` = 0 AND `OptionText` = 'Are you interested in joining our caravan?  We''re headed to Light''s Hope Chapel.';
-- Hegnar Rumbleshot - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4090 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Gordon Wendham - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4283 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Mydrannul - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4305 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Ahanu - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4325 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Zargh - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4341 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Nipsy - I wish to make a purchase, Nipsy.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 4845 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to make a purchase, Nipsy.';
-- Otho Moji'ko - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 6083 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Smith Slagtree - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 6085 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Mystic Yayo'jin - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 6086 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Katoom the Angler - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 6087 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Vargus - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 6527 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Vi'el - Hey Vi'el, show me your wares!
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7046 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Hey Vi''el, show me your wares!';
-- Logistics Officer Ulrike - Show me what I've earned the right to purchase.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7448 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Show me what I''ve earned the right to purchase.';
-- Laando - I wish to travel by hippogryph.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 7470 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to travel by hippogryph.';
-- Clopper Wizbang - I wish to browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7471 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to browse your goods.';
-- Pilot Marsha - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7628 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Dalinna - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7817 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Lebowski - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 7820 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Provisioner Tsaalt - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8031 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Bortega - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8058 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Radulf Leder - I would like to go to the battleground.
UPDATE `gossip_menu_option` SET `OptionType` = 12, `OptionNpcflag` = 1048576 WHERE `MenuID` = 8073 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 9 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to go to the battleground.';
-- The Black Bride - I would like to go to the battleground.
UPDATE `gossip_menu_option` SET `OptionType` = 12, `OptionNpcflag` = 1048576 WHERE `MenuID` = 8075 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 9 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to go to the battleground.';
-- Gargok - I would like to go to the battleground.
UPDATE `gossip_menu_option` SET `OptionType` = 12, `OptionNpcflag` = 1048576 WHERE `MenuID` = 8076 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 9 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to go to the battleground.';
-- Su'ura Swiftarrow - I would like to go to the battleground.
UPDATE `gossip_menu_option` SET `OptionType` = 12, `OptionNpcflag` = 1048576 WHERE `MenuID` = 8077 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 9 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to go to the battleground.';
-- Supply Officer Pestle - What do you have for sale?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8094 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'What do you have for sale?';
-- Vixton Pinchwhistle - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8124 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Dama Wildmane - Let me see the wyverns, Dama.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8629 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me see the wyverns, Dama.';
-- Theramore Guard - Bank
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 8851 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12854 AND `ActionPoiID` = 0 AND `OptionText` = 'Bank';
-- Theramore Guard - Class Trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 8851 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12855 AND `ActionPoiID` = 0 AND `OptionText` = 'Class Trainer';
-- Theramore Guard - Profession Trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 8851 AND `OptionID` = 3 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12865 AND `ActionPoiID` = 0 AND `OptionText` = 'Profession Trainer';
-- Leeni "Smiley" Smalls, Tini Smalls - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 8950 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Provisioner Lorkran - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 9633 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Master Siegesmith Corvus - I require repairs and supplies, Corvus.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 9706 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I require repairs and supplies, Corvus.';
-- Calder - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 10209 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Noblegarden Vendor, Noblegarden Merchant - I want to browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 10257 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I want to browse your goods.';
-- Juno Dufrain - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 10437 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Orphan Matron Aria - Ask about the orphans.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10502 AND `OptionID` = 4 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10505 AND `ActionPoiID` = 0 AND `OptionText` = 'Ask about the orphans.';
-- Dellylah - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 10526 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Gappy Silvertooth - Set me up with the phattest, shiniest bling you got!
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10620 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10619 AND `ActionPoiID` = 0 AND `OptionText` = 'Set me up with the phattest, shiniest bling you got!';
-- Szabo - Szabo, I need a hip, new outfit for the party I'm throwing!
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10622 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10621 AND `ActionPoiID` = 0 AND `OptionText` = 'Szabo, I need a hip, new outfit for the party I''m throwing!';
-- Missa Spekkies - I need some cool shades. What will two stacks of macaroons get me?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10624 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10623 AND `ActionPoiID` = 0 AND `OptionText` = 'I need some cool shades. What will two stacks of macaroons get me?';
-- Kilag Gorefang - Kilag, I... uh, somehow... er, lost my scout.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10708 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10711 AND `ActionPoiID` = 0 AND `OptionText` = 'Kilag, I... uh, somehow... er, lost my scout.';
-- Gobber, Gobber, Gobber - Gobber, I need to look in my pack.
UPDATE `gossip_menu_option` SET `OptionType` = 9, `OptionNpcflag` = 131072 WHERE `MenuID` = 10722 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 6 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Gobber, I need to look in my pack.';
-- Gormungan - I brought you something to eat.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10780 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10781 AND `ActionPoiID` = 0 AND `OptionText` = 'I brought you something to eat.';
-- Gormungan - Tell me again about Azsharite?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10780 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10784 AND `ActionPoiID` = 0 AND `OptionText` = 'Tell me again about Azsharite?';
-- Gormungan - Here, eat this! It's a special chocolate rock.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10780 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10796 AND `ActionPoiID` = 0 AND `OptionText` = 'Here, eat this! It''s a special chocolate rock.';
-- Kilrok Gorehammer - Kilrok, how did we lose Honor's Stand?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10895 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10894 AND `ActionPoiID` = 0 AND `OptionText` = 'Kilrok, how did we lose Honor''s Stand?';
-- Vinny Slapchop - Vinny, I just gotta get me a genuine Cataclysm souvenir!
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 10903 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Vinny, I just gotta get me a genuine Cataclysm souvenir!';
-- Warlord Gar'dul - I'm here on behalf of Warlord Bloodhilt, who wants you to know he will be arriving shortly.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10935 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 10937 AND `ActionPoiID` = 0 AND `OptionText` = 'I''m here on behalf of Warlord Bloodhilt, who wants you to know he will be arriving shortly.';
-- Naralex - What happened here?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 10941 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11024 AND `ActionPoiID` = 0 AND `OptionText` = 'What happened here?';
-- Brett "Coins" McQuid - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11009 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Nablya - Who are you?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11036 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11060 AND `ActionPoiID` = 0 AND `OptionText` = 'Who are you?';
-- Apprentice Crispin - I wish to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11143 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to buy from you.';
-- Apothecary Jerrod - Who's the little guy?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11144 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11183 AND `ActionPoiID` = 0 AND `OptionText` = 'Who''s the little guy?';
-- Gordo - Gordo, do you think you could repair my gear?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11159 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Gordo, do you think you could repair my gear?';
-- Chawg - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11223 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Shyn - Show me where I can fly.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 11304 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Show me where I can fly.';
-- Zorbin Fandazzle - Got anything interesting for sale, Zorbin?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11361 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Got anything interesting for sale, Zorbin?';
-- Salt-Flop - Murloc, I know that you hold the Amulet of Sevine.  I need it.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11479 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11490 AND `ActionPoiID` = 0 AND `OptionText` = 'Murloc, I know that you hold the Amulet of Sevine.  I need it.';
-- Private Pollard - Who are you, friend?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11489 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11509 AND `ActionPoiID` = 0 AND `OptionText` = 'Who are you, friend?';
-- Erunak Stonespeaker - May I rest here?
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536 WHERE `MenuID` = 11522 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 5 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'May I rest here?';
-- Erunak Stonespeaker - Do you have any supplies?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11522 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Do you have any supplies?';
-- Takrik Ragehowl - Tell me about the wolf ancient, Lo'Gosh.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11541 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11544 AND `ActionPoiID` = 0 AND `OptionText` = 'Tell me about the wolf ancient, Lo''Gosh.';
-- L'ghorek - L'ghorek, can you give me another Attuned Runestone of Binding?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11607 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11820 AND `ActionPoiID` = 0 AND `OptionText` = 'L''ghorek, can you give me another Attuned Runestone of Binding?';
-- Eliza Darkgrin - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11646 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Benjamin Sykes - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11672 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Irela Moonfeather - Show me where I can fly.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 11719 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Show me where I can fly.';
-- Sprinkle Noggenfogger - Make mine Noggenfogger!
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11731 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Make mine Noggenfogger!';
-- Bossy - Moo.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11741 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11742 AND `ActionPoiID` = 0 AND `OptionText` = 'Moo.';
-- Narkk - What've you got?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11743 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'What''ve you got?';
-- Fleet Master Seahorn - Here's the head and the hat, Fleet Master.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11744 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11745 AND `ActionPoiID` = 0 AND `OptionText` = 'Here''s the head and the hat, Fleet Master.';
-- Chris Moller - Why pie over cake?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11794 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11795 AND `ActionPoiID` = 0 AND `OptionText` = 'Why pie over cake?';
-- Linzi Hewstrike - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11801 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Orgrimmar Skyway Peon - Has a secret cult infiltrated the Orgrimmar skyway?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11809 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11808 AND `ActionPoiID` = 0 AND `OptionText` = 'Has a secret cult infiltrated the Orgrimmar skyway?';
-- Ashley Blank - I require training.
UPDATE `gossip_menu_option` SET `OptionType` = 5, `OptionNpcflag` = 16 WHERE `MenuID` = 11860 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 3 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I require training.';
-- Morris Vant - Have you noticed anything strange lately?  Any new customers in the blacksmith shop?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11934 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11935 AND `ActionPoiID` = 0 AND `OptionText` = 'Have you noticed anything strange lately?  Any new customers in the blacksmith shop?';
-- Morris Vant - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11934 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- Outfitter Mendelev - Have you noticed anything strange lately?  Any new customers, buying anything suspicious?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11936 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11937 AND `ActionPoiID` = 0 AND `OptionText` = 'Have you noticed anything strange lately?  Any new customers, buying anything suspicious?';
-- Outfitter Mendelev - I would like to buy from you.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 11936 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I would like to buy from you.';
-- High Cleric Alphus - I'm looking for someone who may have stolen some supplies from Hearthglen.  Have any leads for me?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11940 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11941 AND `ActionPoiID` = 0 AND `OptionText` = 'I''m looking for someone who may have stolen some supplies from Hearthglen.  Have any leads for me?';
-- Darkspear Headhunter - Auction House
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11973 AND `ActionPoiID` = 591 AND `OptionText` = 'Auction House';
-- Darkspear Headhunter - Bank
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11973 AND `ActionPoiID` = 591 AND `OptionText` = 'Bank';
-- Darkspear Headhunter - Class Trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11976 AND `ActionPoiID` = 0 AND `OptionText` = 'Class Trainer';
-- Darkspear Headhunter - Inn
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 3 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11974 AND `ActionPoiID` = 595 AND `OptionText` = 'Inn';
-- Darkspear Headhunter - Mailbox
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 4 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11975 AND `ActionPoiID` = 595 AND `OptionText` = 'Mailbox';
-- Darkspear Headhunter - Profession Trainer
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 5 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11977 AND `ActionPoiID` = 0 AND `OptionText` = 'Profession Trainer';
-- Darkspear Headhunter - Stable Master
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 6 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11975 AND `ActionPoiID` = 595 AND `OptionText` = 'Stable Master';
-- Darkspear Headhunter - Vendor
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 11969 AND `OptionID` = 7 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 11978 AND `ActionPoiID` = 0 AND `OptionText` = 'Vendor';
-- D'lom the Collector - Do you have any supplies?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12015 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Do you have any supplies?';
-- D'lom the Collector - Can you repair equipment?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12015 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12016 AND `ActionPoiID` = 0 AND `OptionText` = 'Can you repair equipment?';
-- Narianna - I wish to browse your wares.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12019 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to browse your wares.';
-- Fiona - Let me see what you have for sale, Fiona.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12022 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me see what you have for sale, Fiona.';
-- Urk Gagbaz - I want to browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12033 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I want to browse your goods.';
-- Argus Highbeacon - Are you interested in joining our caravan?  We're headed to Light's Hope Chapel.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12034 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12030 AND `ActionPoiID` = 0 AND `OptionText` = 'Are you interested in joining our caravan?  We''re headed to Light''s Hope Chapel.';
-- Argus Highbeacon - Could you repair my gear, Argus?
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12034 AND `OptionID` = 2 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Could you repair my gear, Argus?';
-- KTC Train-a-Tron Deluxe - First Aid
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12046 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 3 AND `ActionMenuID` = 12047 AND `ActionPoiID` = 0 AND `OptionText` = 'First Aid';
-- Kaja - I wish to browse your wares.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12056 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to browse your wares.';
-- Angus Stillmountain - Angus, I'm here to lead the Wildhammer squad and drop off the SI:7 agents.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12059 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12060 AND `ActionPoiID` = 0 AND `OptionText` = 'Angus, I''m here to lead the Wildhammer squad and drop off the SI:7 agents.';
-- Cayden Dunwald - Yeah...
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12113 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12114 AND `ActionPoiID` = 0 AND `OptionText` = 'Yeah...';
-- Brivelthwerp - Ice cream, Brivelthwerp. I need ice cream!
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12260 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Ice cream, Brivelthwerp. I need ice cream!';
-- Nomarch Teneth - King Phaoris sent me to ask you about Neferset activity.
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12356 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12357 AND `ActionPoiID` = 0 AND `OptionText` = 'King Phaoris sent me to ask you about Neferset activity.';
-- Swift Seahorse - Show me where I can go.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 12370 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Show me where I can go.';
-- Denmother Ulrica - Make this inn your home.
UPDATE `gossip_menu_option` SET `OptionType` = 8, `OptionNpcflag` = 65536 WHERE `MenuID` = 12433 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 5 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Make this inn your home.';
-- Old Umbehto - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12443 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Derak Nightfall - I wish to buy from you, Derak.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12449 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to buy from you, Derak.';
-- Russell Brower - I'm ready, Russell. Let's write a song!
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12451 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12455 AND `ActionPoiID` = 0 AND `OptionText` = 'I''m ready, Russell. Let''s write a song!';
-- Aaron "Sandy Toes" Williamson - I wish to browse your wares.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12591 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I wish to browse your wares.';
-- Fradd Swiftgear - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12675 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Snixx Quickfreeze - Let me see what you have for sale.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12761 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me see what you have for sale.';
-- General Taldris Moonfall - What can you tell me about the Sentinel Tree, general?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12790 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12983 AND `ActionPoiID` = 0 AND `OptionText` = 'What can you tell me about the Sentinel Tree, general?';
-- Deldren Ravenelm - What can you tell me about these spiders?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12795 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12981 AND `ActionPoiID` = 0 AND `OptionText` = 'What can you tell me about these spiders?';
-- Malfurion Stormrage - What are we building here?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12799 AND `OptionID` = 4 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 13002 AND `ActionPoiID` = 0 AND `OptionText` = 'What are we building here?';
-- Damek Bloombeard - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12823 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Thisalee Crow - How do I get out of here?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12833 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12977 AND `ActionPoiID` = 0 AND `OptionText` = 'How do I get out of here?';
-- Jensen Farran - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12835 AND `OptionID` = 3 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Mrs. Gant - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12838 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Faena Woolybush - Let me browse your goods.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 12850 AND `OptionID` = 1 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 1 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'Let me browse your goods.';
-- Dorda'en Nightweaver - How is Hamuul doing?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12914 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12915 AND `ActionPoiID` = 0 AND `OptionText` = 'How is Hamuul doing?';
-- Aggra - Umm, Aggra... shouldn't you be following Thrall?
UPDATE `gossip_menu_option` SET `OptionType` = 1, `OptionNpcflag` = 1 WHERE `MenuID` = 12950 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 0 AND `ActionMenuID` = 12951 AND `ActionPoiID` = 0 AND `OptionText` = 'Umm, Aggra... shouldn''t you be following Thrall?';
-- Pricilla Winterwind - I'd like to travel by air.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 13045 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I''d like to travel by air.';
-- Tomas Riverwell - I'd like to travel by air.
UPDATE `gossip_menu_option` SET `OptionType` = 4, `OptionNpcflag` = 8192 WHERE `MenuID` = 13046 AND `OptionID` = 0 AND `OptionType` = 0 AND `OptionNpcflag` = 0 AND `OptionIcon` = 2 AND `ActionMenuID` = 0 AND `ActionPoiID` = 0 AND `OptionText` = 'I''d like to travel by air.';
