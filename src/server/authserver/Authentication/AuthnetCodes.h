/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETCODES_H
#define SF_AUTHNETCODES_H

#include "Define.h"

// Battle.net v2 header layout (BNETDocs "Battle.net v2 Information",
// matching the 18414 write primitive): 6-bit packetID, 1-bit inChannel,
// then a 4-bit channelID only when inChannel is set. Packets are a bit
// stream - leftover bits in a byte belong to the next packet rather than
// padding that should be skipped. After the plaintext handshake, RC4
// covers the payload; 18414 captures so far still encrypt the whole
// follow-up, including the header.
constexpr uint32 AUTHNET_PACKET_ID_BITS = 6;
constexpr uint32 AUTHNET_IN_CHANNEL_BITS = 1;
constexpr uint32 AUTHNET_CHANNEL_ID_BITS = 4;

// 11-bit header values from 18414 captures. C->S logon is packet 9
// channel 0. Packet 5 is only the C->S RC4 mode-switch.
namespace AuthnetWire
{
    constexpr uint32 PacketStartup = 0;
    constexpr uint32 PacketModeSwitch = 5;
    constexpr uint32 PacketLogin = 9;

    constexpr uint32 ChannelStartup = 0;
    // Channel for anything carrying a Battlenet::Event::Header. The 11-bit
    // transport is packetId(6) + inChannel(1) + channelId(4), so byte 1 bits
    // 0-2 are channelId & 7 and bits 3-6 are m_id. That decodes every control
    // frame the client emits: 4901/4909/4911/4919/4921 are all channel 1 with
    // m_id 0/1/2/3/4, and 4921 is exactly an m_id 4 Authentication event. The
    // old "byte 1 is a 1 + 8k counter" reading was that same bit field
    // misread. Probes that used ChannelStartup sent 4920 - channel 0 - which
    // is a channel the client never sends an event on.
    constexpr uint32 ChannelEvent = 1;
    // Packet 5 uses channel 1 for the RC4 mode-switch. Packet 9 uses
    // channel 1 for the 16-bit request token after Header event 0.
    constexpr uint32 ChannelEncrypt = 1;
    constexpr uint32 ChannelRequestToken = 1;
    constexpr uint32 ChannelAuthStatus = 2;
    constexpr uint32 ChannelEmptyRequest = 11;
}

// App login-socket receive byte (sub_140A7E890), after CreepTendril
// decodes Battlenet::Event::Header (type 4008) and dispatches. These
// are not the 11-bit channel id. Authentication is CreepTendril
// callback sub_140B236E0, which passes byte 5 into the login socket.
enum class AuthnetAppEvent : uint8
{
    Patch = 1,
    Authentication = 5
};

// Event::Header (type 4008) is a 4-field EncodedObject in EventProtocol
// (blob +0xCA97: ce0002fcff, types 4009-4012):
//   4009 -> 4004: kind 9, 4-bit Event::Id 0-15 (m_id)
//   4010: kind 11 optional of 4013 (m_sourceAddress)
//   4011 -> 4003: kind 8 FourCC (m_sourceName)
//   4012 -> 4005: kind 6, 1-bit Destination choice (m_destination)
// Destination variants (names Class / Address):
//   tag 0 -> 4006 -> FourCC
//   tag 1 -> 4007 -> type 1 32-bit Address
// Empty 4920 is only id 4 + sourceAddress absent. B3A4D0 leaves
// Destination::Class at -1, so B236E0 never calls A7E890. Extra
// bits after 4920 are copied as body: 162810 C-strings (0x121C),
// 164556 FourCC+Address (0x120F), 165412 two presence bits plus
// those fields (492003..., token 0x120F) — same hang, no crash,
// no failure log. 173145 sent optional sourceAddress present with
// 32-bit 0 then integer Failure 201 (49A000000000000200C900,
// 11 bytes, token 0x120B). Copied, SetLaunchOption, ping, retry
// 0x180B, ~30s reason 8. No crash, no failure log, not a new
// invoke. Optional-present is field 1; Destination::Class still
// never becomes 0/1. App byte 5 is written after dispatch.
//
// C->S logon has no Event::Header. After the 11-bit packet 9
// channel 0 header, TryDecodeInitialRequest reads FourCC WoW at
// bit 11 (value 0x00576F57). The 0x00 after 0x49 is that FourCC's
// high byte, not event 0 + optional absent. Skipping 5 Header
// bits makes program/platform/locale garbage. A7C500 sends type
// 1620 via vfunc+40(byte 6, &dword_1416108C0, object). S->C packet
// 9 always goes through Header receive: 174840 sent no-Header
// Failure 490000021901 (6 bytes, token 0x1206). First 4 leftover
// bits are event 0, so B23500 Patch ran, Class stayed -1, A78ED0
// never ran. Copy, SetLaunchOption, ping, retry 0x1806, ~31s
// reason 8. Do not send no-Header again. Next probe is event 4
// plus dest name Class (Walker kind 6, B22170 vfunc+136). 162810
// used Authentication/Failure; dest mismatch returns 1 without
// setting Class. 180822 sent dest Class immediately after 4920
// (4920436C61737300..., 17 bytes, token 0x1211). Copied, no crash,
// no failure log. Kind 8 sourceName would consume "Clas"; dest would
// see "s". 181219 sent sourceName FourCC 0 then dest Class
// (492000000000436C61737300..., 21 bytes, token 0x1215). Copied,
// no crash, no failure log. Optional was absent. Next probe is
// 49A0, sourceAddress 0, sourceName 0, dest Class, dest FourCC 0,
// Failure (49A00000000000000000436C6173730000000000000200C900).
// 181706 sent that 25-byte 49A0 plus short name Class (token 0x1219).
// Copied, no crash, no failure log. Type blob dest names are
// Destination::Class / Destination::Address, not Class. 182056 sent
// FourCC 0 then Destination::Class (34 bytes, token 0x3202). 182340
// sent Destination::Class immediately after 4920 (30 bytes, token
// 0x121E). Copied, no crash, no failure log. Type 4005 dest names are
// Class / Address. Dest Class payload is FourCC. 183804 sent 1-bit
// dest 0 after sourceName 0 (15 bytes, token 0x120F). Copied, no
// crash, no failure log. Same as 164556 dest Address. 184634 sent
// compact 4-byte event 4, 2-byte Class 0, Failure (token 0x120D).
// Copied, no crash. 185218 sent compact Class 1
// (4920040000000100000200C900, token 0x120D, retry 0x180D). Copied
// as a 13-byte body. 192222 sent dest Class FourCC WoW
// (492000000000002BB7AB0102006401, token 0x120F, retry 0x180F).
// Copied as a 15-byte body, no crash, no failure log, reason 8.
// Same leftover as 183804 FourCC 0. 193025 sent sourceName 0, 6-bit
// counted Class, dest FourCC WoW (21 bytes, token 0x1215, retry
// 0x1815). Copied as body, same as 181219 NUL Class. Dest packing
// after EncodedObject 4920 is exhausted (integer dest, C-string /
// Destination::Class / counted Class names, compact 4+2 after 4920).
// Do not send those again. 194118 sent compact-only (no EncodedObject
// 4920): 4900040000000100000200c900. Follow-up echoed event 0 under
// EncodedObject parsing (4900), token 0x120D, same hang. Packet 9
// channel 0 uses EncodedObject Header; omit 4920 and event id is 0.
// Soft kind-6 (B2ACE0) and Primitive AEDCA0 (+232) both set Class via
// AF5880 binder+24(&Header+16, 0|1) and return &Header+20 for the
// nested FourCC/Address. AED820 (+136 encode) maps Class->name; decode
// needs stream+136 to produce "Class"/"Address". 195827 identity-style
// (token 0x1216) and 192222 Class0+WoW (token 0x120F, Failure at bit
// 81) both hung with no failure log. A7E890 always calls A78ED0; if
// 2292 decode fails, *a2 stays -1 and A78ED0 returns silently — same
// symptom as Class still -1. 203715 AlignToByte after Class0+WoW then
// Failure (492000000000002bb7ab01000200c900, 16 bytes, token 0x1210,
// retry 0x1810) same hang, no failure log, reason 8. AlignToByte is
// exhausted — do not pad after Header before integer Failure. 204335
// Class0+WoW plus C-string Authentication/Failure
// (492000000000002bb7ab014175...004661696c7572650000c900, 37 bytes,
// token 0x3205, retry 0x3805) same hang, no failure log. Named C-string
// body after Class0+WoW is exhausted. 205436 Class0+WoW plus
// identity-style Authentication/Failure
// (492000000000002bb7ab0503417574...Failure00c900, 38 bytes, token
// 0x3206, retry 0x3806) same hang, no failure log, reason 8. Identity
// named body after Class0+WoW is exhausted. EncodedObject uses
// BSNPrimitiveWalker only (B21CC0→B22990→B22170): kind 6 always calls
// stream+136 for a C name, then matches the type-DB table. Type 2292
// Larva::Invoke names: Authentication, Service, Connection, Block,
// Game, Party, Ladder, Advert, MatchMaker, WoW, DarkPortal, Profile,
// S2Map, Cache, Friend, Toon, Presence, Support. Type 2293
// Authentication names: Module, Success, Failure, CachedAccount,
// CachedGameAccount, CachedToon, ModuleBegin, ModuleEnd,
// GenerateSingleSignOnTokenResponse, SetLaunchOptionResponse,
// GetLaunchOptionResponse, ClearLaunchOptionResponse. Destination
// 4005 (kind 0x86) still goes through B22170 with names Class/Address —
 // 1-bit Soft packing may never set Header.Class under PrimitiveWalker.
 // CreepTendril serializes with Hard::Encoder (bit Soft writers). Parallel
 // VersionedDecoder kind-6 is marker byte 3 + signed varint (BCA9B0 /
 // C39360), no stream+136 names. 212019 Class0+WoW Soft Header then
 // Versioned Authentication/Failure (492000000000002BB7AB010300030400C900,
 // 18 bytes, token 0x1212, retry 0x1812) same hang, no failure log,
 // reason 8. Soft Header + Versioned body exhausted. 213801 full Versioned
 // Header Failure (4900050800090802...576F570300030400C900, 31 bytes,
 // token 0x121F, retry 0x181F): follow-up Soft-parsed event=0 (not 4),
 // SetLaunchOption, reason 8, no failure log. Creep EncodedObject Header
 // is Soft/Primitive bit packing, not Versioned markers — Versioned
 // Header exhausted. Soft Header still reaches event 4; Destination
 // Class and/or Primitive 2292 body remain open. 214443 Soft Class0+WoW
 // plus Align + uint8-length-prefixed Authentication/Failure
 // (492000000000002BB7AB010E4175...074661696C75726500C900, 37 bytes,
 // token 0x3205, retry 0x3805) same hang as C-string 204335 — length-
 // prefixed names exhausted. Soft 1-bit Destination does not set Class
 // (183804 Class0+FourCC0: no B1B310 crash). Soft Class0+WoW body probes
 // were unreachable. 214735 Soft sourceName0 + uint8-len Class + WoW +
 // Soft Failure (49200000000005436C61737300576F57000200C900, 21 bytes,
 // token 0x1215, retry 0x1815) same hang — uint8-len Class exhausted.
 // 215252–215712 rotated all 10 battery probes (initial + 0x18xx retry
 // each launch): u16/7-bit/raw/identity/Align-CString Class names,
 // Soft Class1 Address 0/WoW, Soft Class0+WoW ± Align Failure. Every
 // session: SetLaunchOption, reason 8, no failure log. Soft Header dest
 // name guesses exhausted. IDA local debug: Soft Class0+WoW sets Class=0,
 // A7E890 runs (byte 5) and IS the 2292 decode site: it decodes type 2292
 // with BSNPrimitiveEncoder into the GLOBAL dword_1415BA200 (base+0x15BA200)
 // and calls A78ED0(conn, dword_1415BA200). So RDX at A78ED0 is the decoded
 // Invoke, but the buffer is a persistent global seeded to -1 once, so a
 // failed/partial decode leaves stale values there — always read AL right
 // after the decode call instead of trusting the buffer alone.
 // A78ED0 entry dispatch on the decoded struct:
 //   [rdx+0] = Invoke choice: 0 -> Authentication (A79495), 1 -> Service
 //             (A7940F), 9 -> third branch, else default
 //   [rdx+4] = sub-tag; on the Service path it is switched (valid 2..7) into
 //             login failure reasons (tag 2 -> 0x0B), which is exactly the
 //             {1,2} + "Invoke::Service::Lost" we kept seeing.
 // Header Class=0 verified live (RDX+0x10 reads 00 00 00 00 at B236E0).
 // B241A0 confirmed hit live: it is the inbound decoder for
 // BSNEncodedObject<Battlenet::Event::Header,EventProtocol> (v19 = 4008,
 // schema unk_141610820) using BSNPrimitiveEncoder. Layout:
 //   B24283  call [rbp+108h]      -> Header decode
 //   B24289  test al, al          -> al = decode ok
 //   B2428B  jz B242BD            -> failure: AEC060(unk_140E93747) assert
 //   B2428D  movzx eax,[rsp+40h]  -> event id byte (var_88)
 //   B242B3  call [r13+r9*8+B28h] -> per-event dispatch
 // Live 001300: AL = 1 at B24289 (Header decode SUCCEEDS) and the event id
 // byte (EAX after B2428D) is 5, although the probe writes 4 in the 4-bit
 // event field (verified: byte1 = 0x20 is exactly WriteBits(4,4) at bit 11).
 // Either slot 5 is the Authentication handler (then Header is fine and the
 // 2292 body is the only open item) or this decode is not our packet.
 // Slot 5 is confirmed Authentication anyway: B236E0 (the Destination router,
 // Class 0 -> forward with app byte 5) is reached, so the Header path is done.
 // Open item is purely the 2292 body. Live 003900 at A7E9A3 (BP right after
 // "call [rbp+108h]" in A7E890): AL = 1, so the 2292 body DOES decode, and
 // dword_1415BA200 read fresh at that instant = {1,2} -> Invoke choice
 // Service, sub-tag 2. Marker tags 7/3 with the LSB0 FourCC
 // (…2BB7AB00070300C900) left the buffer at {1,2}, but that FourCC is
 // 0x00576F56 = "WoV", not a registered subscriber, so B236E0 may have
 // dropped the event before decode — LSB0 FourCC is therefore retired.
 // RESOLVED 005600: the A7E9A3 breakpoint fires ~30 SECONDS after login, not
 // on arrival of our probe, and still reports AL=1 with {1,2}. That call is
 // the client's own Service::Lost teardown at the connect timeout (hence the
 // R8/R10 "Invoke::Service::Lost" string), NOT a decode of our packet.
 // Everything then falls into one consistent story: event slot 5 is SERVICE,
 // not Authentication; B236E0 is the Service router; Invoke choice 1 is
 // Service and sub-tag 2 is Lost. So "event 4 -> Authentication -> app byte
 // 5" was wrong, and A7E890 / A78ED0 / B236E0 / B24289 observations were all
 // the timeout path. No live evidence yet that our probe is decoded at all.
 // CONFIRMED 005800: B24289 (live 7FF66D894289) also fires ONLY at the 30s
 // timeout, never on probe arrival. So no inbound Creep event decode happens
 // for our bytes at all: the client buffers them and never parses them. Every
 // Header/Destination/body encoding result recorded above was measured on the
 // timeout path and proves nothing about our packet. STOP guessing bodies.
 // Send/read paths in RealmSocket are separate (_inputBuffer from
 // async_read_some vs QueueSend's own copy), so the peer genuinely mirrors
 // our exact probe bytes inside its next packet — not a logging artifact.
 // Two directions, in order:
 // 1. IN PROGRESS: transport may never be "open", so channel-0 events are
 //    dropped. 134616 refused to echo the 4001 packet-0 channel-1 ping
 //    because it "opened a new invoke", but that echo may be the required
 //    handshake. Now on first ping we send 4001 (byte-identical to the
 //    client's own ping) and then immediately resend the marker event, so a
 //    single login shows before/after. B24289 firing immediately after the
 //    ack would prove the transport gate was the bug.
 //    RESULT 010500: ping ack changed nothing. B24289 still only fires at the
 //    timeout, so the ping is not the transport gate. Direction 1 closed.
 // 2. THE ECHO IS THE REAL SIGNAL. Peer verified as Wow-64 itself
 //    (Get-NetTCPConnection -RemotePort 1119 -> pid = Wow-64), not a proxy or
 //    the launcher, and RealmSocket keeps read/write fully separate
 //    (_inputBuffer only ever filled from async_read_some; QueueSend copies
 //    into its own _writeQueue), so the mirroring is genuinely client-sent.
 //    After acking the ping, one client frame came back as:
 //      49013202 <our 16-byte marker> 4001 <our 16-byte marker> 930039D54A04
 //    i.e. it quotes EVERY byte we sent, in order, including the ping ack.
 //    Best reading: this is a transport-level NACK/bounce that quotes the
 //    offending packet ("no subscriber / unroutable"), not a
 //    SetLaunchOptionRequest — that decode was us pattern-matching our own
 //    echoed bytes plus a 6-byte trailer 9300<u32>, where the u32 has the
 //    high bit set (0x8006635E, 0x80073A21, 0x80073AAA) and changes per
 //    frame, so it looks like a timestamp or error code.
 //    Consequences: the "SetLaunchOptionRequest / tag 9 / Creep trailer"
 //    interpretation in CollectCreepFollowUpAcks is suspect, and the packet is
 //    being rejected at ROUTING (Destination subscriber), before event decode.
 //    ECHO IS CUMULATIVE: each client frame = 4 real bytes + EVERY byte we
 //    have sent so far, in order + a 5-6 byte tail (26 = 4+16+6,
 //    44 = 4+16+2+16+6, 43 = 4+16+2+16+5). Read path audited and airtight
 //    (_readBuffer is only the async_read_some target, _inputBuffer only
 //    inserted from it, _captured only via ReadBytes), and only ONE Wow
 //    process exists (pid matched the socket owner), so this is real client
 //    behaviour on the right process. Best explanation: the client cannot
 //    parse our FIRST byte, so our bytes are never consumed from its receive
 //    buffer, and every report quotes the whole unconsumed buffer - which is
 //    why it grows. The rejection is at the very start of our stream, not in
 //    the Header/Destination/body.
 //    Client tail bytes decode oddly under our bit convention: 0x93 -> packet
 //    id 19 not in channel, 0x02 -> packet id 2 not in channel, while the
 //    4-byte prefix is a clean packet 9 / channel 1 / event 0 / token.
 //    Note PacketPipe_CliSvr (A7ED40) frames as 4-byte header with
 //    length = hdr >> 13 and id = hdr & 0x1FFF, total = payload + 4. That does
 //    NOT fit the Battle.net stream (client's own 2-byte 4001 ping), so it is
 //    probably the world/game pipe - but S->C on 1119 may still want a length
 //    prefix we never send.
 //    SILENT BASELINE 011700: with zero server bytes the client sends exactly
 //    100 bytes and closes - logon(89), 4001 ping(2), 4901 1800 02D4CD7D00(9).
 //    Probes change that, so the client does read our data.
 // BREAKTHROUGH 012100: sending ONLY the 4001 ping ack (nothing on initial
 // logon) was ACCEPTED - no echo, no 9300 frame. The client then sent a second
 // 4001 and 25 bytes with a 14-byte payload never seen before:
 //   4901 0127 3AB602D651D3000142B7B7370000 1800 02D68E5F00
 // Conclusions:
 //   - S->C bytes are accepted at the transport level; framing was never the
 //     problem, and the 9300 frames were bounces of malformed/early packets.
 //   - The 4001 ping ack is part of the handshake and the client keeps pinging,
 //     so ack every one.
 //   - ORDER IS CRITICAL: sending anything BEFORE the client's first ping puts
 //     it into bounce mode for the rest of the session. That is what ruined the
 //     earlier ping-ack attempt and, most likely, EVERY probe in the whole
 //     history above - they were all sent on initial logon.
 // 012500 (silent logon + ack every ping + one event after the first ack):
 // the client NO LONGER DISCONNECTS. It stays on CONNECTING and runs a stable
 // ping loop for as long as we keep acking, instead of dying at 30s. Best
 // state reached so far. It is now clearly a RELIABLE transport waiting for
 // ACKs we never send: each frame retransmits the entire unacked backlog, so
 // frames grow 44, 62, 80, 98, 116, 134, 152, 170 bytes (+18 per ping cycle).
 // Frame layout, all cumulative:
 //   [49 <seq> <token16>] [N x 16-byte client unit] [12XX] [our marker bytes]
 //   [our 4001 acks, one per cycle] [0000] [9300 <u32>]
 // Client unit (repeats, one added per cycle):
 //   0027 3AB602 <tstamp3> 0001 42B7B737 0000
 //   tstamp3 steps ~0x1F40 (8000) per cycle: D7A3DE, D7C326, D7E266, D8019E,
 //   D820DE, D84026, D85F5E, D87EAD. Note 42B7B737 resembles our own 2BB7AB
 //   (FourCC WoW at a different bit offset), so the unit likely carries a
 //   FourCC plus a timestamp.
 // Prefix bytes: 49 01|09|11|19|21 ... i.e. byte1 = 1 + 8*k, so what the old
 // decoder called "event" is really byte1 >> 3 = a monotonic counter that
 // increments every second frame, and "token" alternates 0x0127 / 0x0027
 // (differing only in bit 0x100 - a flag). The 6-bit packetId / inChannel /
 // 4-bit channel model does NOT describe these control frames.
 // The trailing 12XX before our echoed bytes also steps 1212, 1214, 1216,
 // 1218, 121A, 121C, 121E, 3200 - +2 per cycle, so it is a window/ack counter.
 // 013100 CONTROL TEST (ping acks only, event probe held): the growing frames
 // VANISH. Session 51555 was 4 clean 4001/4001 cycles, 97 bytes total, no 49xx
 // frame, no 9300 tail, no backlog, and the mirrored control ack never even had
 // a control frame to answer. So the retransmit-needs-an-ACK theory is WRONG:
 // the backlog was the client NACKing OUR 16-byte marker event over and over,
 // re-reporting it with one 16-byte unit appended per cycle. The transport is
 // healthy on its own; the marker event body is what it refuses to route.
 // This finally gives a clean, trustworthy discriminator, which is what every
 // earlier body test lacked:
 //   silent logon -> ack every ping -> send ONE event -> watch the next frame.
 //   49xx + 9300 tail  = that encoding was rejected.
 //   clean 4001 loop   = that encoding was accepted; move to the next field.
 // Caveat: with pure ping acks the client closes after ~4 cycles (it still
 // needs a real logon answer), while the NACKed marker kept it alive longer.
 // So judge a variant on bounce-vs-clean in the FIRST frame after the event,
 // not on how long the session lives.
 // The mirrored control ack (AuthnetAckControlFrames) is kept but is dead code
 // while we behave; it only fires if a real 49xx control frame ever arrives.
 // 013700: acking every ping keeps the session alive INDEFINITELY - session
 // 54736 ran 5+ cycles with no close and sent nothing but 4001. So 51555's
 // close was the user retrying, not a client timeout, and the client is simply
 // idle-waiting for the server to speak. The 4901 1800 <tick> 00 packet only
 // appears in fully-silent runs as the client's farewell before closing, so
 // there is no live shape to mirror and the tick mirror cannot be triggered
 // while we behave. We have to send an event; the only question is its bytes.
 // FRAMING, from every packet the client emits:
 //   byte0 = 0x4X, high nibble constant 4, low nibble = packet id
 //           4900 logon, 4001 ping, 4901 tick, 4909/4911/4919/4921 control
 //   byte1 = 0 on the logon, and 1 + 8k on every control frame - i.e. a
 //           counter in the upper bits with bit 0 SET
 // Our probes have always sent 4920: counter 4 with bit 0 CLEAR, a byte pair
 // the client never produces. That is a one-bit, evidence-backed suspect and
 // it was present in EVERY body variant ever tested, which would explain why
 // no body ever mattered.
 // 014000 RESULT: byte1 = 0x01 STILL BOUNCES. The prefix bit is not the gate,
 // so 0x21 is not worth a login either. What the run did buy is a full decode
 // of the bounce frame, which is a SERVER->CLIENT BYTE REPORT, not a NACK of
 // one packet:
 //   [49 <1+8k> <token>] [N x 16-byte client unit] [12|72 <len>]
 //   [verbatim copy of EVERY byte we have sent, in order] [0000] [93 <b> <u32>]
 // The length byte is confirmed arithmetic, not a guess: marker(16)+ping(2)=18
 // =0x12 -> 1212; +4 = 22 = 0x16 -> 1216; 26 -> 121A; 30 -> 121E; then the tag
 // shifts (3202, 7212, 7216, 721A) once the count needs more bits. The block
 // after it was literally our marker followed by each control ack in send
 // order, so the client is logging the whole S->C stream back at us.
 // ALSO: acking those frames is a FEEDBACK LOOP - each ack is unroutable, so it
 // joins the report, which produces another frame, which we ack. Tick deltas
 // fell from ~8008 to ~8 (8ms, back to back) and the 8192 byte capture cap was
 // hit in seconds. AuthnetAckControlFrames is now OFF permanently.
 // STATE OF PLAY: exactly one routable server packet is known, 4001. Anything
 // with a body is reported. Body encodings, tag values, FourCCs and now the
 // framing prefix have all been swept blind without a single acceptance, so
 // STOP GUESSING. Read the client's S->C parser instead: B241A0 (header
 // decode) and B236E0 (dispatch) are already located, and idalib can decompile
 // them statically without live debugging.
 // 014500 STATIC DECOMPILE of B241A0 (idalib, session wow548b) - the S->C
 // header is NOT hand-rolled bit fields, which is why 20+ blind variants all
 // failed:
 //   B241A0 builds BSN::BSNEncodedObject<Battlenet::Event::Header,EventProtocol>
 //   -> BSN::BSNPrimitiveEncoder vtable 140FB6A38, slot +264 = 140AED0C0
 //   -> 140AED0C0 builds BSN::BSNPrimitiveWalker and calls 140B21CC0
 // So decoding is a GENERIC SCHEMA-DRIVEN WALK, not a fixed layout. Two facts
 // fall straight out of it:
 //   1. 140AED0C0 returns false immediately when *(*a4 + 56) == 0, i.e. when
 //      the schema pointer is null. A schema object must be resolved first.
 //   2. B241A0 dispatches on the FIRST decoded field:
 //        (*(a1 + 16*hdr[0] + 2856))(a1 + *(int*)(a1 + 16*(hdr[0]+179)) - 56,
 //                                   hdr, a3)
 //      hdr is a 12-byte struct; only hdr[0] and hdr[4] are pre-zeroed, plus
 //      two ints, the second initialised to -1. hdr[0] is a HANDLER INDEX into
 //      a table at a1+2856 - get it wrong and nothing routes, which matches
 //      every bounce we have seen.
 // Dead end noted: unk_141610820 is a shared global with 100+ xrefs and is all
 // zeros statically, so it is not the Header schema and v19=4008 is just a size.
 // 020000 BSN TYPE REGISTRY CRACKED. Live dump at the decode call site
 // (static 0x140B24283, `call qword ptr [rbp+108h]` in B241A0, R9 = the
 // encoder arg) gave R9 -> obj = unk_141610820 with module base 7FF7F9640000,
 // i.e. obj IS the global registry. The value at +0x38 (0x27102) is NOT a
 // schema pointer, just a registry field, so the earlier "null schema" read
 // was wrong. Real resolution, from sub_140B21800 + sub_140AEC4F0:
 //   registry+0x00 blobSize   = 0x3A2BA
 //   registry+0x08 blobBase   = 0x140F773E0
 //   registry+0x28 typeCount  = 0x12FB (4859)
 //   registry+0x40 indexTable = 0x140F773F4  (= blobBase + 0x14)
 //   descriptor = blobBase + BIG_ENDIAN_U32(indexTable + 4 * typeId)
 //   kind       = descriptor[0] & 0x3F        (1..0x10, walker sub_140B22990)
 // Everything is STATIC .rdata, so the whole schema is readable with idalib -
 // no debugger needed from here on. Descriptor length = next index entry minus
 // this one, which makes every record self-delimiting.
 // Event::Header type id = 4008, from dword_140FB1740 (static value 0x0FA8).
 //   4008 @ +0xCA97, 21 bytes: CE | 00 02 FC FF | 0FA9 0FAA 0FAB 0FAC
 //     kind 0x0E, FOUR fields - matches the 4-member struct B241A0 builds
 //     (byte, byte, int, int preset to -1).
 //   field 0: 4009 = 10 00 00 0F A4  kind 0x10 -> 4004
 //   field 1: 4010 = 0B 00 00 0F AD  kind 0x0B -> 4013 -> 4002
 //   field 2: 4011 = 10 00 00 0F A3  kind 0x10 -> 4003
 //   field 3: 4012 = 10 00 00 0F A5  kind 0x10 -> 4005
 //   leaves: 4003 = 08                                  (kind 8, bare)
 //           4004 = 09 01 84 00 0F                      (kind 9)
 //           4005 = 86 01 01 00 01 00 00 0F A6 00 00 0F A7
 //                                                      (kind 6 -> 4006,4007)
 //           4013 = 10 00 00 0F A2   kind 0x10 -> 4002 @ +0xCA75, 5 bytes
 // hdr[0] (field 0, the handler index that B241A0 dispatches on) therefore
 // resolves to kind 9 with payload 01 84 00 0F. HYPOTHESIS ONLY, do not encode
 // against it yet: if that trailing 000F is an inclusive max then the handler
 // index is 0..15, i.e. a 4-BIT field. Verify by reading the kind-9 accessor
 // sub_140AEC9F0 and its walker sub_140B22450 before trusting it.
 // 021000 DESCRIPTOR FORMAT FULLY DECODED (sub_140AEC870 + sub_140B21A30):
 //   [kindByte] [widthCode] [flag<<7 | value7] [MIN: n bytes BE] [MAX: n bytes BE]
 //   n = 1 << (widthCode >> 1); signed unless (widthCode & 1); when signed and
 //   the first payload byte is >= 0x80 the accumulator starts at -1.
 //   kind = kindByte & 0x3F. Wrapper kinds 11 and 16 skip all of the above and
 //   are just [kindByte][BE u32 typeId].
 //   For struct-ish kinds, fieldCount = MAX - MIN + 1, then fieldCount BE u32
 //   type ids follow. The 7-bit value in byte 1 is the BIT WIDTH.
 // Verified on three independent records, all self-consistent with the
 // index-table lengths:
 //   4008 CE|00 02 FC FF|... -> MIN -4, MAX -1 => 4 fields, and exactly 4 ids
 //   4004 09 01 84 00 0F     -> uint, range 0..15,  width 4   (5 bytes)
 //   4005 86 01 01 00 01|... -> range 0..1 => 2 alts, ids 4006/4007 (13 bytes)
 //   type 1 09 05 A0 00000000 FFFFFFFF -> uint32, width 32    (11 bytes)
 // EVENT::HEADER (4008) RESOLVED:
 //   f0 4009 -> k16 -> 4004  = uint 4 bits, 0..15   <- hdr[0], handler index
 //   f1 4010 -> k11 -> 4013 -> k16 -> 4002 -> k16 -> type 1 = uint32 OPTIONAL
 //   f2 4011 -> k16 -> 4003  = kind 8 scalar
 //   f3 4012 -> k16 -> 4005  = kind 6 choice, 1-bit selector
 //        alt 0 -> 4006 -> 4003 (kind 8)
 //        alt 1 -> 4007 -> 4002 -> type 1 (uint32)
 // kind 11 = TRUE OPTIONAL, confirmed in sub_140B224E0: when there is no value
 // it writes ONE presence flag and returns - nothing follows.
 // kind 8 is NOT void: sub_140B22400 moves a real value (src slot 64, dst slot
 // 192). Its descriptor carries no bounds, so it is a fixed built-in scalar.
 // CORRECTION, IMPORTANT: an intermediate reading here claimed
 // WriteCompleteEventHeader emitted ~64 spurious bits. THAT WAS WRONG. Mapping
 // it onto the schema, it is field-for-field correct:
 //   WriteBits(eventId,4)=f0, WriteBits(0,1)=f1 absent (correctly nothing
 //   after), WriteUInt32(0)=f2 sourceName, WriteBits(destClass,1)=f3 selector,
 //   WriteUInt32(destValue)=f3 alt 0. 70 bits total.
 // So the S->C HEADER WAS PROBABLY NEVER THE BUG, and the existing comment in
 // BsnBitStream.h ("m_id, optional Address, sourceName FourCC, dest") was right
 // all along. Do not "fix" the header on the basis of the retracted claim.
// kind 8 IS 32 bits. sub_140AEDB10 (BSNPrimitiveEncoder vtable slot 24, the
// kind-8 writer) gates on kind == 8 and moves the value as `unsigned int`, as
// does v5 in the kind-8 walker sub_140B22400. The width is not in the schema
// at all - it lives in a generated per-type setter reached through the blob at
// registry+0x18, entry (typeId+1), function pointer at +8. Confirmed on the
// wire too: the client logon carries the WoW FourCC as 00 2B B7 AB, which is
// exactly 0x00576F57 written MSB-first at a 1-bit offset. So the header is 70
// bits and correct; stop looking at it.
//
// THE SCHEMA ALSO CARRIES NAMES. sub_140B21880 is a SECOND index table at
// registry+0x50 (0x140F859AC), gated by registry+0x38 being nonzero, pointing
// into the same blob at registry+0x08:
//   aux = blobBase + BE_u32(0x140F859AC + 4*typeId)
//   aux is an array of BE u32 name offsets: [0] = type name,
//   [1..n] = member/field names, each blobBase + offset, NUL-terminated.
// Every string boundary matches its own offset exactly, so this is exact, not
// heuristic. Verified: type 4008 = "Battlenet::Event::Header" with fields
// m_id, m_sourceAddress, m_sourceName, m_destination - field-for-field
// identical to the structural decode above, which validates the whole reader.
//
// FULL DESCRIPTOR KIND TABLE (payload after [kind][widthCode][flag|width7]):
//   Signed/unsigned and bound size come from widthCode as documented above.
//   When the kind byte's high bit is CLEAR the member count is read from the
//   payload as [0x01][count][count member bytes][count BE u32 ids]; when it is
//   SET the count is MAX-MIN+1 and only the ids follow. That is the single
//   rule that reconciles kinds 6/7 with kinds 14/0xCE.
//   kind 1  = ARRAY. width bits = element count, range MIN..MAX.
//   kind 2  = STRING/BLOB. width bits = length, range MIN..MAX.
//   kind 6  = CHOICE. width bits = selector; explicit member value list.
//   kind 7  = ENUM. width bits = value; explicit member value list.
//   kind 8  = 32-bit FourCC-ish scalar (no bounds in schema).
//   kind 9  = INTEGER. width bits, range MIN..MAX.
//   kind 11 = TRUE OPTIONAL. one presence bit; absent writes nothing more.
//   kind 14 = STRUCT. all fields in order; the width7 value is just the field
//             index width and is NOT emitted on the wire.
//   kind 16 = WRAPPER. [kind][BE u32 typeId], no payload of its own.
//
// m_id 4 IS CORRECT - DO NOT "FIX" IT TO 5. sub_140A7E890 branches on *a2,
// selecting type 2292 for 5 and type 3788 for 1, which looks like an event id
// but is not: sub_140B236E0 stores a literal 5 into a local and passes its
// address as a2 (`v9 = 5; (*(vtable+32))(a1, &v9, a2+20, a3)`), branching on
// the m_destination selector at a2+16 rather than on anything from the packet.
// A7E890 is that vtable+32 slot - its data xref 0x140F4C830 is exactly the
// vtable base 0x140F4C810 plus 32. So the 5 is router-internal and m_id 4 is
// what reaches this path, as the EventProtocol note below already said.
// The {1,2} buffer from the earlier playtest was therefore a genuine 2292
// decode of a client-internal Service event, since Invoke tag 1 is Service.
//
// Battlenet::Event::Larva::Invoke = type 2292, kind 6, 8-BIT selector, 18
// alternatives. AUTHNET_INVOKE_CHOICE_BITS = 8 is CORRECT - do not change it.
// (An intermediate reading claimed 5 bits; that came from mis-indexing type
// 2277 and is wrong. 2292*4 = 0x23D0, not 0x2394.)
//   0 Authentication  6 Ladder      12 S2Map
//   1 Service         7 Advert      13 Cache
//   2 Connection      8 MatchMaker  14 Friend
//   3 Block           9 WoW         15 Toon
//   4 Game           10 DarkPortal  16 Presence
//   5 Party          11 Profile
// Invoke::Authentication = type 2293, kind 6, 8-bit selector, 12 members with
// tag 8 absent from the member list: Module 0, Success 1, Failure 2,
// CachedAccount 3, CachedGameAccount 4, CachedToon 5, ModuleBegin 6,
// ModuleEnd 7, GenerateSingleSignOnTokenResponse 9, SetLaunchOptionResponse
// 10, GetLaunchOptionResponse 11, ClearLaunchOptionResponse 12. The existing
// AuthnetLarvaInvoke/AuthnetLarvaAuth constants already match this exactly.
//
// Authentication::Failure = type 2313, the smallest real reply in the tree:
//   m_error      -> 486  -> 162  kind 9 uint16, 16 bits, range 0..65535
//   m_parameters -> 1317        kind 1 array, 3-bit count, range 0..4
// So a complete, schema-exact Failure body is 8 + 8 + 16 + 3 = 35 bits:
//   WriteBits(0, 8) WriteBits(2, 8) WriteBits(error, 16) WriteBits(0, 3)
// which is EXACTLY the shape BuildMarkerBodyProbe already emitted - it just
// used tag values 7 ("Advert") and 3 instead of 0 and 2. Send Failure first:
// it is far simpler than Success (type 2312 is a 6-field struct) and it is
// self-diagnosing. If the client swaps its hang for a real login error dialog,
// then framing + header + dispatch + body are all correct and only the success
// path remains.
//
// 013200 RUN, first with correct tags. Sent (after ping ack):
//   4901 00 00 00 00 00 2B B7 AB 01 | 00 02 00 C9 00
// Simulating BitWriter over those bytes accounts for every bit: byte 0 = 0x49
// is packetId 9 + inChannel 1, bytes 2-5 are the zero m_sourceName, bytes 6-9
// plus bit 0 of byte 10 are the full 32-bit FourCC 0x00576F57 (all of it - the
// low bit is present, so this run was NOT the "WoV" case), and bytes 11-15 are
// the byte-aligned body 00 02 00 C9 00 = Authentication, Failure, 201, empty
// array. So the body encoding is now provably correct on the wire.
// It was still rejected, and the reason is in byte 1. It reads 0x01 because
// BuildMarkerBodyProbeWithClientPrefix overwrote it, which set channel 1 but
// also zeroed m_id - so we shipped a correct Failure body under event id 0.
// The client's reply is the usual cumulative echo: its own control frame, then
// our 16 bytes verbatim, then our 4001 verbatim, then a 9301...0E tail whose
// middle bytes advance (3ED2870E, 3ED66F0E, 3EDA570E) like a tick. Once the
// report starts it swallows the ping acks too, which is why the backlog grows
// again here after previously staying clean on pings alone.
// NEXT RUN should send 4921 - channel 1 with m_id 4 intact.
//
// 014000 RUN. Sent 492100000000002BB7AB01000200C900 - byte 1 now 0x21, so
// channel 1 and m_id 4 both intact and byte-for-byte the shape of the client's
// own 4921 frame. STILL BOUNCED, identical signature: our 16 bytes echoed
// verbatim, then our 4001, then 9301...  So channel was not the last problem
// and the transport prefix is now provably not the discriminator.
// Remaining suspect, and the reason for the current change: m_sourceName. The
// 00 00 00 00 at bytes 2-5 is a hardcoded WriteUInt32(0) in
// WriteCompleteEventHeader, so every event we have ever sent declared a zero
// sender identity. It is a kind-8 FourCC, the same type as the m_destination
// alt 0 that B1B310 crashes on when 0, and the client's own frames carry real
// FourCC-looking words (42B7B737) in that region. Now sending the WoW FourCC.
// If that still bounces, stop pushing unsolicited events and answer the
// client instead: it repeats an m_id 0 channel 1 event carrying a 16-bit token
// (0x127, then 0x27) which we have never once replied to. A reply keyed to
// that token is the next hypothesis, since a bounce may simply mean "no
// request is outstanding for this event".
//
// 014300 RUN. Sent 492100576F57002BB7AB01000200C900 - m_sourceName is now the
// real FourCC (bytes 2-5 = 00576F57, byte-aligned since m_sourceName starts at
// bit 16). STILL BOUNCED, same signature, backlog 44 -> 62 -> 80.
//
// THREE SEPARATE FIELDS HAVE NOW BEEN FIXED AND VERIFIED CORRECT ON THE WIRE -
// Invoke/method tags, channel + m_id, and m_sourceName - WITH NO CHANGE IN
// BEHAVIOUR. Combined with the earlier result that withholding the event gives
// a clean ping-only loop, the discriminator is very unlikely to be event
// content at all. STOP GUESSING AT FIELDS.
// The client's report frame decomposes as:
//   [49xx][token][N x 14-byte record][retry][OUR FRAME][our 4001s][9301 tail]
//   record = [6-byte counter 3AB602xxxxxx][2 bytes][42B7B737][0000]
// One record is appended per ping cycle, the 6-byte counters advance
// monotonically like a tick, 42B7B737 is constant, and the retry field steps
// 1212 -> 1214 -> 1216 (+2 per record). The tail is packet 19 inChannel 0 with
// its own advancing counter. So this is a transport-level report that
// accumulates one entry per cycle plus everything we sent, not a per-event
// NACK carrying a reason code.
// The client is meanwhile RETRYING an m_id 0 channel 1 request with a 16-bit
// token (0x127, 0x27, 0x127) that we have never answered. Highest-value next
// step is a live breakpoint on B241A0 to see whether the client even attempts
// to decode our frame; if it never reaches the decode, event content is
// irrelevant and the blocker is the unanswered request or a transport field.
//
// !! THE 025800 CONCLUSION BELOW IS WRONG - SEE THE 030500 ENTRY AFTER IT.
// B241A0 turned out to be on the client's OUTBOUND event path, so "our frame
// never arrives there" was never evidence of anything. Kept for the record.
//
// 025800 LIVE BREAKPOINT RESULT - THE FRAME NEVER REACHES THE DECODER.
// Logging breakpoint at 140B242B3 (the dispatch call; RDX holds the decoded
// header - do NOT use RBP, this build keeps no frame pointer there). Over the
// window the authserver sent our event on FOUR separate connections, and
// B241A0 dispatched exactly TWO events, both m_id 5:
//   [1] m_id=5 +0=012FEF05 +4=00000000 +8=0DF7B640 +C=0 +10=0
//   [2] m_id=5 +0=012FEF05 +4=00000000 +8=0D7FA4F0 +C=0 +10=0
// Each was followed immediately by connection thread churn, i.e. the timeout /
// Service::Lost teardown. ZERO m_id 4 dispatches. So our frame is rejected
// upstream of any BSN decode, and every field-level fix - tags, channel, m_id,
// m_sourceName - was unverifiable by construction. DO NOT tune event fields
// again until a frame is observed reaching this breakpoint.
// Corollary worth noting: m_id 5 is the ONLY id ever seen decoded here, and
// A7E890 selects type 2292 (Larva::Invoke) for 5. That is live support for
// m_id 5 being the Invoke event id, i.e. for the hypothesis that was reverted
// on the strength of the old "event 4 dispatches B236E0" note. Unresolved.
// The echo is NOT an instrumentation artifact: _captured is appended only from
// socket().ReadBytes and SendCreepProbe only calls QueueSend, so those bytes
// really do come back over TCP.
// Also from the long 55185 connection: the client escalates its own frames
// through m_id 0,1,2,3,4 (two tokens each, 0x27 then 0x127), the retry field
// steps 1212..121E then jumps to 3200/3202, and then it gives up and closes.
// CAUTION on channel: the logon itself is channel 0 (channel_id=0 in the
// initial_header decode), so the earlier reasoning that events belong on
// channel 1 rests on reading the client's 49xx control frames as
// Event::Headers, which their length does not support. Treat ChannelEvent as
// unproven rather than established.
// NEXT: instrument UPSTREAM of B241A0 - the inbound packet path that parses
// the 6-bit packetId + channel - to find where our 16 bytes are discarded.
//
// 030500 BACKTRACE - B241A0 IS THE OUTBOUND PATH, AND THE CLIENT IS
// DELIBERATELY DISCONNECTING. Same m_id 5 hit, now with a call stack:
//   #13 sub_140A7C920  #9 sub_140B23920  #4 sub_140BE5820  #1 sub_140B38510
//   #12 sub_140B19230  #8 sub_140B5C010  #3 sub_140B7F8B0   #0 sub_140B241A0
//   #11 sub_140B1B5B0  #7 sub_140B6FEC0  #2 sub_140B6F530
//   #10 sub_140B1B310  #6 sub_140B6F8B0  #5 sub_140B5F460
// sub_140A7C920 is BattlenetLogin.cpp:1913 - it asserts with that file/line via
// sub_1402CF250, logs "Client initiated Disconnect from %s", and builds its
// event with &BSN::BSNPrimitiveEncoder::vftable over registry unk_141610820
// with typeId 1620. An ENCODER, not a walker. So the m_id 5 events we caught
// are the CLIENT EMITTING ITS OWN DISCONNECT, which is also why every hit was
// followed instantly by connection thread churn.
// Consequences:
//   - B241A0 is used on the emit path, so it is NOT proof of an inbound decode
//     and the 025800 "frame never reaches the decoder" reading is void. It is
//     still unknown whether the client parses our 16 bytes.
//   - The client is not passively timing out; it calls a deliberate disconnect.
//     Finding what invokes A7C920 is now the shortest path to the real cause.
//   - A7C920 is virtual - its only xrefs are vtable data slots (140F4CAA0,
//     14112994C, 141129960, 1417167F8) - so the caller is only visible live.
//     BT_DEPTH was 14 and truncated exactly at A7C920; raised to 48.
// The whole BattlenetLogin.cpp translation unit is enumerable via xrefs to the
// filename string at 140F4C5C0: ~35 functions from 140A72F60 to 140A7E620,
// which puts the 2292/3788 decoder A7E890 just past the end of the same TU.
// Each carries its own source line in the sub_1402CF250 assert, so the client's
// login state machine can be ordered by line number when needed.
//
// 031000 FULL 48-FRAME STACK - THE DISCONNECT IS A UI CANCEL, NOT A PROTOCOL
// REJECTION. Above A7C920 the stack leaves the network code entirely:
//   #14 sub_1403A6950  #15 sub_1403B8AC0 (thunk)  #16.. the glue/UI + script
//   dispatch chain (14015EEB0, 1401613A0, 14015F3C0, 14015E0B0, 14015F670,
//   140155B50, 140086BA0, 140086E90, 1400AE750, 1400AF0D0 ...), which REPEATS
//   verbatim at #27..#34, i.e. a nested script/event dispatch, down to
//   sub_140A91480 and the OS thread start at 784C4CCB2 / 786F7AD67.
// sub_1403A6950 is a pending-UI-action dispatcher switching on the global
// dword_14136685C over the bitmask 0,1,2,4,8,...,4096. Case 1 does
// `v2 = sub_140A819E0(); (*(*v2 + 272))(v2);` - that indirect vtable+272 call is
// the one at return address 1403A69C7, i.e. the frame that entered A7C920.
// dword_14136685C has 54 xrefs, all inside the 1403Axxxx glue module, so it is
// a general UI state word and NOT a single identifiable trigger. Do not chase
// it further: the conclusion is simply that the login UI waits, gives up, fires
// its cancel action, and the client disconnects. The client is not rejecting a
// packet at this point - it is timing out waiting for one.
//
// WHAT THE REPORT FRAME ACTUALLY IS - OUR BYTES COME BACK GROUPED BY KIND.
// Re-reading the 013200-014300 captures with exact send order:
//   we sent  4001 , then our 16-byte event, then 4001, 4001, ...
//   it returns  [records][retry] OUR EVENT , then ALL the 4001s , then the tail
// Cycle 1 returns event then one 4001 - REVERSED from the order we sent them -
// and cycles 2,3,4 return the event once followed by two, three, four 4001s.
// So the frame is not a stream echo. A buffer-aliasing bug in our own socket
// would preserve write order, so this grouping is real client-side structure,
// which also independently confirms the bytes are genuinely from the client.
// The implication is the important part: our 2-byte ping acks are accumulating
// in that report exactly like the event does. If the report is unconsumed or
// unroutable input, then NOTHING we send is being consumed - not even the
// trivial ack - and the discriminator cannot be event body content. The client
// looks like it is buffering our bytes without consuming them while it waits
// for some specific reply to its channel-0 logon, then UI-cancels.
// 032200 INDEPENDENT RELAY CAPTURE - THE REJECTION IS SYNCHRONOUS AND THE
// FRAMING IS CORRECT. .agents/authnet_relay.py sits between client and
// authserver on 1119 (authserver moved to Authnet.Port 11190) and logs both
// directions itself, so nothing below depends on our own socket code.
// Confirmed: the client returns our 16-byte event VERBATIM and never consumes
// it. Those bytes are distinctive so there is no ambiguity. It then keeps the
// event parked in its report while the 16-byte record list grows around it.
// CAVEAT, do not repeat this mistake: the relay's echo detector also flags the
// 2-byte 4001s, but our ping ack is byte-identical to the client's own ping, so
// a 2-byte match cannot say whose bytes they are. The 4001 counts prove nothing
// either way. Use --drop-sc-len 2 to withhold our acks if that ever matters.
//
// TIMING IS THE REAL FIND. The client bounces the event back 15-43 ms after
// receiving it, every single cycle, and returns EXACTLY our 16 bytes - no more,
// no less. So it parses the transport header, knows precisely where our frame
// starts and ends, and then refuses it. That is a synchronous ROUTING
// rejection, not a timeout, not a framing error, and not a buffering stall.
// Transport framing and length are therefore CORRECT. Stop working on them.
//
// TWO HYPOTHESES KILLED, both single-variable, both with no rebuild:
//   TCP COALESCING - REFUTED. The authserver queues the ack and the event
//   separately but the stack merges them into one 18-byte segment, so the event
//   was the trailing packet in that segment. Relay --split-first 2 sent the ack
//   alone and the event in its own segment 250 ms later; the client bounced it
//   back identically. Segmentation is not the discriminator.
//   m_id - REFUTED, AND THE 4-vs-5 QUESTION IS SETTLED AS IRRELEVANT. Relay
//   --sc-replace 492100576F57=492900576F57 rewrote m_id 4 to 5 on the wire
//   (byte 1 = 0x01 + 8*m_id, derived from the client's own frames 4901/4909/
//   4911/4919/4921 for m_id 0..4, so 5 is 0x29). The client returned the
//   REWRITTEN bytes, same 43 ms, same report shape. m_id 4 and m_id 5 fail
//   identically, so m_id is not what the client is discriminating on.
//
// The relay can rewrite any equal-length byte pattern in the S->C direction via
// --sc-replace OLDHEX=NEWHEX, so header field hypotheses no longer need a
// rebuild or even a server restart. Header bit layout for our 16-byte frame is
// 11 transport + 4 m_id + 1 optional + 32 m_sourceName + 1 destClass + 32
// destValue = 81 bits, padded to 11 bytes, body from byte 11.
// STILL UNTESTED, all equal-length and so all relay-testable:
//   - destClass bit 0 (NONE) -> 1 (ADDRESS); both alternatives are 32 bits so
//     the frame length does not change.
//   - destValue FourCC, currently WoW 0x00576F57, which appears on the wire as
//     002BB7AB + a low bit in byte 10 because it is written MSB-first at a
//     1-bit offset (0x00576F57 >> 1 == 0x002BB7AB exactly).
//   - m_sourceName: 0 and WoW have both been tried via rebuilds and both fail.
// NOT relay-testable byte-for-byte: setting the optional bit adds a 32-bit
// m_sourceAddress and shifts everything after it.
//
// 034700 THE ROUTER TABLE, READ LIVE. m_id 5 IS INVOKE AND m_id 4 IS NOT
// REGISTERED AT ALL. Dumped R13+0xB28, stride 16, 16 slots, at the dispatch:
//   registered: 1->140B23500  2->140B23560  3->140B235C0  5->140B236E0
//               6->140B23740  8->140B23800  9->140B237A0 10->140B23620
//              11->140B23680 14->140B23860 15->140B238C0   (all adj +0)
//   NOT registered: 0, 4, 7, 12, 13 - non-pointer values with nonsense
//              adjustors, i.e. uninitialized slots with no subscriber.
// The 11 handlers are consecutive 0x60-spaced thunks from 140B23500 to
// 140B238C0, which is what makes the stride and offset certain.
// m_id 5's handler is 140B236E0 - B236E0 itself. So B236E0 is REGISTERED AT
// SLOT 5 and its hardcoded 5 is simply its own event id, exactly as originally
// suspected. The old "Header event 4 dispatches CreepTendril Authentication"
// note is WRONG: m_id 5 is Invoke (B236E0 -> A7E890 -> type 2292) and m_id 4
// has no handler whatsoever. Delete that claim wherever it is relied on.
// This also explains a paradox: the dispatch has NO bounds check, so had our
// m_id 4 frame ever reached this router it would have called into garbage and
// crashed the client. It bounces cleanly instead, which independently confirms
// this is the OUTBOUND bus and our frame is refused before ever reaching it.
//
// INSTRUMENT NOTE, and the reason the above is trustworthy: reading registers
// from a wait_for_next_event loop returns a context that does NOT belong to the
// breakpoint instruction (rdx-rsp came out 0xD8 where the code requires 0x40,
// and rbp/r13 read as 0 although the code loads rbp from [rax] and indexes
// through r13). Use a PYTHON-CONDITIONAL BREAKPOINT instead - IDA evaluates the
// condition in the right thread context - and validate every sample against
// r9 == 2*m_id and rdx == rsp+0x40, both of which the CPU computes from the
// same header byte at that instruction. ida_bytes also returns 0xFF fill for
// unmapped pages rather than failing, so an all-0xFF buffer is a FAILED READ,
// not data; one run reported m_id 255 and 16 identical FFFF handlers that way.
// The validated header is at RSP+0x40 and is at least 24 bytes: +0 m_id byte
// (bytes 1-3 are uninitialized padding that can look like a stack address),
// +16 m_destination selector, +20 m_destination value.
//
// B236E0 FULLY DECOMPILED - OUR DESTINATION IS ALREADY CORRECT:
//   v3 = *(a2+16);  selector 0 -> (vtable+32)(a1, &5, a2+20, a3)
//                   selector 1 -> same but with a COPY of *(a2+20)
//                   selector >= 2 -> falls through and returns 0, SILENT DROP
// So destClass 0 and 1 are both valid and only >=2 is dropped. We send
// destClass 0, which is the handled else-branch, and vtable+32 is A7E890 (its
// data xref 140F4C830 is vtable base 140F4C810 + 32). m_id 5 + destClass 0 is
// therefore exactly the combination that reaches the Invoke 2292 decode - and
// we PUT THAT EXACT FRAME ON THE WIRE at 033000 and it still bounced.
// CONCLUSION: the rejection is not in this router and not in the header fields
// we can reach from it. Every field we can currently justify is already right.
// THE ONE BLOCKING UNKNOWN IS THE INBOUND PATH, which is still unlocated.
// Suggested attack: breakpoint the client's socket read (ws2_32 recv/WSARecv)
// and walk up to whoever parses the 6-bit packetId + channel, then instrument
// THAT with a validated conditional breakpoint. Do not assume any breakpoint is
// inbound without a stack that proves it - that error has cost two runs.
//
// TWO CAPTURE ARTEFACTS TO NOT MISREAD NEXT TIME (035000 relay log):
//  1. Connections that show "C->S 89 B, S->C 0 B" then close after 9-14 s are
//     NOT a protocol signal. The client never sent its 4001 ping, so the
//     authserver - which is silent until the ping - correctly sent nothing.
//     They line up exactly with the runs where the old watch() loop was
//     SUSPENDING the client at the breakpoint. Debugger artefact, ignore.
//  2. The 2-byte slot immediately after our echoed event VARIES between cycles:
//     observed 0000, 4001, ED01, 4992, 24B6, 0D00. So the region after the
//     event is a growing run of 2-byte entries with a variable trailing field,
//     not a clean list of our ping acks. Combined with our ack being
//     byte-identical to the client's own ping, whether ANY of those 4001s are
//     ours still cannot be settled from these bytes. Settle it with the relay's
//     --drop-sc-len 2 (withhold our acks) before building anything on it.
//
// BETTER THAN PERMUTING FIELDS: the client bounces because no subscriber
// matches. Find the client's SUBSCRIBER/ROUTING TABLE and read off which
// (m_id, destination) pairs are actually registered, instead of guessing. The
// routing chain seen live is B1B310 / B1B5B0 / B19230 / B23920. Note B241A0 is
// OUTBOUND - the inbound decode path is still unlocated, and A7E890 (selects
// type 2292 for 5 / 3788 for 1) remains the best candidate. Do NOT assume any
// breakpoint is on the inbound path again without a stack that proves it.
 // DONE - kind payload semantics are in the FULL DESCRIPTOR KIND TABLE above.
 // Per-kind accessor map, kept for reference (from sub_140B22990):
 //   kind 6  -> sub_140AEC990 / sub_140B22170
 //   kind 8  -> sub_140AECA20 / sub_140B22400
 //   kind 9  -> sub_140AEC9F0 / sub_140B22450
 //   kind 11 -> sub_140AECB90 / sub_140B224E0
 //   kind 16 -> sub_140AECB90 / sub_140B22940
 // The blob at registry+0x18 (0x140F578C0) is NOT the name table - it is the
 // generated per-type accessor table, entry (typeId+1), function pointer at
 // +8. Names live behind registry+0x50 via sub_140B21880, as described above.
 // Live address mapping (base 0x7FF66CD70000, verify per launch):
 //   A78ED0 = 7FF66D7E8ED0   A7E890 = 7FF66D7EE890   A7E9A3 = 7FF66D7EE9A3
 //   B236E0 = 7FF66D8936E0   B241A0 = 7FF66D8941A0   B24289 = 7FF66D894289
 // A78ED0 and A7E890 transpose easily — do not mix them up again.
constexpr char const* AUTHNET_DESTINATION_CLASS_NAME = "Destination::Class";
constexpr char const* AUTHNET_DESTINATION_ADDRESS_NAME = "Destination::Address";
constexpr uint32 AUTHNET_EVENT_ID_BITS = 4;
constexpr uint32 AUTHNET_EVENT_OPTIONAL_BITS = 1;
constexpr uint32 AUTHNET_DESTINATION_CLASS_BITS = 1;
constexpr uint32 AUTHNET_DESTINATION_CLASS_NONE = 0;
constexpr uint32 AUTHNET_DESTINATION_CLASS_ADDRESS = 1;
// Client packs "WoW" as char|(acc<<8) = 0x00576F57. B1B310 Class 0
// crashes on dest FourCC 0, so this must stay non-zero.
constexpr uint32 AUTHNET_SUBSCRIBER_FOURCC_WOW = 0x00576F57;
constexpr uint32 AUTHNET_HEADER_FIELD_PRESENT_BITS = 1;
// Old 4-bit Class packing. Optional-present plus a bare integer
// (49A0130122 / 49A0120211 / 49A00000120211) set m_sourceAddress,
// not Destination, and opened a new invoke. Do not use on 4920.
constexpr uint32 AUTHNET_EVENT_CLASS_BITS = 4;
constexpr uint32 AUTHNET_EVENT_CLASS_CORRELATED = 1;

namespace AuthnetCreepEvent
{
    constexpr uint32 Patch = 0;
    constexpr uint32 Authentication = 4;
}

// Packet 9 channel 1 in the 4920 playtest: Header event 0, then exactly
// 16 bits (0x1202, retry 0x1802). Same 11-bit transport as channel 0;
// this is not the packet-5 RC4 mode-switch.
constexpr uint32 AUTHNET_REQUEST_TOKEN_BITS = 16;

// Leading bytes of a retransmit frame - 49, the 1+8k counter, and the 16-bit
// token - mirrored back verbatim as the transport ack.
constexpr size_t AUTHNET_CONTROL_ACK_BYTES = 4;

// Client-emitted tick packet: 4901 1800 <u32 tick> 00. The same u32 appears in
// the 16-byte units as 0027 3AB6 <tick> 0001 42B7B737 0000, and it advances
// ~8008 per ping, i.e. milliseconds on an 8 second interval.
constexpr size_t AUTHNET_TICK_PACKET_BYTES = 9;
constexpr uint8 AUTHNET_TICK_PACKET_PREFIX0 = 0x49;
constexpr uint8 AUTHNET_TICK_PACKET_PREFIX1 = 0x01;
constexpr uint8 AUTHNET_TICK_PACKET_TAG0 = 0x18;
constexpr uint8 AUTHNET_TICK_PACKET_TAG1 = 0x00;

// Byte 1 of the client's control frames. The "1 + 8k counter" (01, 09, 11, 19,
// 21) is not a counter at all: byte 1 is channelId & 7 in bits 0-2 and m_id in
// bits 3-6, so those five values are channel 1 with m_id 0, 1, 2, 3 and 4.
// 0x21 is therefore channel 1 + m_id 4, the shape of an Authentication event,
// and the 0x20 our probes used to send is the same m_id on channel 0 - a
// channel the client never puts an event on. Produce these by passing
// AuthnetWire::ChannelEvent to WritePacketHeader; do NOT assign byte 1
// directly, which wipes m_id.
constexpr uint8 AUTHNET_CLIENT_FIRST_CONTROL_PREFIX_BYTE = 0x01;
constexpr uint8 AUTHNET_CLIENT_COUNTER4_CONTROL_PREFIX_BYTE = 0x21;

// Creep Authentication failure codes from sub_140A73A20. 103 is the
// value that becomes LOGIN_BAD_SERVER_PROOF (login result 4). 138 is
// ignored after the connection.log print. 201 maps to login result 7.
enum class AuthnetAuthFailure : uint32
{
    BadServerProof = 103,
    Ignored = 138,
    Probe = 201
};

// EventProtocol lives in Wow-64.exe (blob at 0x140F773E0, 238266 bytes,
// version 7, 4859 types). sub_140E66490 calls sub_140B21600 to bind it
// to unk_141610820. Type 1620 is Creep::Invoke; 1621 is
// Creep::Invoke::Authentication. Type 2292 is Larva::Invoke; 2293 is
// Larva::Invoke::Authentication. Header event 4 dispatches CreepTendril
// Authentication (B236E0), then A7E890 decodes the remaining bits as
// type 2292 (outer Invoke, tag 0 = Authentication), then 2293.
// C->S follow-ups after Header event 4 still decode as 1-bit present
// plus 4-bit 1621 tags. sub_140A78ED0 discriminators are in-memory
// after decode.
enum class AuthnetAuthEvent : uint32
{
    LogonResult = 0,
    ConnectionState = 1,
    Nested = 9
};

// Type 2293 Success (tag 1) has six fields including program FourCC.
// RequestPassword is a Module program-name string compared in
// sub_140A78ED0 (A91DA4), not a 2293 variant. A78ED0's Authentication
// branch (*a2==0) only handles 2293 tags 0 Module, 1 Success, 2 Failure,
// and 9 GenerateSingleSignOnTokenResponse. ModuleBegin (tag 6) and
// SetLaunchOptionResponse (tag 10) are no-ops there. 143208 sent
// 49200006; 145725 sent empty Module 49200000. Both copied the extra
// tags into the next request, still sent SetLaunchOptionRequest, and
// timed out reason 8. Empty Module writes a 0-byte function blob, so
// A91DA4 has no RequestPassword string to match.

// Type 1621/2293 choices: kind 0x06, bounds bits=8 min=0 max=255, with a
// mapping table. C->S bodies after Header event 4 still decode as 1-bit
// present plus 4-bit tag (tag 9 = SetLaunchOptionRequest). S->C is type
// 2292 (A7E890), so the decoder reads bounds.bits (8) for the outer
// Larva::Invoke tag (0 = Authentication), then 8 bits for 2293. 142448
// sent 1+4 tag 9 on the initial 4920 (492013); the client copied 0x13
// into the next Header packet. Type 2293's map is
// [0,1,2,3,4,5,6,7,9,10,11,12]: wire tag 8 is unused; wire tag 9 is
// GenerateSingleSignOnTokenResponse; SetLaunchOptionResponse is wire
// tag 10. Module (tag 0) is a 3-field struct2: m_function is type 1309
// (kind 2, 6-bit byte length 0-63), m_parameters is type 1 (32-bit blob
// length), and the third field is a 3-bit integer 0-4. 145725 packed
// tags only (49200000). 150523 nested a 3-bit/9-bit name from the type
// records after 1309. 151715 packed RequestPassword immediately after
// the 6-bit length (492000004F52...); the client copied 25 bytes
// (token 0x1219) and stayed CONNECTING. Kind-2 blobs are consumed as
// bytes after the length integer, so the name must AlignToByte like
// identity strings. 152406 sent that aligned blob after 8-bit tags
// 0,0 (492000000F52...); the client copied 25 bytes (token 0x1219)
// and stayed CONNECTING. 154819 replaced those tags with counted
// Authentication/Module names (49200E41..., 45 bytes, token 0x320D)
// and still timed out: 2292's map includes wire value 14, so 0x0E
// selected the wrong Larva::Invoke variant. Kind-14 Module fields are
// named m_function / m_parameters. ModuleBegin (tag 6) is empty.
// 160034/160122 sent those field names on the wire after tags 0,0
// (49 bytes, token 0x3211). Both the correct-password and
// incorrect-password attempts were identical: copy, SetLaunchOption,
// ping, retry 0x3811, ~30s reason 8. Walker kind-14 (B22700) passes
// field names into stream vfunc+120 from the type DB; they are not a
// 6-bit blob. That packet decoded m_function as "m_function".
// Failure (type 2313) is kind-14 with two fields: type 162 (kind 9,
// 16-bit 0-65535) then type 1317 (3-bit count 0-4). A78ED0 logs
// "Battle.net Authentication failure code: %d" from that WORD.
// 161118 sent 8-bit tags 0,2 plus code 201 (4920000200C900, 7 bytes,
// token 0x1207). The client copied it, SetLaunchOption, ping, retry
// 0x1807, ~31s reason 8. connection.log never printed a failure
// code, so A78ED0 did not take the Failure branch: Walker vfunc+136
// does not treat those 8-bit integers as Authentication/Failure
// names. 162103 sent identity-style 9-bit (length-3) names
// (4920050141..., 30 bytes, token 0x121E) and also timed out with
// no failure log. 162443 sent 6-bit+align Authentication/Failure
// (49200E4175..., 28 bytes, token 0x121C) — the 154819 name form
// with Failure instead of Module — and still no failure log.
// 0x0E is not a usable 6-bit name length here. 162810 sent
// NUL-terminated Authentication/Failure (4920417574..., 28 bytes,
// token 0x121C) and still no failure log. 164556 packed FourCC and
// Destination Address immediately after 4920 (49200000000001...,
// 15 bytes, token 0x120F). 165412 prefixed those with two
// presence-present bits (492003..., 15 bytes, token 0x120F). Both
// copied as body: leftover bits after id+optional are not Header
// fields 2-3. 173145 sent optional m_sourceAddress present with
// 32-bit 0 then integer Failure 201 (49A000000000000200C900, 11
// bytes, token 0x120B). Same hang, no crash, no failure log. That
// field is consumed; Destination is not. 174840 sent those Failure
// tags with no Header (490000021901, token 0x1206): event 0 Patch,
// same hang. 180822 sent dest Class immediately after 4920
// (4920436C61737300..., token 0x1211): copied, no failure log.
// 181219 sent sourceName FourCC 0 then dest Class (token 0x1215):
// copied, no failure log. Next probe is 4920, sourceName 0,
// Destination::Class, dest FourCC 0, then Failure
// (49200000000044657374696E6174696F6E3A3A436C6173730000000000000200C900).
// 182056 copied that as token 0x3202. 182340 sent Destination::Class
// immediately after 4920 (token 0x121E). 183804 sent 1-bit dest Class
// FourCC 0 (token 0x120F). 184634/185218 compact 4+2 after 4920
// (token 0x120D) copied as body. 192222 dest Class FourCC WoW
// (token 0x120F) copied as body. 193025 counted Class (token 0x1215)
// copied as body. 194118 compact-only (token 0x120D) echoed as
// EncodedObject event 0. Next probe is 4920, sourceName 0,
// identity-style counted Class, FourCC WoW
// (4920000000000100436c61737300576f57000200c900).
//
// Extra packets after the initial 4920 opened a new invoke. Channel-1
// token low byte is the copied S->C packet length (0x1202 empty 4920,
// 0x1207 = 7-byte Failure tags, 0x120F = 15-byte complete Header plus
// Failure, 0x1219 = 25-byte Module, 0x121A = 26-byte Module, 0x121C =
// 28-byte named Failure, 0x121E = 30-byte identity-name Failure,
// 0x320D = 45-byte named-choice copy, 0x3211 = 49-byte named-field
// copy). Do not echo 4001. Do not send a second packet.
//
// 2026-08-29 MIDDAY - MOST OF THE BOUNCE INVESTIGATION ABOVE IS INVALID.
// The "client bounces our event" hunt was chasing a self-inflicted bug.
// Uncommitted local edits had replaced the startup response to the 89-byte
// logon with a marker probe behind AuthnetSilentProbe, so the server answered
// the logon with NOTHING. The client waited ~16 s, fell back to a 2-byte
// frame, and rejected an unsolicited event sent in a phase where it is still
// waiting for a startup response. Every header-field permutation recorded
// above was therefore evaluated in the wrong protocol phase: none of those
// results are evidence about Event::Header correctness, and the tag/channel/
// m_id/m_sourceName conclusions drawn from them must be re-derived before
// being trusted. Restoring BuildStartupResponseProbe changed client behaviour
// immediately - logon answered in 15 ms with 37 bytes, and the client now
// CLOSES cleanly 94 ms later instead of hanging. A close is a decision; the
// hang never was.
//
// 4001 IS NOT A PING. Decoded with the 6+1+4 transport it is command 0,
// modeSwitch 1, mode 1. Every "ping ack" / "keepalive" reading above is
// wrong. ReadBits(6) on a fresh stream is just data[0] & 0x3F, so 0x49 -> 9
// (login) and 0x40 -> 0; the mode nibble then takes bit 7 of byte 0 as its
// high bit and bits 0-2 of byte 1 as the rest. Note the mode-switch gate
// wants command 5 (frame 4501) and we have never captured one from the
// client, so the encrypted era has never actually been entered.
//
// WHAT STILL STANDS. The 6+1+4 transport layout is confirmed by a second
// independent implementation (upstream TryReadPacketHeader), as is command 9
// = login. The client-side inbound path is now known: the PacketPipe recv at
// 140A61CFA packs a descriptor (+00 buf, +10 len, +14 0x20, +18 remaining)
// and hands it to 140A6C090, a thunk to 140A60130, which dispatches to
// [[obj+32]+24]. A 16-byte event was observed arriving there intact, so
// neither the network nor the transport was ever the problem. B241A0 is the
// OUTBOUND bus, not the inbound parser.
//
// HTTP ON THE AUTHNET PORT IS PROTOCOL, NOT NOISE. The client polls
// GET /agent with Content-Type application/com.blizzard.agent.v1+json and
// treats a bare 204 as "retry now" - 1500+ connections per minute. It also
// fetches /wow-pod/ paths. Any capture on this port must filter it out or it
// buries the authnet frames.
//
// The startup account list is advertised as EMPTY by default
// (AUTHNET_STARTUP_ACCOUNT_RESPONSE=none), i.e. we tell the client it owns no
// game accounts. Its 40-byte key field is exactly the width of
// auth.account.session_key binary(40).
//
// 2026-08-29 AFTERNOON - Soft header was never the reject. Live Creep Layer
// trace: soft_done result=0 pkt=0 flag=1 chan=0, then handler_call target
// 140B56F70 (packet-type-0 body). That body is:
//   choice(1): 1 = account (E2A3C0), 0 = large alt (E2AE20)
//   keyPresent(1) + optional 40 raw bytes (byte-aligned)
//   nameTag(2): 0 empty / 1 = uint16+uint32 via E25F30 / 2 other
//   listPresent(1) + optional 10-bit count + bytes
// BuildStartupResponseProbe had been writing choice=0 then a bogus 3-bit
// "account count", so AUTHNET_STARTUP_ACCOUNT_RESPONSE=1 still fed the key
// into the wrong variant. Corrected to choice=1 + keyPresent + empty name
// tag + no list. Name is NOT a 10-bit length-prefixed string.
//
// Live follow-up: Soft ok + body decoder ran, then ~30s later err_soft
// code=13 (idle_rx_timeout via B4C3B0→B76E30). That means the body was
// accepted; name tag 0 only sets a flag and the client waits forever.
// Name tag 2 (no extra payload) makes B53280 hardcode id 117 and fire
// Larva::Invoke — which surfaces as Battle.net error #117 in the UI.
// Tag 1 uses the wire uint16 as that same error code. So choice=1 is a
// Result union, not "account name". Initial login wants choice=0 session
// (E2AE20): 3-bit account count + E14340 records (40-byte key + 10-bit
// name) + timeout + two E14100 strings + trailing fields + optional list.
//
// 2026-08-29 choice-0 BODY OK then Battle.net error #114. That is
// B53190 from AuthModuleCacheLookupManager when the client cannot
// resolve an AuthModule for the account record ("Authentication module
// not found..."). The 40-byte E14340 field is hashed/looked up as a
// module id — it is NOT simply auth.account.session_key. Sending a
// session_key there makes Soft+body succeed then fail module lookup.
//
// count=0 (no E14340 records) BODY OKs then B537E0 hardcodes error
// #103 — UI string "You have attempted to connect to an invalid
// server." Empty account list is a dead end; need count>=1 with a
// real AuthModule id (or a Module download path that fills the cache).
//
// 2026-08-29 AuthModule id format (Wow-64 18414):
//   .auth file = PE32+ DLL + 260-byte SIGN trailer (magic NGIS/SIGN).
//   ClientSideCache key is 40 bytes laid out as:
//     [0..3]  extension FourCC, e.g. 'auth' = 61 75 74 68
//     [4..7]  pad (zeros work)
//     [8..39] SHA256 of the whole .auth file (32 bytes)
//   C1BBF0 builds Cache\<hh0>\<hh1>\<64-hex>.<ext> from that (a2=2
//   shard depth). SHA1(PE)||SHA1(SIGN) was WRONG — live mod_open then
//   missed and fell into Cache\TMP\<other> download staging → #114.
//   Probe module (Ovahlord x64 0a3afee2….auth):
//     AUTHNET_STARTUP_ACCOUNT_KEY=
//       61757468000000000A3AFEE2CADE3A0E8B458C4B4660104CAC7FC50E2CA9BEF0D708942E77F15C1D
//     file already at ...\Battle.net\Cache\0a\3a\0a3afee2….auth
//   Live: CreateModule OK, then mod_handle len=4 op=87 head=576f5731 ("WoW1").
//   E14340 "name" is fed straight to Password.dll Handle as the first module
//   message — not a display name for this module. Opcode must be 0/1/3;
//   challenge payload is 1+32+32+128+128. Probe knobs:
//     AUTHNET_STARTUP_ACCOUNT_PAYLOAD=challenge  → placeholder opcode-0 blob
//     AUTHNET_STARTUP_ACCOUNT_PAYLOAD_HEX=<hex> → raw name-field bytes
//   Soft0 session0_empty (count=0): BODY OK then B537E0 d1=2 word=103
//   (confirmed live). Soft0 with Password empty-name also finishes → #103 —
//   Soft2 proof does not Set Auth property 0xA2057DF4 (byte>=1) that
//   B537E0 needs to return 1 → B53940.
//   Password.dll (PasswordSafe) state machine on prop 2462361631:
//     op0 CHALLENGE → Set state=1, RequestPassword
//     Soft2 proof (622B0): needs state==1; Sets A/M1/crc props; Sets
//       state=2; store 289-byte uplink (op=2 + A128 + M1_32 + crc128)
//     op3 server proof (61D10): needs state==2; 32-byte M2 + 128 blob;
//       success → state=3 + session props; fail → #101 (no state) / #103
//   Soft2 flush after uplink destroys ChallengeHandlers. Soft2 op=3 into
//   a fresh module: live 2026-08-29 proof3 → mod_handle len=161 op=3 →
//   mod_report code=103 (NOT #101). State==2 and SRP props survive
//   recreate; zero M2/blob fail crypto.
//   Op3 verify (61D10):
//     M2 check 61220: SHA256(A128_le || M1_32 || K64) == server_32
//     629D0: second == (g^S * k^H) mod ProofN where g=4, H=HMAC-SHA512(
//       accountName, prop205). prop205 = 128 bytes from Auth
//       Crypto::SecureRandomWrapper @ +9784 (CryptGenRandom) — Set as Auth
//       prop 2055043961 in Success handlers B541A0 / B54330. Soft2 uplink
//       Gets that prop. File reuse across logins cannot satisfy 629D0.
//       Operator path: SkyFire Launcher Soft mode overwrites the fill
//       (call BE6050 before Set) with kAuthnetFixedProp205; authserver
//       builds challenge.second from the same bytes.
//   Module SRP (CreateModule 61080): g=2, N=unk_3896E390 (not 6E280),
//     k=SHA256(N||g), M1=SHA256(NgHash||SHA256(I)||salt||A||B||K),
//     I = hex(userSeed). BN load/export (62CE0/63000/630C0) is LE —
//     wire B/second/A/proofS and rdata N must be LE. BE B → B>=N → #5.
//     Password uppercased (6CC70) before x = SHA256(salt||SHA256(I:pw)).
//     Implemented in Authentication/PasswordSrp.*.
//   AUTHNET_SRP_PASSWORD or authnet_srp_password.txt enables real challenge/op3.
//   Soft2 identity is the client cmd=9 email (any account_login_identity row);
//   AUTHNET_STARTUP_CHALLENGE_IDENTITY / AUTHNET_SRP_ACCOUNT_NAME are probe
//   fallbacks only when that field is empty. Shared password file is used for
//   every account (classic account.salt/verifier is GRUNT SHA1, not Password.dll).
//   AUTHNET_MODULE_PROOF_RESPONSE=proof3|session0_empty|session0|none
//   AUTHNET_MODULE_PROOF_REPLY_HEX=<hex> overrides op3 payload.
//
//   Soft2 629D0 / prop205 (launcher Soft mode)
//     Challenge.second is HMAC-SHA512(accountName, prop205) in the Soft2
//     challenge on the wire. Stock client CryptGenRandoms prop205 at Soft
//     Success Set (B54242 / B545E0, prop 2055043961) AFTER that challenge
//     is already queued. Soft2 uplink carries prop205 too late to rebuild
//     second (discrete log on proofS). SkyFire Launcher Soft mode replaces
//     those two BE6050 fills with kAuthnetFixedProp205; LoadAuthnetProp205
//     defaults to the same constant (AUTHNET_CLIENT_PROP205_HEX overrides).
//
//   Soft0 vs second TCP (2026-08-30 / 2026-08-31)
//     Soft0 session0_empty on Soft2 empty → B537E0 #103 without A2057DF4.
//     Soft-mode launcher patches B537E0 → mov al,1; ret (AuthnetProp205Hook).
//     Soft0 is ON by default (AUTHNET_POST_PROOF_SOFT0=session0_empty);
//     AUTHNET_POST_PROOF_SOFT0=none restores hop TCP. Soft2 Success alone
//     (no Soft0) B541A0→B7FBA0 opens Creep::Server::Sunken to
//     {region}.logon.battle.net. SkyFire Launcher gethostbyname hook must
//     redirect that host to authnet (Soft mode: DNS + prop205 force, do not
//     apply classic LoginFlowPatches). AuthnetSocket publishes SoftK64 by
//     client IP. Adopt only on hop 4501. Hop cmd=9 is a fresh Auth login
//     (60523). Do not Close() the proof socket (live 58053: that destroyed
//     Auth/Sunken and the hop never spoke). Auth/Prf 205 zero-blob on the
//     proof TCP precedes the 444-byte client close; answer Conn/Nul there
//     instead. Do not force B7FBA0's numeric-host path (C1B1A0("127.0.0.1")):
//     Tumor inet_pton skips WowConnection::StartConnect (playtest 18:35, no
//     hop). FQDN C1B1A0 reaches Tumor A75CC0 (gethostbyname → B1B120 →
//     B23210 BSN on the proof socket → server echo → Resolver B80580).
//     Stock A75CC0 returns "not connected" without sending. Skipping that
//     early-out for B7FBA0 (playtest 21:57) still mute: ACCEPT ~80ms after
//     Success with 0 bytes, proof 444/610, no extra C→S besides 4200.
//     B1B120 looks up Adapter+0x938 slots filled by B1AC80, not C1B1A0's
//     Auth map. Do not jump A75CC0 to A75E8F with a fake IP list (21:23).
//     Wrapping every C1B1A0 with B80580 hits Login (dump 22:06 pid 13356,
//     ERROR #132 at 0x1B014D, `movaps` on RSP ≡ 8; 21:28 silent close, no
//     cmd=9). Second-call B80580 (playtest 22:54) still mute: Manager
//     BE5F10 vtbl[1] is nullsub and skips BE5860 while the proof socket
//     occupies Manager+16. Forcing BE5860 after zeroing +16 (playtest
//     23:03) still mute: hash at Manager+80 already holds the proof
//     IP:port so B80270 is skipped, and B80580 destroys Resolver first
//     (no +58ms mute ACCEPT). B80270 before B80580 (playtest 23:12): hop
//     ACCEPT :55597 +32ms, 0 bytes; B80580 still queues nothing.
//     Soft-finish send is B53940 then B76E40 on that Sunken (B53E40 after
//     Soft0). Soft-finish assert (B54108 / AEC060): when Auth+9609 and
//     +9610 are set, B53E40 walks ChallengeHandlers and asserts if any
//     has [handler+0x432] != 0 (outbound blob busy). Soft2 flush after
//     uplink destroys those handlers — Soft0 after Soft2 empty is safe;
//     forcing Soft-finish / Auth+9609+9610 mid Soft2 (or Soft Success
//     stubs that arm Soft-finish early) hits Internal Battle.net Error.
//     AUTHNET_POST_PROOF_SOFT0=none skips Soft0 and publishes Soft-finish
//     pending for the hop TCP. Soft0+B537E0 force (2026-08-29): client
//     sends 4501 (cmd=5 mode=1) then encrypted follow-up — first live
//     packet-5 mode-switch. ProcessPostProof hands off to
//     ProcessEncryptedClientBytes. Soft finish RC4 is
//     HMAC-SHA256(SRP_K64, salt16) from B75150/B751B0 (salts DD10/DD20), not
//     the static Initial*CryptState tables. OnRead must not decrypt from
//     initialRequestLen+2 after Soft2 (that offset is Soft2 uplink).
//   Soft Join Soft-ACK behind authnet_relay (2026-08-31)
//     Debug relay owns :1119 and forwards to Authnet.Port 11190. AuthnetSocket
//     then sees every Soft peer as 127.0.0.1 → Soft Join Soft-ACK list6 uses
//     LocalAddress (127.0.0.1:8086). Local in-game works; LAN Soft clients try
//     world on themselves and never reach character select. Production Soft:
//     Authnet.Port=1119, no relay, so Soft peer IP is real and non-loopback
//     Soft Join Soft-ACK uses ExternalAddress (e.g. 192.168.1.226:8086).
//
//   Soft finish RC4 (continued from Soft0 4501 path):
//   Live: DD20 decrypts C→S (cmd=9 mode=11); DD10 encrypts S→C. Default
//   assignment matches that; AUTHNET_SOFT_FINISH_CRYPT_SWAP=1 flips.
//   Post-finish LoginAuth is mode2. Client sends empty cmd0 (4002).
//   S→C cmd0 body is Hard choice (E24300): status u8 or 7-bit Handle list —
//   not Larva 2315 names. B398F0 registers cmd0 (not cmd1).
//   :58993 status0 / :64118 Handle list → COP_CONNECT RESPONSE_CONNECTED,
//   no bounce, but no COP_GET_REALMS. Default: single-list (region=1) +
//   Soft Soft mode2 cmd3 (empty). Do not unsolicited-push realmlist:
//   B53940 enables mode1 cmd6 on the TX bitmask (client may send 4601)
//   and mode0 cmd4 on the RX bitmask. Unsolicited mode1 cmd6 Start
//   bounces (quoted after 9200). CGA works because B75090 attaches the
//   mode2 handler table (B398F0 / B393D0).
//   A83990 (COP_GET_REALMS) on BattlenetLogin is A7C410 → Soft cmd6
//   Creep::Invoke::WoW::RequestRealmList. Live 4601 is header-only
//   (cmd=6 mode=1, 5 pad bits 0) — command 6 is the WoW slot. A75010
//   encodes type 1620; S->C is type 2292 (Larva tag 9 = WoW) then
//   type 2302 (8-bit method: 0=RealmUpdate, 1=RealmList, 2=JoinResponse).
//   filled by A75420 after Complete (event 5) walks BN_REALM from
//   A777B0 (A78ED0 method 0). Soft packets are one bit stream:
//   leftover bits in a byte belong to the next header, not padding.
//   2026-08-30 coalesced five AlignToByte frames (190 B); Start's 4 pad
//   bits poisoned the next header. Bit-contiguous Start+Updates+Complete
//   (189 B, names on the wire) still left the viewer empty: without
//   Larva tag 9 the 8-bit method is read as 2292's service, A78ED0
//   never sees WoW, A777B0 never runs. Default service bits 8 (tag 9);
//   AUTHNET_REALMLIST_WOW_SERVICE_BITS=0 omits it.
//   2026-08-30 tag 9 on cmd6 (194 B): realmlist UI opened empty; user
//   cancelled. 4601 RPC waiter takes one 2292 object (Start) and drops
//   Updates+Complete. Names fill only via Header m_id 5 (B236E0 →
//   A7E890). AUTHNET_REALMLIST_HEADER_INVOKE=1 sends packet 9 channel 1
//   Header frames (post-Soft-finish those parse as Soft cmd 9, not
//   Invoke). Default 0: queue separate cmd6 Start/Updates/Complete in
//   OnRead. Mode1 table 62F2B0 has cmds 0-4 only. 2026-08-30 024431
//   stuffed Event m_id 5 on cmd6 (4629…); five SMSG, still empty —
//   B38510 is in-process vtable+112, not Soft cmd6 body. Set
//   AUTHNET_REALMLIST_FRAME_DELAY_MS=25 to stagger recvs.
//   AUTHNET_REALMLIST_WOW_METHOD_BITS=0 is 2547-only (empty viewer).
//   Mode1 cmd7 JoinRealm → JoinResponse Success.
//   Mode2 cmd8 Join: B39180 CHOICE 0 = Success Event + decoder addresses
//   (B383D0 list6, B38470 list18). CHOICE 1 = Failure (status byte) —
//   live :63405 480A01 bounced 445 B then closed. :65342 empty-structured
//   Soft-ACKed Join and showed the list, but Join retries had 0 routes.
//   Default structured = CHOICE 0 + 1 route from the Join realm id.
//   Do not Complete again on 4601. AUTHNET_MODE2_COMMAND3_ON_4601=after-join
//   sends that Complete only after Join Success.
constexpr uint32 AUTHNET_CHOICE_PRESENT_BITS = 1;
constexpr uint32 AUTHNET_CHOICE_TAG_BITS = 4;
constexpr uint32 AUTHNET_INVOKE_CHOICE_BITS = 8;
constexpr uint32 AUTHNET_WOW_METHOD_BITS = 8;
constexpr uint32 AUTHNET_GENERIC_RESULT_BITS = 1;
constexpr uint32 AUTHNET_NESTED_TRAILER_BITS = 32;
constexpr uint32 AUTHNET_NESTED_TRAILER_EXTRA_BITS = 11;
constexpr uint32 AUTHNET_MODULE_FUNCTION_BYTES_BITS = 6;
constexpr uint32 AUTHNET_MODULE_PARAMETERS_BITS = 32;
constexpr uint32 AUTHNET_MODULE_EXTRA_BITS = 3;
constexpr uint32 AUTHNET_FAILURE_CODE_BITS = 16;
constexpr uint32 AUTHNET_IDENTITY_LENGTH_BITS = 9;
constexpr uint32 AUTHNET_IDENTITY_LENGTH_BIAS = 3;

namespace AuthnetCreepAuth
{
    constexpr uint32 SetLaunchOptionRequest = 9;
}

namespace AuthnetLarvaInvoke
{
    constexpr uint32 Authentication = 0;
    constexpr uint32 WoW = 9;
}

namespace AuthnetLarvaWow
{
    constexpr uint32 RealmUpdate = 0;
    constexpr uint32 RealmList = 1;
    constexpr uint32 JoinResponse = 2;
}

namespace AuthnetLarvaAuth
{
    constexpr uint32 Module = 0;
    constexpr uint32 Success = 1;
    constexpr uint32 Failure = 2;
    constexpr uint32 ModuleBegin = 6;
    constexpr uint32 ModuleEnd = 7;
    constexpr uint32 GenerateSingleSignOnTokenResponse = 9;
    constexpr uint32 SetLaunchOptionResponse = 10;
}

namespace AuthnetGenericResult
{
    constexpr uint32 Success = 0;
    constexpr uint32 Failure = 1;
}

enum class AuthnetLogonResult : uint32
{
    Success = 0,
    Extra = 1,
    Failure = 2,
    Module = 9
};

// Values below are read directly out of client build 18414's own tables,
// not inferred - see the authnet client login probe research note.

enum class AuthnetLoginState : uint32
{
    INITIALIZED = 0x00,
    CONNECTING = 0x01,
    HANDSHAKING = 0x02,
    AUTHENTICATING = 0x03,
    AUTHENTICATED = 0x04,
    FAILED = 0x05,
    DOWNLOADFILE = 0x06,
    FIRST_SECURITY = 0x07,
    PIN = 0x08,
    PIN_WAIT = 0x09,
    MATRIX = 0x0A,
    MATRIX_WAIT = 0x0B,
    TOKEN = 0x0C,
    TOKEN_WAIT = 0x0D,
    CHECKINGVERSIONS = 0x0E,
    RESPONSE_CONNECTED = 0x0F,
    DISCONNECTED = 0x10,
    SURVEY = 0x11,
    OUTOFBAND_CHALLENGE = 0x12,
    OUTOFBAND_CHALLENGE_WAIT = 0x13
};

// Client-side event queue message types, drained by the login object's
// main-thread pump. Only entries whose handler has an unambiguous
// meaning are named; the rest are deliberately left out rather than
// guessed at.
enum class AuthnetClientEvent : uint32
{
    CONNECTED = 0x07,
    SURVEY = 0x0E,
    GAME_ACCOUNT_SELECT = 0x0C,
    OUTOFBAND_CHALLENGE = 0x10,
    RECONNECT = 0x11
};

#endif
