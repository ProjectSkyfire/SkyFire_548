#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See COPYRIGHT file for Copyright information
#

add_executable(shared_protocol_data_tests SharedProtocolDataTests.cpp)
target_include_directories(shared_protocol_data_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Network
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${Boost_INCLUDE_DIRS}
)
if(TARGET Boost::headers)
  target_link_libraries(shared_protocol_data_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(shared_protocol_data_tests Boost::boost)
endif()
target_link_libraries(shared_protocol_data_tests Threads::Threads)
add_test(NAME shared_protocol_data_tests COMMAND shared_protocol_data_tests)

add_executable(database_queue_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database/DatabaseQueue.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database/DatabaseWorker.cpp
  DatabaseQueueTests.cpp
)

target_include_directories(database_queue_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Network
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${Boost_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR}
)

if(TARGET Boost::headers)
  target_link_libraries(database_queue_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(database_queue_tests Boost::boost)
endif()

target_link_libraries(database_queue_tests Threads::Threads)

add_test(NAME database_queue_tests COMMAND database_queue_tests)

add_executable(world_fallback_tests WorldFallbackTests.cpp)
target_include_directories(world_fallback_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/hub
)
add_test(NAME world_fallback_tests COMMAND world_fallback_tests)

add_executable(database_setup_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database/DatabaseSetup/DatabaseSetup.cpp
  DatabaseSetupTests.cpp
)

target_include_directories(database_setup_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database/DatabaseSetup
)

add_test(NAME database_setup_tests COMMAND database_setup_tests)

add_executable(realm_socket_tests
  ${CMAKE_SOURCE_DIR}/src/server/authserver/Server/RealmSocket.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets/PacketLogServer.cpp
  PacketLogServerConfigStub.cpp
  RealmSocketLogStub.cpp
  RealmSocketTests.cpp
)

target_include_directories(realm_socket_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Network
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/authserver/Server
  ${Boost_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

if(TARGET Boost::headers)
  target_link_libraries(realm_socket_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(realm_socket_tests Boost::boost)
endif()

target_link_libraries(realm_socket_tests
  Threads::Threads
)

add_test(NAME realm_socket_tests COMMAND realm_socket_tests)

add_executable(auth_patch_transfer_tests
  AuthPatchTransferTests.cpp
)

target_include_directories(auth_patch_transfer_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/authserver/Server
)

add_test(NAME auth_patch_transfer_tests COMMAND auth_patch_transfer_tests)

add_executable(login_identity_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Auth/LoginIdentity.cpp
  LoginIdentityTests.cpp
)

target_include_directories(login_identity_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Auth
)

add_test(NAME login_identity_tests COMMAND login_identity_tests)

add_executable(totp_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Auth/TOTP.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography/CryptoRandom.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/Errors.cpp
  TOTPTests.cpp
)

target_include_directories(totp_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Auth
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(totp_tests
  ${OPENSSL_LIBRARIES}
)

add_test(NAME totp_tests COMMAND totp_tests)

add_executable(account_data_utils_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities/AccountDataUtils.cpp
  AccountDataUtilsTests.cpp
)

target_include_directories(account_data_utils_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
)

add_test(NAME account_data_utils_tests COMMAND account_data_utils_tests)

add_executable(account_data_update_handler_tests
  AccountDataUpdateHandlerTests.cpp
)

target_compile_definitions(account_data_update_handler_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME account_data_update_handler_tests COMMAND account_data_update_handler_tests)

add_executable(world_auth_identity_tests
  WorldAuthIdentityTests.cpp
)

target_compile_definitions(world_auth_identity_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME world_auth_identity_tests COMMAND world_auth_identity_tests)

add_executable(character_creation_realm_limit_tests
  CharacterCreationRealmLimitTests.cpp
)

target_compile_definitions(character_creation_realm_limit_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME character_creation_realm_limit_tests COMMAND character_creation_realm_limit_tests)

add_executable(account_email_conversion_tests
  AccountEmailConversionTests.cpp
)

target_compile_definitions(account_email_conversion_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME account_email_conversion_tests COMMAND account_email_conversion_tests)

add_executable(network_address_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities/NetworkAddress.cpp
  NetworkAddressTests.cpp
)

target_include_directories(network_address_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(network_address_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(network_address_tests Boost::boost)
endif()

add_test(NAME network_address_tests COMMAND network_address_tests)

add_executable(memory_usage_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities/MemoryUsage.cpp
  MemoryUsageTests.cpp
)

target_include_directories(memory_usage_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
)

add_test(NAME memory_usage_tests COMMAND memory_usage_tests)

add_executable(gm_note_utils_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities/GMNoteUtils.cpp
  GMNoteUtilsTests.cpp
)

target_include_directories(gm_note_utils_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
)

add_test(NAME gm_note_utils_tests COMMAND gm_note_utils_tests)

add_executable(boost_asio_utils_tests
  BoostAsioUtilsTests.cpp
)

target_include_directories(boost_asio_utils_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Network
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_utils_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_utils_tests Boost::boost)
endif()

add_test(NAME boost_asio_utils_tests COMMAND boost_asio_utils_tests)

add_executable(boost_asio_work_tests
  BoostAsioWorkTests.cpp
)

target_include_directories(boost_asio_work_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_work_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_work_tests Boost::boost)
endif()

add_test(NAME boost_asio_work_tests COMMAND boost_asio_work_tests)

add_executable(boost_asio_write_queue_tests
  BoostAsioWriteQueueTests.cpp
)

target_include_directories(boost_asio_write_queue_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Network
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_write_queue_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_write_queue_tests Boost::boost)
endif()

add_test(NAME boost_asio_write_queue_tests COMMAND boost_asio_write_queue_tests)

add_executable(boost_asio_executor_tests
  BoostAsioExecutorTests.cpp
)

target_include_directories(boost_asio_executor_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_executor_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_executor_tests Boost::boost)
endif()

add_test(NAME boost_asio_executor_tests COMMAND boost_asio_executor_tests)

add_executable(boost_asio_thread_group_tests
  BoostAsioThreadGroupTests.cpp
)

target_include_directories(boost_asio_thread_group_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_thread_group_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_thread_group_tests Boost::boost)
endif()

target_link_libraries(boost_asio_thread_group_tests Threads::Threads)

add_test(NAME boost_asio_thread_group_tests COMMAND boost_asio_thread_group_tests)

add_executable(boost_asio_thread_tests
  BoostAsioThreadTests.cpp
)

target_include_directories(boost_asio_thread_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_thread_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_thread_tests Boost::boost)
endif()

target_link_libraries(boost_asio_thread_tests Threads::Threads)

add_test(NAME boost_asio_thread_tests COMMAND boost_asio_thread_tests)

add_executable(boost_asio_task_runner_tests
  BoostAsioTaskRunnerTests.cpp
)

target_include_directories(boost_asio_task_runner_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(boost_asio_task_runner_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(boost_asio_task_runner_tests Boost::boost)
endif()

target_link_libraries(boost_asio_task_runner_tests Threads::Threads)

add_test(NAME boost_asio_task_runner_tests COMMAND boost_asio_task_runner_tests)

add_executable(delay_executor_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading/DelayExecutor.cpp
  DelayExecutorTests.cpp
)

target_include_directories(delay_executor_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(delay_executor_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(delay_executor_tests Boost::boost)
endif()

target_link_libraries(delay_executor_tests Threads::Threads)

add_test(NAME delay_executor_tests COMMAND delay_executor_tests)

add_executable(time_utils_tests
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities/ServerRestartSchedule.cpp
  TimeUtilsTests.cpp
)

target_include_directories(time_utils_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
)

add_test(NAME time_utils_tests COMMAND time_utils_tests)

add_executable(sfmt_random_tests
  SfmtRandomTests.cpp
)

target_include_directories(sfmt_random_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/dep/SFMT
)

target_link_libraries(sfmt_random_tests SFMT)

add_test(NAME sfmt_random_tests COMMAND sfmt_random_tests)

add_executable(sfsoap_service_tests
  SFSoapServiceTests.cpp
)

target_include_directories(sfsoap_service_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/worldserver/SFSoap
  ${Boost_INCLUDE_DIRS}
)

if(TARGET Boost::headers)
  target_link_libraries(sfsoap_service_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(sfsoap_service_tests Boost::boost)
endif()

target_link_libraries(sfsoap_service_tests Threads::Threads)

add_test(NAME sfsoap_service_tests COMMAND sfsoap_service_tests)

add_executable(world_socket_session_state_tests
  WorldSocketSessionStateTests.cpp
)

target_include_directories(world_socket_session_state_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
)

add_test(NAME world_socket_session_state_tests COMMAND world_socket_session_state_tests)

add_executable(player_dump_schema_tests
  PlayerDumpSchemaTests.cpp
)

target_include_directories(player_dump_schema_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/game/Tools
)

target_compile_definitions(player_dump_schema_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME player_dump_schema_tests COMMAND player_dump_schema_tests)

add_executable(spell_book_packet_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player/SpellBookPackets.cpp
  GameDomainTestSupport.cpp
  SpellBookPacketTests.cpp
)

target_include_directories(spell_book_packet_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol
)

add_test(NAME spell_book_packet_tests COMMAND spell_book_packet_tests)

add_executable(movement_opcode_table_tests
  MovementOpcodeTableTests.cpp
)

target_compile_definitions(movement_opcode_table_tests PRIVATE SKYFIRE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME movement_opcode_table_tests COMMAND movement_opcode_table_tests)

add_executable(spell_charge_persistence_tests
  SpellChargePersistenceTests.cpp
)

target_include_directories(spell_charge_persistence_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells
)

add_test(NAME spell_charge_persistence_tests COMMAND spell_charge_persistence_tests)

add_executable(allow_only_ability_cast_tests
  AllowOnlyAbilityCastTests.cpp
)

target_include_directories(allow_only_ability_cast_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells
)

add_test(NAME allow_only_ability_cast_tests COMMAND allow_only_ability_cast_tests)

add_executable(paladin_spell_calculation_tests
  PaladinSpellCalculationTests.cpp
)

target_include_directories(paladin_spell_calculation_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Spells
)

add_test(NAME paladin_spell_calculation_tests COMMAND paladin_spell_calculation_tests)

add_executable(pvp_resilience_tests
  PvpResilienceTests.cpp
)

target_include_directories(pvp_resilience_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Combat
)

add_test(NAME pvp_resilience_tests COMMAND pvp_resilience_tests)

add_executable(pvp_toggle_handler_tests
  PvpToggleHandlerTests.cpp
)

target_compile_definitions(pvp_toggle_handler_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME pvp_toggle_handler_tests COMMAND pvp_toggle_handler_tests)

add_executable(pvp_power_tests
  PvpPowerTests.cpp
)

target_include_directories(pvp_power_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Combat
  ${CMAKE_SOURCE_DIR}/src/server/game/Miscellaneous
)

add_test(NAME pvp_power_tests COMMAND pvp_power_tests)

add_executable(paladin_script_registration_tests
  PaladinScriptRegistrationTests.cpp
)

target_include_directories(paladin_script_registration_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
)

target_compile_definitions(paladin_script_registration_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME paladin_script_registration_tests COMMAND paladin_script_registration_tests)

add_executable(hunter_improved_serpent_sting_tests
  HunterImprovedSerpentStingTests.cpp
)

target_include_directories(hunter_improved_serpent_sting_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
)

target_compile_definitions(hunter_improved_serpent_sting_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME hunter_improved_serpent_sting_tests COMMAND hunter_improved_serpent_sting_tests)

add_executable(talent_cooldown_removal_tests
  TalentCooldownRemovalTests.cpp
)

target_compile_definitions(talent_cooldown_removal_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME talent_cooldown_removal_tests COMMAND talent_cooldown_removal_tests)

add_executable(death_knight_anti_magic_zone_tests
  DeathKnightAntiMagicZoneTests.cpp
)

target_include_directories(death_knight_anti_magic_zone_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
)

target_compile_definitions(death_knight_anti_magic_zone_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME death_knight_anti_magic_zone_tests COMMAND death_knight_anti_magic_zone_tests)

add_executable(death_knight_scent_of_blood_tests
  DeathKnightScentOfBloodTests.cpp
)

target_include_directories(death_knight_scent_of_blood_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
)

target_compile_definitions(death_knight_scent_of_blood_tests PRIVATE SKYFIRE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_test(NAME death_knight_scent_of_blood_tests COMMAND death_knight_scent_of_blood_tests)

add_executable(battle_pet_packet_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePet/BattlePet.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePet/BattlePetPackets.cpp
  GameDomainTestSupport.cpp
  BattlePetPacketTests.cpp
)

target_include_directories(battle_pet_packet_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePet
  ${CMAKE_SOURCE_DIR}/src/server/game/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol
)

add_test(NAME battle_pet_packet_tests COMMAND battle_pet_packet_tests)

add_executable(gossip_schema_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals/GossipSchema.cpp
  GossipSchemaTests.cpp
)

target_include_directories(gossip_schema_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals
)

add_test(NAME gossip_schema_tests COMMAND gossip_schema_tests)

add_executable(legacy_transport_support_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps/LegacyTransportSupport.cpp
  LegacyTransportSupportTests.cpp
)

target_include_directories(legacy_transport_support_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps
)

add_test(NAME legacy_transport_support_tests COMMAND legacy_transport_support_tests)

add_executable(pet_transport_support_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Transport/PetTransportSupport.cpp
  PetTransportSupportTests.cpp
)

target_include_directories(pet_transport_support_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Transport
)

add_test(NAME pet_transport_support_tests COMMAND pet_transport_support_tests)

add_executable(loot_count_tests
  LootCountTests.cpp
)

target_include_directories(loot_count_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/game/Loot
)

add_test(NAME loot_count_tests COMMAND loot_count_tests)

add_executable(loot_packet_encoding_tests
  LootPacketEncodingTests.cpp
)

target_include_directories(loot_packet_encoding_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/game/Loot
)

add_test(NAME loot_packet_encoding_tests COMMAND loot_packet_encoding_tests)

add_executable(loot_object_guid_tests
  LootObjectGuidTests.cpp
)

target_include_directories(loot_object_guid_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
)

add_test(NAME loot_object_guid_tests COMMAND loot_object_guid_tests)

add_executable(darkmoon_shooting_gallery_tests
  DarkmoonShootingGalleryTests.cpp
)

target_include_directories(darkmoon_shooting_gallery_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Events/darkmoonfaire
)

add_test(NAME darkmoon_shooting_gallery_tests COMMAND darkmoon_shooting_gallery_tests)

add_executable(darkmoon_ring_toss_tests
  DarkmoonRingTossTests.cpp
)

target_include_directories(darkmoon_ring_toss_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Events/darkmoonfaire
)

add_test(NAME darkmoon_ring_toss_tests COMMAND darkmoon_ring_toss_tests)

add_executable(darkmoon_tonk_challenge_tests
  DarkmoonTonkChallengeTests.cpp
)

target_include_directories(darkmoon_tonk_challenge_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Events/darkmoonfaire
)

add_test(NAME darkmoon_tonk_challenge_tests COMMAND darkmoon_tonk_challenge_tests)

add_executable(darkmoon_cannon_tests
  DarkmoonCannonTests.cpp
)

target_include_directories(darkmoon_cannon_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Events/darkmoonfaire
)

add_test(NAME darkmoon_cannon_tests COMMAND darkmoon_cannon_tests)

add_executable(darkmoon_faire_tests
  DarkmoonFaireTests.cpp
)

target_include_directories(darkmoon_faire_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/scripts/Events/darkmoonfaire
)

add_test(NAME darkmoon_faire_tests COMMAND darkmoon_faire_tests)

add_executable(game_event_schedule_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Events/GameEventSchedule.cpp
  GameEventScheduleTests.cpp
)

target_include_directories(game_event_schedule_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/game/Events
)

add_test(NAME game_event_schedule_tests COMMAND game_event_schedule_tests)

add_executable(game_domain_tests
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player/PlayerRestState.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/Auras/SpellAuraMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellCalculations.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellCombatMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellEffectMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellItemMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellMovementMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellSummonMetadata.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/SpellTargeting.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Diagnostics/RuntimeMetrics.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps/MapLifecycle.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps/LegacyTransportSupport.cpp
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals/ObjectAccessorLifecycle.cpp
  ${CMAKE_SOURCE_DIR}/src/server/worldserver/WorldThread/WorldShutdownLifecycle.cpp
  GameDomainTestSupport.cpp
  GameDomainTests.cpp
)

target_include_directories(game_domain_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/src/server/collision
  ${CMAKE_SOURCE_DIR}/src/server/collision/Management
  ${CMAKE_SOURCE_DIR}/src/server/collision/Models
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/game
  ${CMAKE_SOURCE_DIR}/src/server/game/Accounts
  ${CMAKE_SOURCE_DIR}/src/server/game/Addons
  ${CMAKE_SOURCE_DIR}/src/server/game/Achievements
  ${CMAKE_SOURCE_DIR}/src/server/game/AI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/CoreAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/ScriptedAI
  ${CMAKE_SOURCE_DIR}/src/server/game/AI/SmartScripts
  ${CMAKE_SOURCE_DIR}/src/server/game/BattlePet
  ${CMAKE_SOURCE_DIR}/src/server/game/Chat
  ${CMAKE_SOURCE_DIR}/src/server/game/Combat
  ${CMAKE_SOURCE_DIR}/src/server/game/Conditions
  ${CMAKE_SOURCE_DIR}/src/server/game/DataStores
  ${CMAKE_SOURCE_DIR}/src/server/game/Diagnostics
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Item/Container
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Pet
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Transport
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/server/game/Entities/Vehicle
  ${CMAKE_SOURCE_DIR}/src/server/game/Globals
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/server/game/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/server/game/Groups
  ${CMAKE_SOURCE_DIR}/src/server/game/Guilds
  ${CMAKE_SOURCE_DIR}/src/server/game/Handlers
  ${CMAKE_SOURCE_DIR}/src/server/game/Instances
  ${CMAKE_SOURCE_DIR}/src/server/game/Maps
  ${CMAKE_SOURCE_DIR}/src/server/game/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/MovementGenerators
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Spline
  ${CMAKE_SOURCE_DIR}/src/server/game/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/server/game/Opcodes
  ${CMAKE_SOURCE_DIR}/src/server/game/Quests
  ${CMAKE_SOURCE_DIR}/src/server/game/Scripting
  ${CMAKE_SOURCE_DIR}/src/server/game/Server
  ${CMAKE_SOURCE_DIR}/src/server/game/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells
  ${CMAKE_SOURCE_DIR}/src/server/game/Spells/Auras
  ${CMAKE_SOURCE_DIR}/src/server/game/Tools
  ${CMAKE_SOURCE_DIR}/src/server/game/World
  ${CMAKE_SOURCE_DIR}/src/server/worldserver/WorldThread
  ${Boost_INCLUDE_DIRS}
  ${MYSQL_INCLUDE_DIR}
)

add_test(NAME game_domain_tests COMMAND game_domain_tests)

add_executable(cluster_foundation_tests ClusterFoundationTests.cpp)
target_include_directories(cluster_foundation_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME cluster_foundation_tests COMMAND cluster_foundation_tests)

add_executable(handoff_store_tests HandoffStoreTests.cpp)
target_include_directories(handoff_store_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME handoff_store_tests COMMAND handoff_store_tests)

add_executable(handoff_client_tests HandoffClientTests.cpp ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/HandoffClient.cpp)
target_include_directories(handoff_client_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(handoff_client_tests Threads::Threads ${OPENSSL_LIBRARIES})
if(TARGET Boost::headers)
  target_link_libraries(handoff_client_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(handoff_client_tests Boost::boost)
endif()
if(WIN32)
  target_link_libraries(handoff_client_tests ws2_32 mswsock)
endif()
add_test(NAME handoff_client_tests COMMAND handoff_client_tests)

add_executable(hub_auth_proxy_tests
  HubAuthProxyTests.cpp
  ${CMAKE_SOURCE_DIR}/src/server/hub/HubAuthProxy.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets/PacketLogServer.cpp
  PacketLogServerConfigStub.cpp
  RealmSocketLogStub.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets/ByteBuffer.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging/Errors.cpp
)
target_include_directories(hub_auth_proxy_tests PRIVATE
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/hub
  ${Boost_INCLUDE_DIRS}
)
if(TARGET Boost::headers)
  target_link_libraries(hub_auth_proxy_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(hub_auth_proxy_tests Boost::boost)
endif()
target_link_libraries(hub_auth_proxy_tests Threads::Threads)
if(WIN32)
  target_link_libraries(hub_auth_proxy_tests ws2_32 mswsock)
endif()
add_test(NAME hub_auth_proxy_tests COMMAND hub_auth_proxy_tests)
set_tests_properties(hub_auth_proxy_tests PROPERTIES TIMEOUT 60)

add_executable(realm_directory_tests RealmDirectoryTests.cpp)
target_include_directories(realm_directory_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
target_link_libraries(realm_directory_tests Threads::Threads)
add_test(NAME realm_directory_tests COMMAND realm_directory_tests)

add_executable(hub_control_protocol_tests HubControlProtocolTests.cpp)
target_include_directories(hub_control_protocol_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/hub ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME hub_control_protocol_tests COMMAND hub_control_protocol_tests)

add_executable(hub_standby_control_tests HubStandbyControlTests.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Platform/HubProcessControl.cpp)
target_include_directories(hub_standby_control_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
target_link_libraries(hub_standby_control_tests Threads::Threads)
add_test(NAME hub_standby_control_tests COMMAND hub_standby_control_tests)

add_executable(realm_directory_agent_probe
  RealmDirectoryAgentProbe.cpp RealmSocketLogStub.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/ClusterAgent.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/HandoffClient.cpp)
target_include_directories(realm_directory_agent_probe PRIVATE
  ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(realm_directory_agent_probe Threads::Threads ${OPENSSL_LIBRARIES})
if(TARGET Boost::headers)
  target_link_libraries(realm_directory_agent_probe Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(realm_directory_agent_probe Boost::boost)
endif()
if(WIN32)
  target_link_libraries(realm_directory_agent_probe ws2_32.lib mswsock.lib)
endif()

# Optional real-MySQL fixture; ordinary CTest runs skip without explicit test credentials.
add_executable(hub_handoff_store_tests HubHandoffStoreTests.cpp
  ${CMAKE_SOURCE_DIR}/src/server/hub/HubHandoffStore.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/HandoffClient.cpp)
target_include_directories(hub_handoff_store_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/src/server/hub ${CMAKE_SOURCE_DIR}/src/server/shared
  ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR})
target_link_libraries(hub_handoff_store_tests Threads::Threads ${OPENSSL_LIBRARIES} ${MYSQL_LIBRARY})
if(TARGET Boost::headers)
  target_link_libraries(hub_handoff_store_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(hub_handoff_store_tests Boost::boost)
endif()
if(WIN32)
  target_link_libraries(hub_handoff_store_tests ws2_32 mswsock)
endif()
add_test(NAME hub_handoff_store_tests COMMAND hub_handoff_store_tests)
set_tests_properties(hub_handoff_store_tests PROPERTIES SKIP_RETURN_CODE 77)
add_executable(map_data_directory_tests MapDataDirectoryTests.cpp)
target_include_directories(map_data_directory_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME map_data_directory_tests COMMAND map_data_directory_tests)

add_executable(chat_protocol_tests ChatProtocolTests.cpp)
target_include_directories(chat_protocol_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME chat_protocol_tests COMMAND chat_protocol_tests)

add_executable(chat_presence_tests ChatPresenceTests.cpp)
target_include_directories(chat_presence_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME chat_presence_tests COMMAND chat_presence_tests)

add_executable(chat_whisper_tests ChatWhisperTests.cpp)
target_include_directories(chat_whisper_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared)
add_test(NAME chat_whisper_tests COMMAND chat_whisper_tests)

# Certificate utilities are also used by standalone transport fixtures.
foreach(pki_fixture handoff_client_tests realm_directory_agent_probe hub_handoff_store_tests)
  target_sources(${pki_fixture} PRIVATE ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/CertificateTools.cpp)
  if(WIN32)
    target_link_libraries(${pki_fixture} advapi32)
  endif()
endforeach()

add_executable(hub_certificate_tests HubCertificateTests.cpp RealmSocketLogStub.cpp
  ${CMAKE_SOURCE_DIR}/src/server/hub/HubCertificates.cpp
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cluster/CertificateTools.cpp)
target_include_directories(hub_certificate_tests PRIVATE
  ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/server/hub ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
target_link_libraries(hub_certificate_tests Threads::Threads ${OPENSSL_LIBRARIES})
if(TARGET Boost::headers)
  target_link_libraries(hub_certificate_tests Boost::headers)
elseif(TARGET Boost::boost)
  target_link_libraries(hub_certificate_tests Boost::boost)
endif()
if(WIN32)
  target_link_libraries(hub_certificate_tests advapi32 ws2_32 mswsock)
endif()
add_test(NAME hub_certificate_tests COMMAND hub_certificate_tests)
