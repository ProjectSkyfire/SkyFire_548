-- Quest 31450 "A New Fate" -- kembalikan pilihan faksi Pandaren.
--
-- GEJALA
--   Setelah quest diterima, Spirit of Master Shang Xi (entry 56013) di Temple
--   of Five Dawns diam saja waktu diklik. Dialog pilihan Alliance/Horde tidak
--   pernah muncul, jadi quest mustahil diselesaikan dan pandaren netral
--   terkunci selamanya tanpa faksi.
--
-- SEBAB 1 -- ScriptName di DB tidak cocok dengan script yang ada di core
--   Dump world lama mengisi `ScriptName` entry 56013 dengan 'npc_choose_faction'.
--   Nama itu tidak pernah ada di core ini. Script yang benar-benar terdaftar
--   adalah 'npc_Spirit_of_Master_Shang_Xi'
--   (src/server/scripts/World/npcs_special.cpp -- CreatureScript + AddSC).
--
--   ObjectMgr::LoadScriptNames() (ObjectMgr.cpp:8466) membangun daftar nama
--   script DARI database, jadi 'npc_choose_faction' tetap dapat ScriptID.
--   Tapi tidak ada CreatureScript C++ yang terdaftar di id itu, sehingga NPC
--   berjalan tanpa script sama sekali.
--
--   Konfirmasinya ada di log worldserver saat start (tipe `sql.sql`,
--   ScriptMgr.cpp:98) -- arah kebalikannya yang dilaporkan:
--     Script named 'npc_Spirit_of_Master_Shang_Xi' does not have a script name
--     assigned in database.
--
--   Tanpa script, gossip "I would like to choose my faction" tidak pernah
--   ditambahkan, sehingga Player::ShowNeutralPlayerFactionSelectUI() tidak
--   pernah dipanggil, SMSG_SHOW_NEURTRAL_PLAYER_FACTION_SELECT_UI tidak pernah
--   terkirim, client tidak pernah membalas CMSG_SELECT_FACTION, dan
--   WorldSession::HandleSelectFactionOpcode() (MiscHandler.cpp:2937) -- yang
--   memberi kill credit 64594 penyelesai quest ini -- tidak pernah jalan.
--
-- SEBAB 2 -- npcflag tanpa bit GOSSIP (yang membuatnya benar-benar bisu)
--   56013 hanya punya UNIT_NPC_FLAG_QUESTGIVER (2), tanpa UNIT_NPC_FLAG_GOSSIP (1).
--   Begitu quest sudah diterima, quest menu jadi kosong. Dengan gossip menu juga
--   kosong (tidak ada script, gossip_menu_id = 0), Player::SendPreparedGossip()
--   (Player.cpp:12012) melewati jalur SendPreparedQuest dan mengirim gossip
--   kosong ke NPC yang tidak punya bit gossip -- client tidak menampilkan apa pun.
--   Itulah kenapa NPC terlihat bisu HANYA setelah quest diterima.
--
-- Kedua handler gossip (CMSG_GOSSIP_HELLO di NPCHandler.cpp:338 dan
-- CMSG_QUESTGIVER_HELLO di QuestHandler.cpp:147) sama-sama memanggil
-- sScriptMgr->OnGossipHello, jadi menambah bit gossip aman untuk kedua jalur.
--
-- Karakter yang sudah terlanjur stuck tidak perlu diperbaiki manual: begitu
-- script terikat dan world restart, ngobrol lagi ke Spirit memunculkan opsinya.

UPDATE `creature_template`
SET `ScriptName` = 'npc_Spirit_of_Master_Shang_Xi',
    `npcflag`    = `npcflag` | 1
WHERE `entry` = 56013;

-- Baris spawn menimpa npcflag template kalau nilainya bukan 0
-- (ObjectMgr::ChooseCreatureFlags, ObjectMgr.cpp:1195-1201), jadi bit gossip
-- harus ikut di-OR di sana. ScriptName tidak punya override per-spawn, jadi
-- perbaikan utama di atas tidak terpengaruh hal ini.
UPDATE `creature`
SET `npcflag` = `npcflag` | 1
WHERE `id` = 56013 AND `npcflag` <> 0;
