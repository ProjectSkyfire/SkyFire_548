# Shared protocol and client data foundation

The initial extraction makes protocol definitions and client-data schemas reusable
without linking the game library. Gameplay dispatch and store initialization have
not moved into shared yet.

## Reusable interfaces

- `shared/Protocol/Opcodes.h` owns the existing symbolic opcode IDs, protocol
  constants, immutable metadata lookup and diagnostic name formatting.
- `ClientOpcodeCatalog.inc` and `ServerOpcodeCatalog.inc` are the authoritative
  build-18414 wire mappings. Symbolic enum IDs are not wire numbers. A zero wire
  value remains unresolved; no new client support is implied by a catalog entry.
- `shared/Packets/WorldPacket.*` owns the packet container and existing compression
  implementation. ByteBuffer was already shared. Logging no longer requires a
  world handler table to have been initialized.
- `shared/DataStores` now owns DBC/DB2 enums, record structures, format descriptors
  and item-array size constants, alongside the existing loaders/storage templates.
- `shared/Utilities/Path.h` provides the data-only path container used by taxi data.

Protocol-only consumers include `Protocol/Opcodes.h` from the shared root. Use
`GetOpcodeMetadata(symbol, isServerOpcode)` to obtain a name and wire number; a
null name denotes an entry absent from that direction's catalog. Invalid IDs are
rejected by lookup. Client, server and internal cluster messages remain separate.

Worldserver keeps `WorldOpcodeDispatch.h` and the registration implementation in
`game/Server/Protocol/Opcodes.cpp`. Registrations obtain their numeric values from
the shared catalog while retaining their original session checks, processing thread
and member-function handler. Game compatibility headers preserve old include paths.
The shared packet container does not expose world dispatch through a transitive include.

## Preserved behavior and remaining work

The extraction preserves all 1,062 registration mappings and their dispatch policy,
the symbolic enum order, schema bodies/packing, format strings, path implementation
and packet compression body. Metadata lookup is available before world startup.

`DBCStores.*`, `DB2Stores.*`, their mutable initialization, spell/transport hooks,
hotfix handling and gameplay helpers still live in game. The next slice will separate
those adapters from reusable store contexts and selected-table load profiles. This
release does not promise a standalone full-store initialization API, shared process
memory, remote DBC lookups or a running chat daemon.

## Build and verification

Regenerate the existing CMake configuration before building so the moved packet
source belongs to shared exactly once. Shared carries its zlib link dependency.
No configuration, database, extracted-data or Python-service update is required.
Build and INSTALL through the normal workflow; no live services need to be changed
just to inspect or compile this source extraction.

`src/tests/shared_foundation_test.py` checks the extraction against the prior source
baseline, including every wire mapping/dispatch tuple and unchanged schema bodies.
`shared_protocol_data_tests` is a native test target with no game include directories
or game link dependency. It checks representative opcode values, direction and
invalid lookup behavior, record sizes/offsets and packet-container availability.
Native compilation and the existing game-domain/client regression tests remain
required before deployment. Deliberate future ABI changes need an explicit baseline
update; moving files is not authorization to change packet mappings or data layouts.
