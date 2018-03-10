# Copyright (C) 2011-2018 Project SkyFire <http://www.projectskyfire.org/
# Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  #World Boss Scripts
  Pandaria/WorldBosses/boss_galleon.cpp
  Pandaria/WorldBosses/boss_nalak.cpp
  Pandaria/WorldBosses/boss_oondasta.cpp
  Pandaria/WorldBosses/boss_sha_of_anger.cpp
  #Zone Scripts
  Pandaria/Zones/zone_dread_wastes.cpp
  Pandaria/Zones/zone_isle_of_giants.cpp
  Pandaria/Zones/zone_isle_of_thunder.cpp
  Pandaria/Zones/zone_krasarang_wilds.cpp
  Pandaria/Zones/zone_kun_lai_summit.cpp
  Pandaria/Zones/zone_the_jade_forest.cpp
  Pandaria/Zones/zone_the_veiled_stair.cpp
  Pandaria/Zones/zone_townlong_steppes.cpp
  Pandaria/Zones/zone_vale_of_eternal_blossoms.cpp
  Pandaria/Zones/zone_valley_of_the_four_winds.cpp
  #Sub-Zone TimelessIsle
  Pandaria/Zones/TimelessIsle/boss_chi_ji.cpp
  Pandaria/Zones/TimelessIsle/boss_niuzao.cpp
  Pandaria/Zones/TimelessIsle/boss_ordos.cpp
  Pandaria/Zones/TimelessIsle/boss_xuen.cpp
  Pandaria/Zones/TimelessIsle/boss_yu_lon.cpp
  Pandaria/Zones/TimelessIsle/timeless_isle.h
  Pandaria/Zones/TimelessIsle/timeless_isle_global.cpp
  #Rares Sub-Zone TimelessIsle
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_archereus_of_flame.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_bufo.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_champion_of_the_black_flame.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_chelon.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_cinderfall.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_cranegnasher.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_dread_ship_vazuvius.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_emerald_gander.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_evermaw.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_flintlord_gairan.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_garnia.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_golganarr.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_great_turtle_furyshell.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_gu_chi_the_swarmbringer.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_huolon.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_imperial_python.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_ironfur_steelhorn.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_jakur_of_ordon.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_karkanos.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_leafmender.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_monstrous_spineclaw.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_rattleskew.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_rock_moss.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_spelurk.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_spirit_of_jadefire.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_stinkbraid.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_tsavo_ka.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_urdur_the_cauterizer.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_watcher_osu.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_zesqua.cpp
  Pandaria/Zones/TimelessIsle/Rares/rare_mob_zhu_gon_the_sour.cpp
  #Sub-Zone WanderingIslands
  Pandaria/Zones/WanderingIsland/WanderingIsland_East.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_North.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_South.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_West.cpp
)

message("  -> Prepared: Pandaria")
