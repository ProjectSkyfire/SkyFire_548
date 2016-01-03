# Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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
  
  # Dungeons
  Pandaria/Dungeons/MoguShanPalace/boss_gekkan.cpp
  Pandaria/Dungeons/MoguShanPalace/boss_trial_of_the_king.cpp
  Pandaria/Dungeons/MoguShanPalace/boss_xin_the_weaponmaster.cpp
  Pandaria/Dungeons/MoguShanPalace/instance_mogu_shan_palace.cpp
  Pandaria/Dungeons/MoguShanPalace/mogu_shan_palace.h
  Pandaria/Dungeons/ShadoPanMonastery/shadopan_monastery.h
  Pandaria/Dungeons/ShadoPanMonastery/boss_gu_cloudstrike.cpp
  Pandaria/Dungeons/ShadoPanMonastery/boss_master_snowdrift.cpp
  Pandaria/Dungeons/ShadoPanMonastery/boss_sha_of_violence.cpp
  Pandaria/Dungeons/ShadoPanMonastery/boss_taran_zhu.cpp
  Pandaria/Dungeons/ShadoPanMonastery/instance_shadopan_monastery.cpp
  Pandaria/Dungeons/ShadoPanMonastery/shadopan_monastery.cpp
  Pandaria/Dungeons/TempleoftheJadeSerpent/temple_of_jade_serpent.h
  Pandaria/Dungeons/TempleoftheJadeSerpent/instance_temple_of_jade_serpent.cpp
  Pandaria/Dungeons/TempleoftheJadeSerpent/boss_wise_mari.cpp
  Pandaria/Dungeons/TempleoftheJadeSerpent/boss_sha_of_doubt.cpp
  Pandaria/Dungeons/TempleoftheJadeSerpent/boss_lorewalker_stonestep.cpp
  Pandaria/Dungeons/TempleoftheJadeSerpent/boss_liu_flameheart.cpp
  Pandaria/Dungeons/StormstoutBrewery/stormstout_brewery.h
  Pandaria/Dungeons/StormstoutBrewery/boss_hoptallus.cpp
  Pandaria/Dungeons/StormstoutBrewery/boss_ook_ook.cpp
  Pandaria/Dungeons/StormstoutBrewery/boss_yan_zhu.cpp
  Pandaria/Dungeons/StormstoutBrewery/boss_yan_zhu_the_uncasked.cpp
  Pandaria/Dungeons/StormstoutBrewery/instance_stormstout_brewery.cpp
  Pandaria/Dungeons/StormstoutBrewery/stormstout_brewery.cpp
  Pandaria/Dungeons/GateoftheSettingSun/gate_setting_sun.h
  Pandaria/Dungeons/GateoftheSettingSun/gate_setting_sun.cpp
  Pandaria/Dungeons/GateoftheSettingSun/instance_gate_setting_sun.cpp
  Pandaria/Dungeons/GateoftheSettingSun/boss_commander_rimok.cpp
  Pandaria/Dungeons/GateoftheSettingSun/boss_raigonn.cpp
  Pandaria/Dungeons/GateoftheSettingSun/boss_saboteur_kiptilak.cpp
  Pandaria/Dungeons/GateoftheSettingSun/boss_striker_gadok.cpp
  Pandaria/Dungeons/SiegeofNiuzaoTemple/siege_of_the_niuzoa_temple.h
  Pandaria/Dungeons/SiegeofNiuzaoTemple/siege_of_the_niuzoa_temple.cpp
  Pandaria/Dungeons/SiegeofNiuzaoTemple/boss_jinbak.cpp
  Pandaria/Dungeons/SiegeofNiuzaoTemple/instance_siege_of_the_niuzoa_temple.cpp

  # Raids
  Pandaria/Raids/MogushanVault/boss_elegon.cpp
  Pandaria/Raids/MogushanVault/boss_feng.cpp
  Pandaria/Raids/MogushanVault/boss_garajal.cpp
  Pandaria/Raids/MogushanVault/boss_spirit_kings.cpp
  Pandaria/Raids/MogushanVault/boss_stone_guard.cpp
  Pandaria/Raids/MogushanVault/boss_will_of_emperor.cpp
  Pandaria/Raids/MogushanVault/instance_mogu_shan_vault.cpp
  Pandaria/Raids/MogushanVault/mogu_shan_vault.cpp
  Pandaria/Raids/MogushanVault/mogu_shan_vault.h
  Pandaria/Raids/TerraceOfEndlessSpring/boss_lei_shi.cpp
  Pandaria/Raids/TerraceOfEndlessSpring/boss_protectors_of_the_endless.cpp
  Pandaria/Raids/TerraceOfEndlessSpring/boss_sha_of_fear.cpp
  Pandaria/Raids/TerraceOfEndlessSpring/boss_tsulong.cpp
  Pandaria/Raids/TerraceOfEndlessSpring/instance_terrace_of_endless_spring.cpp
  Pandaria/Raids/TerraceOfEndlessSpring/terrace_of_endless_spring.h
  Pandaria/Raids/ThroneOfThunder/boss_council_of_elders.cpp
  Pandaria/Raids/ThroneOfThunder/boss_dark_animus.cpp
  Pandaria/Raids/ThroneOfThunder/boss_durumu_the_forgotten.cpp
  Pandaria/Raids/ThroneOfThunder/boss_horridon.cpp
  Pandaria/Raids/ThroneOfThunder/boss_iron_qon.cpp
  Pandaria/Raids/ThroneOfThunder/boss_ji_kun.cpp
  Pandaria/Raids/ThroneOfThunder/boss_jinrokh.cpp
  Pandaria/Raids/ThroneOfThunder/boss_lei_shen.cpp
  Pandaria/Raids/ThroneOfThunder/boss_megaera.cpp
  Pandaria/Raids/ThroneOfThunder/boss_primordius.cpp
  Pandaria/Raids/ThroneOfThunder/boss_ra_den.cpp
  Pandaria/Raids/ThroneOfThunder/boss_tortos.cpp
  Pandaria/Raids/ThroneOfThunder/boss_twin_consorts.cpp
  Pandaria/Raids/ThroneOfThunder/instance_throne_of_thunder.cpp
  Pandaria/Raids/ThroneOfThunder/throne_of_thunder.cpp
  Pandaria/Raids/ThroneOfThunder/throne_of_thunder.h

  # Timeless Island
  Pandaria/TimelessIsle/boss_chi_ji.cpp
  Pandaria/TimelessIsle/timeless_isle.cpp
  Pandaria/TimelessIsle/zone_timeless_isle.cpp
  Pandaria/TimelessIsle/timeless_isle.h

  # World Bosses
  Pandaria/WorldBosses/boss_galion.cpp
  Pandaria/WorldBosses/boss_nalak_the_storm_lord.cpp
  Pandaria/WorldBosses/boss_oondasta.cpp
  Pandaria/WorldBosses/boss_ordos.cpp
  Pandaria/WorldBosses/boss_sha_of_anger.cpp

  # Zones
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
  
  # Wandering Island (Panda start zone)
  Pandaria/Zones/WanderingIsland/WanderingIsland_East.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_North.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_South.cpp
  Pandaria/Zones/WanderingIsland/WanderingIsland_West.cpp
)

message("  -> Prepared: Pandaria")
