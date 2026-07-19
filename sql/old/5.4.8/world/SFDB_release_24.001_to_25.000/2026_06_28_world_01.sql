-- Fix hover/bounce NPCs (birds, trikes with bad addon bytes, etc.)
-- Pair with core fix in Creature::UpdateMovementFlags (air-only + idle ground skip).
-- Generated: 2026-06-28 23:34:37
-- InhabitType rows: 163; bytes1 clear rows: 232
-- After apply: restart worldserver or .reload creature_template and respawn affected zones.

UPDATE creature_template SET InhabitType=5 WHERE entry=6377; -- was 3, Thunderhead Stagwing
UPDATE creature_template SET InhabitType=5 WHERE entry=8197; -- was 1, Chronalis
UPDATE creature_template SET InhabitType=5 WHERE entry=8198; -- was 1, Tick
UPDATE creature_template SET InhabitType=4 WHERE entry=11897; -- was 3, Duskwing
UPDATE creature_template SET InhabitType=4 WHERE entry=25445; -- was 5, Nerub''ar Corpse Harvester
UPDATE creature_template SET InhabitType=5 WHERE entry=25582; -- was 4, Scourged Flamespitter
UPDATE creature_template SET InhabitType=5 WHERE entry=26286; -- was 4, Emberwyrm
UPDATE creature_template SET InhabitType=5 WHERE entry=26841; -- was 4, Reanimated Frost Wyrm
UPDATE creature_template SET InhabitType=4 WHERE entry=27638; -- was 5, Azure Ring Guardian
UPDATE creature_template SET InhabitType=5 WHERE entry=27691; -- was 7, Frenzied Gargoyle
UPDATE creature_template SET InhabitType=4 WHERE entry=27925; -- was 5, Nozdormu
UPDATE creature_template SET InhabitType=4 WHERE entry=28276; -- was 5, Greater Ley-Whelp
UPDATE creature_template SET InhabitType=4 WHERE entry=28487; -- was 5, Val''kyr Battle-maiden
UPDATE creature_template SET InhabitType=5 WHERE entry=29708; -- was 7, Captive Proto-Drake
UPDATE creature_template SET InhabitType=4 WHERE entry=29754; -- was 3, Column Ornament
UPDATE creature_template SET InhabitType=4 WHERE entry=29792; -- was 3, Frostfeather Screecher
UPDATE creature_template SET InhabitType=4 WHERE entry=29793; -- was 3, Frostfeather Witch
UPDATE creature_template SET InhabitType=4 WHERE entry=29808; -- was 7, Stormcrest Hatchling
UPDATE creature_template SET InhabitType=4 WHERE entry=30013; -- was 3, Stormcrest Eagle
UPDATE creature_template SET InhabitType=4 WHERE entry=30235; -- was 3, Glory
UPDATE creature_template SET InhabitType=4 WHERE entry=30500; -- was 3, Argent Skytalon
UPDATE creature_template SET InhabitType=4 WHERE entry=32180; -- was 5, Tempus Wyrm
UPDATE creature_template SET InhabitType=4 WHERE entry=32185; -- was 5, Infinite Eradicator
UPDATE creature_template SET InhabitType=4 WHERE entry=32186; -- was 7, Infinite Timebreaker
UPDATE creature_template SET InhabitType=5 WHERE entry=32189; -- was 7, Skybreaker Recon Fighter
UPDATE creature_template SET InhabitType=5 WHERE entry=32323; -- was 7, Aldur''thar Sentry
UPDATE creature_template SET InhabitType=5 WHERE entry=32534; -- was 7, Scalesworn Elite
UPDATE creature_template SET InhabitType=7 WHERE entry=33186; -- was 4, Razorscale
UPDATE creature_template SET InhabitType=5 WHERE entry=33214; -- was 4, Mechanolift 304-A
UPDATE creature_template SET InhabitType=4 WHERE entry=34160; -- was 3, Watch Wind Rider
UPDATE creature_template SET InhabitType=5 WHERE entry=34295; -- was 3, Lord Magmathar
UPDATE creature_template SET InhabitType=5 WHERE entry=34832; -- was 4, Warsong War Rider
UPDATE creature_template SET InhabitType=5 WHERE entry=34886; -- was 3, Kalimdor Eagle
UPDATE creature_template SET InhabitType=5 WHERE entry=34897; -- was 3, Black Drake
UPDATE creature_template SET InhabitType=5 WHERE entry=36649; -- was 3, Malicion
UPDATE creature_template SET InhabitType=7 WHERE entry=36725; -- was 4, Nerub''ar Broodkeeper
UPDATE creature_template SET InhabitType=5 WHERE entry=37522; -- was 3, Romo''s Standard Bunny
UPDATE creature_template SET InhabitType=4 WHERE entry=37528; -- was 5, Spire Frostwyrm (Ambient)
UPDATE creature_template SET InhabitType=4 WHERE entry=37804; -- was 3, Kaja''Cola Balloon
UPDATE creature_template SET InhabitType=4 WHERE entry=37906; -- was 3, Imprisoned Soul
UPDATE creature_template SET InhabitType=7 WHERE entry=38421; -- was 4, Nightmare Cloud
UPDATE creature_template SET InhabitType=4 WHERE entry=40320; -- was 3, Valiona
UPDATE creature_template SET InhabitType=7 WHERE entry=41200; -- was 3, Generic Bunny - PRK
UPDATE creature_template SET InhabitType=5 WHERE entry=41266; -- was 7, Goblin Hot Air Balloon
UPDATE creature_template SET InhabitType=4 WHERE entry=41418; -- was 3, Krom''gar Hot Air Balloon
UPDATE creature_template SET InhabitType=4 WHERE entry=41987; -- was 3, Krom''gar Hot Air Balloon
UPDATE creature_template SET InhabitType=5 WHERE entry=42015; -- was 3, Gnomish Bomber
UPDATE creature_template SET InhabitType=7 WHERE entry=44261; -- was 3, Sharptalon Eagle
UPDATE creature_template SET InhabitType=5 WHERE entry=44481; -- was 3, Diseased Hawk
UPDATE creature_template SET InhabitType=7 WHERE entry=44483; -- was 3, Kirtonos the Herald
UPDATE creature_template SET InhabitType=7 WHERE entry=44586; -- was 3, Bad Intentions Target
UPDATE creature_template SET InhabitType=4 WHERE entry=44608; -- was 3, Agatha
UPDATE creature_template SET InhabitType=7 WHERE entry=44880; -- was 3, Sea Gull
UPDATE creature_template SET InhabitType=5 WHERE entry=45716; -- was 3, Orsis Survivor Vehicle
UPDATE creature_template SET InhabitType=5 WHERE entry=46024; -- was 3, Orsis Colossus Vehicle
UPDATE creature_template SET InhabitType=5 WHERE entry=46087; -- was 3, Orsis Colossus Vehicle
UPDATE creature_template SET InhabitType=4 WHERE entry=46416; -- was 3, Twilight Skyterror
UPDATE creature_template SET InhabitType=4 WHERE entry=46418; -- was 3, Lirastrasza
UPDATE creature_template SET InhabitType=7 WHERE entry=46419; -- was 4, Hurricane
UPDATE creature_template SET InhabitType=5 WHERE entry=46861; -- was 3, Nyxondra
UPDATE creature_template SET InhabitType=4 WHERE entry=47221; -- was 3, Schnottz Supply Chopper
UPDATE creature_template SET InhabitType=5 WHERE entry=47510; -- was 3, Twilight Skymaster Richtofen
UPDATE creature_template SET InhabitType=7 WHERE entry=47777; -- was 3, Animus
UPDATE creature_template SET InhabitType=4 WHERE entry=47806; -- was 3, Relentless Storm Initial Vehicle
UPDATE creature_template SET InhabitType=5 WHERE entry=48137; -- was 3, Monstrous Macaw
UPDATE creature_template SET InhabitType=5 WHERE entry=48138; -- was 3, Brilliant Clayscraper
UPDATE creature_template SET InhabitType=5 WHERE entry=48155; -- was 3, Saltspray Gull
UPDATE creature_template SET InhabitType=4 WHERE entry=48214; -- was 3, Schnottz Fighter
UPDATE creature_template SET InhabitType=4 WHERE entry=48623; -- was 3, Wisp
UPDATE creature_template SET InhabitType=4 WHERE entry=48624; -- was 3, Wisp
UPDATE creature_template SET InhabitType=4 WHERE entry=49128; -- was 3, Daschla
UPDATE creature_template SET InhabitType=4 WHERE entry=49129; -- was 3, Arthura
UPDATE creature_template SET InhabitType=5 WHERE entry=49251; -- was 3, Wild Gryphon
UPDATE creature_template SET InhabitType=5 WHERE entry=49844; -- was 3, Silky Moth
UPDATE creature_template SET InhabitType=7 WHERE entry=49928; -- was 3, Crimson Moth
UPDATE creature_template SET InhabitType=4 WHERE entry=50260; -- was 3, Gilnean Crow
UPDATE creature_template SET InhabitType=4 WHERE entry=50372; -- was 3, Aradne
UPDATE creature_template SET InhabitType=5 WHERE entry=51346; -- was 7, Orgrimmar Wind Rider
UPDATE creature_template SET InhabitType=4 WHERE entry=51371; -- was 3, Darnassus Hippogryph Rider
UPDATE creature_template SET InhabitType=7 WHERE entry=51372; -- was 3, Thunder Bluff Wind Rider
UPDATE creature_template SET InhabitType=7 WHERE entry=51383; -- was 3, Ironforge Gryphon Rider
UPDATE creature_template SET InhabitType=7 WHERE entry=51384; -- was 3, Forsaken Bat
UPDATE creature_template SET InhabitType=7 WHERE entry=52347; -- was 4, Zul''gurub Eagle
UPDATE creature_template SET InhabitType=7 WHERE entry=52373; -- was 3, Florawing Needler
UPDATE creature_template SET InhabitType=7 WHERE entry=52375; -- was 3, Florawing Needler
UPDATE creature_template SET InhabitType=7 WHERE entry=52376; -- was 3, Florawing Needler
UPDATE creature_template SET InhabitType=7 WHERE entry=52379; -- was 4, Venomtip Needler
UPDATE creature_template SET InhabitType=7 WHERE entry=52442; -- was 4, Florawing Hive Queen
UPDATE creature_template SET InhabitType=7 WHERE entry=53102; -- was 4, Inferno Hawk
UPDATE creature_template SET InhabitType=5 WHERE entry=53394; -- was 3, Blue Dragon
UPDATE creature_template SET InhabitType=5 WHERE entry=53396; -- was 3, Tarecgosa
UPDATE creature_template SET InhabitType=5 WHERE entry=54543; -- was 3, Time-Twisted Drake
UPDATE creature_template SET InhabitType=7 WHERE entry=55370; -- was 3, General Purpose Bunny ZTO
UPDATE creature_template SET InhabitType=4 WHERE entry=56199; -- was 3, Lightwing Hunter
UPDATE creature_template SET InhabitType=4 WHERE entry=56283; -- was 3, Tigerfly
UPDATE creature_template SET InhabitType=4 WHERE entry=56396; -- was 3, Hungry Bloodtalon
UPDATE creature_template SET InhabitType=4 WHERE entry=57195; -- was 3, Ik''thik Swiftclaw
UPDATE creature_template SET InhabitType=4 WHERE entry=57196; -- was 3, Ik''thik Wingblade
UPDATE creature_template SET InhabitType=4 WHERE entry=57216; -- was 3, Soaring Hunter
UPDATE creature_template SET InhabitType=7 WHERE entry=57864; -- was 4, Alurmi
UPDATE creature_template SET InhabitType=4 WHERE entry=58139; -- was 3, Observer Owl
UPDATE creature_template SET InhabitType=4 WHERE entry=58213; -- was 3, Windward Matriarch
UPDATE creature_template SET InhabitType=4 WHERE entry=58214; -- was 3, Windward Matriarch
UPDATE creature_template SET InhabitType=4 WHERE entry=58217; -- was 3, Windward Viper
UPDATE creature_template SET InhabitType=4 WHERE entry=58230; -- was 3, Sandy Petrel
UPDATE creature_template SET InhabitType=4 WHERE entry=58231; -- was 3, Dampnet Osprey
UPDATE creature_template SET InhabitType=4 WHERE entry=58367; -- was 3, Ik''thik Precursor
UPDATE creature_template SET InhabitType=4 WHERE entry=58457; -- was 3, Silkfeather Hawk
UPDATE creature_template SET InhabitType=4 WHERE entry=58910; -- was 3, Plainshawk
UPDATE creature_template SET InhabitType=4 WHERE entry=59154; -- was 3, Yak Statuette
UPDATE creature_template SET InhabitType=4 WHERE entry=59197; -- was 3, Canopy Screecher
UPDATE creature_template SET InhabitType=4 WHERE entry=59384; -- was 3, Bluff Hawk
UPDATE creature_template SET InhabitType=4 WHERE entry=59409; -- was 3, Hozen Swing
UPDATE creature_template SET InhabitType=4 WHERE entry=59768; -- was 3, Jadeglow Wasp
UPDATE creature_template SET InhabitType=4 WHERE entry=59770; -- was 3, Shrine Fly
UPDATE creature_template SET InhabitType=4 WHERE entry=59774; -- was 3, Ivory Starling
UPDATE creature_template SET InhabitType=4 WHERE entry=59984; -- was 3, Trained Jibberwing
UPDATE creature_template SET InhabitType=4 WHERE entry=60027; -- was 3, Prisoner Bindings
UPDATE creature_template SET InhabitType=4 WHERE entry=60166; -- was 3, Cloudrunner Leng
UPDATE creature_template SET InhabitType=4 WHERE entry=60167; -- was 3, Skydancer Shun
UPDATE creature_template SET InhabitType=4 WHERE entry=60200; -- was 3, Emerald Lory
UPDATE creature_template SET InhabitType=4 WHERE entry=60862; -- was 3, Steam Bunny
UPDATE creature_template SET InhabitType=4 WHERE entry=60866; -- was 3, Upland Eagle
UPDATE creature_template SET InhabitType=4 WHERE entry=60920; -- was 3, Dafeng
UPDATE creature_template SET InhabitType=4 WHERE entry=62267; -- was 3, Wild Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62309; -- was 3, Hei Feng
UPDATE creature_template SET InhabitType=4 WHERE entry=62311; -- was 3, Shan''ze Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62386; -- was 3, Sapfly
UPDATE creature_template SET InhabitType=4 WHERE entry=62448; -- was 3, Onyx Stormclaw
UPDATE creature_template SET InhabitType=4 WHERE entry=62576; -- was 3, Sik''thik Drone
UPDATE creature_template SET InhabitType=4 WHERE entry=62584; -- was 3, Shan''ze Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62585; -- was 3, Shan''ze Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62586; -- was 3, Shan''ze Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62597; -- was 3, Wild Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62598; -- was 3, Wild Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=62599; -- was 3, Wild Cloudrunner
UPDATE creature_template SET InhabitType=4 WHERE entry=63176; -- was 3, Ik''thik Egg-Drone
UPDATE creature_template SET InhabitType=4 WHERE entry=63289; -- was 3, Luyu Moth
UPDATE creature_template SET InhabitType=4 WHERE entry=63361; -- was 3, Mei Li Sparkler
UPDATE creature_template SET InhabitType=7 WHERE entry=63517; -- was 3, The Spring Drifter
UPDATE creature_template SET InhabitType=4 WHERE entry=63980; -- was 3, Amber Moth
UPDATE creature_template SET InhabitType=4 WHERE entry=64403; -- was 3, Alani
UPDATE creature_template SET InhabitType=4 WHERE entry=64411; -- was 3, Zandalari Pterrorwing
UPDATE creature_template SET InhabitType=4 WHERE entry=64554; -- was 3, Zhao-Ren
UPDATE creature_template SET InhabitType=4 WHERE entry=64995; -- was 3, Adjunct Sek''ot
UPDATE creature_template SET InhabitType=4 WHERE entry=65204; -- was 3, Effervescent Glowfly
UPDATE creature_template SET InhabitType=4 WHERE entry=65559; -- was 3, Vision of Zhao-Ren
UPDATE creature_template SET InhabitType=4 WHERE entry=65560; -- was 3, Vision of Dafeng
UPDATE creature_template SET InhabitType=4 WHERE entry=65573; -- was 3, Ik''thik Warrior
UPDATE creature_template SET InhabitType=4 WHERE entry=65575; -- was 3, Vor''thik Swarmborn
UPDATE creature_template SET InhabitType=4 WHERE entry=65608; -- was 3, [DNT] Fireflies
UPDATE creature_template SET InhabitType=4 WHERE entry=65757; -- was 3, Azure Flutterwing
UPDATE creature_template SET InhabitType=4 WHERE entry=65759; -- was 3, Iridescent Flutterwing
UPDATE creature_template SET InhabitType=4 WHERE entry=65760; -- was 3, Sun-Touched Flutterwing
UPDATE creature_template SET InhabitType=4 WHERE entry=65787; -- was 3, Zan''thik Resonator
UPDATE creature_template SET InhabitType=4 WHERE entry=65799; -- was 3, Fluttering Swallowtail
UPDATE creature_template SET InhabitType=4 WHERE entry=65802; -- was 3, Krasari Hissing Wasp
UPDATE creature_template SET InhabitType=4 WHERE entry=65814; -- was 3, Krasari Hissing Waspling
UPDATE creature_template SET InhabitType=4 WHERE entry=65984; -- was 3, Tiny Mutated Silkmoth
UPDATE creature_template SET InhabitType=4 WHERE entry=66194; -- was 3, Waste Scavenger
UPDATE creature_template SET InhabitType=4 WHERE entry=66319; -- was 3, Fear-Touched Moth
UPDATE creature_template SET InhabitType=4 WHERE entry=66451; -- was 3, Gilded Tern
UPDATE creature_template SET InhabitType=7 WHERE entry=67160; -- was 3, Klaxxi Vess-Guard

UPDATE creature_template_addon SET bytes1=0 WHERE entry=6377; -- Thunderhead Stagwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=8197; -- Chronalis
UPDATE creature_template_addon SET bytes1=0 WHERE entry=8198; -- Tick
UPDATE creature_template_addon SET bytes1=0 WHERE entry=11897; -- Duskwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=23680; -- Plagued Proto-Dragon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=25445; -- Nerub''ar Corpse Harvester
UPDATE creature_template_addon SET bytes1=0 WHERE entry=25582; -- Scourged Flamespitter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=26286; -- Emberwyrm
UPDATE creature_template_addon SET bytes1=0 WHERE entry=26841; -- Reanimated Frost Wyrm
UPDATE creature_template_addon SET bytes1=0 WHERE entry=27629; -- Wyrmrest Defender
UPDATE creature_template_addon SET bytes1=0 WHERE entry=27638; -- Azure Ring Guardian
UPDATE creature_template_addon SET bytes1=0 WHERE entry=27691; -- Frenzied Gargoyle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=27925; -- Nozdormu
UPDATE creature_template_addon SET bytes1=0 WHERE entry=28093; -- Sholazar Tickbird
UPDATE creature_template_addon SET bytes1=0 WHERE entry=28276; -- Greater Ley-Whelp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=28487; -- Val''kyr Battle-maiden
UPDATE creature_template_addon SET bytes1=0 WHERE entry=28859; -- Malygos
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29333; -- Onslaught Gryphon Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29488; -- Scourge Gryphon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29501; -- Scourge Gryphon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29570; -- Nascent Val''kyr
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29625; -- Hyldsmeet Proto-Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29708; -- Captive Proto-Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29754; -- Column Ornament
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29792; -- Frostfeather Screecher
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29793; -- Frostfeather Witch
UPDATE creature_template_addon SET bytes1=0 WHERE entry=29808; -- Stormcrest Hatchling
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30013; -- Stormcrest Eagle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30090; -- Vortex
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30118; -- Portal (Malygos)
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30235; -- Glory
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30446; -- Frostfloe Rift
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30454; -- Frostfloe Deep Stalker
UPDATE creature_template_addon SET bytes1=0 WHERE entry=30500; -- Argent Skytalon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=31263; -- Carrion Hunter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=31396; -- Val''kyr Taskmistress
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32180; -- Tempus Wyrm
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32185; -- Infinite Eradicator
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32186; -- Infinite Timebreaker
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32189; -- Skybreaker Recon Fighter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32265; -- Northrend Daily Dungeon Image Bunny
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32323; -- Aldur''thar Sentry
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32358; -- Fumblub Gearwind
UPDATE creature_template_addon SET bytes1=0 WHERE entry=32534; -- Scalesworn Elite
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33115; -- Lor''danel Sentinel
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33134; -- Sara
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33186; -- Razorscale
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33214; -- Mechanolift 304-A
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33280; -- Voice of Yogg-Saron
UPDATE creature_template_addon SET bytes1=0 WHERE entry=33721; -- Lore Keeper Projection Unit
UPDATE creature_template_addon SET bytes1=0 WHERE entry=34160; -- Watch Wind Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=34295; -- Lord Magmathar
UPDATE creature_template_addon SET bytes1=0 WHERE entry=34832; -- Warsong War Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=34886; -- Kalimdor Eagle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=34897; -- Black Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36640; -- Sable Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36649; -- Malicion
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36661; -- Rimefang
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36725; -- Nerub''ar Broodkeeper
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36794; -- Scourgelord Tyrannus
UPDATE creature_template_addon SET bytes1=0 WHERE entry=36852; -- Skychaser Hippogryph
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37522; -- Romo''s Standard Bunny
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37528; -- Spire Frostwyrm (Ambient)
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37804; -- Kaja''Cola Balloon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37906; -- Imprisoned Soul
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37950; -- Valithria Dreamwalker
UPDATE creature_template_addon SET bytes1=0 WHERE entry=37985; -- Dream Cloud
UPDATE creature_template_addon SET bytes1=0 WHERE entry=38421; -- Nightmare Cloud
UPDATE creature_template_addon SET bytes1=0 WHERE entry=39390; -- Twilight Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=40081; -- Orb Carrier
UPDATE creature_template_addon SET bytes1=0 WHERE entry=40183; -- Cave In Stalker
UPDATE creature_template_addon SET bytes1=0 WHERE entry=40320; -- Valiona
UPDATE creature_template_addon SET bytes1=0 WHERE entry=41095; -- Twilight Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=41200; -- Generic Bunny - PRK
UPDATE creature_template_addon SET bytes1=0 WHERE entry=41266; -- Goblin Hot Air Balloon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=41418; -- Krom''gar Hot Air Balloon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=41987; -- Krom''gar Hot Air Balloon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=42015; -- Gnomish Bomber
UPDATE creature_template_addon SET bytes1=0 WHERE entry=42608; -- Battered Red Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44261; -- Sharptalon Eagle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44481; -- Diseased Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44483; -- Kirtonos the Herald
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44586; -- Bad Intentions Target
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44608; -- Agatha
UPDATE creature_template_addon SET bytes1=0 WHERE entry=44880; -- Sea Gull
UPDATE creature_template_addon SET bytes1=0 WHERE entry=45475; -- Plague Disseminator
UPDATE creature_template_addon SET bytes1=0 WHERE entry=45716; -- Orsis Survivor Vehicle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46024; -- Orsis Colossus Vehicle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46087; -- Orsis Colossus Vehicle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46416; -- Twilight Skyterror
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46418; -- Lirastrasza
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46419; -- Hurricane
UPDATE creature_template_addon SET bytes1=0 WHERE entry=46861; -- Nyxondra
UPDATE creature_template_addon SET bytes1=0 WHERE entry=47221; -- Schnottz Supply Chopper
UPDATE creature_template_addon SET bytes1=0 WHERE entry=47510; -- Twilight Skymaster Richtofen
UPDATE creature_template_addon SET bytes1=0 WHERE entry=47680; -- Rustberg Gull
UPDATE creature_template_addon SET bytes1=0 WHERE entry=47777; -- Animus
UPDATE creature_template_addon SET bytes1=0 WHERE entry=47806; -- Relentless Storm Initial Vehicle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48137; -- Monstrous Macaw
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48138; -- Brilliant Clayscraper
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48155; -- Saltspray Gull
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48214; -- Schnottz Fighter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48523; -- Battered Red Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48623; -- Wisp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=48624; -- Wisp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49128; -- Daschla
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49129; -- Arthura
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49251; -- Wild Gryphon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49842; -- Forest Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49844; -- Silky Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=49928; -- Crimson Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=50260; -- Gilnean Crow
UPDATE creature_template_addon SET bytes1=0 WHERE entry=50372; -- Aradne
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51346; -- Orgrimmar Wind Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51348; -- Stormwind Gryphon Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51371; -- Darnassus Hippogryph Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51372; -- Thunder Bluff Wind Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51383; -- Ironforge Gryphon Rider
UPDATE creature_template_addon SET bytes1=0 WHERE entry=51384; -- Forsaken Bat
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52347; -- Zul''gurub Eagle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52373; -- Florawing Needler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52375; -- Florawing Needler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52376; -- Florawing Needler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52377; -- Florawing Needler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52379; -- Venomtip Needler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52419; -- Tiki Torch
UPDATE creature_template_addon SET bytes1=0 WHERE entry=52442; -- Florawing Hive Queen
UPDATE creature_template_addon SET bytes1=0 WHERE entry=53102; -- Inferno Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=53394; -- Blue Dragon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=53396; -- Tarecgosa
UPDATE creature_template_addon SET bytes1=0 WHERE entry=53474; -- Web Rip
UPDATE creature_template_addon SET bytes1=0 WHERE entry=53648; -- Inferno Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=54020; -- General Purpose Bunny JMF (Look 2 - Flying, Huge AOI)
UPDATE creature_template_addon SET bytes1=0 WHERE entry=54543; -- Time-Twisted Drake
UPDATE creature_template_addon SET bytes1=0 WHERE entry=54787; -- Huo
UPDATE creature_template_addon SET bytes1=0 WHERE entry=55370; -- General Purpose Bunny ZTO
UPDATE creature_template_addon SET bytes1=0 WHERE entry=55650; -- Shang Xi''s Hot Air Balloon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=55786; -- Zhao-Ren
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56171; -- Great White Plainshawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56199; -- Lightwing Hunter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56201; -- Orchard Wasp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56283; -- Tigerfly
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56396; -- Hungry Bloodtalon
UPDATE creature_template_addon SET bytes1=0 WHERE entry=56754; -- Azure Serpent
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57195; -- Ik''thik Swiftclaw
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57196; -- Ik''thik Wingblade
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57216; -- Soaring Hunter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57422; -- Mutated Silkmoth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57445; -- War Serpent
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57779; -- Huo
UPDATE creature_template_addon SET bytes1=0 WHERE entry=57864; -- Alurmi
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58139; -- Observer Owl
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58213; -- Windward Matriarch
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58214; -- Windward Matriarch
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58217; -- Windward Viper
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58230; -- Sandy Petrel
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58231; -- Dampnet Osprey
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58362; -- Orchard Beehive
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58363; -- Honeykeeper
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58367; -- Ik''thik Precursor
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58457; -- Silkfeather Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=58910; -- Plainshawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59112; -- Dawnfeather Flyer
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59154; -- Yak Statuette
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59197; -- Canopy Screecher
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59235; -- Albino Canopy Screecher
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59384; -- Bluff Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59409; -- Hozen Swing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59528; -- Cliff Kite
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59641; -- Monstrous Plainshawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59700; -- Northwind Hawk
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59757; -- Duskstalker
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59768; -- Jadeglow Wasp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59770; -- Shrine Fly
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59774; -- Ivory Starling
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59785; -- Singing Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59788; -- Spirit Darter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=59984; -- Trained Jibberwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60027; -- Prisoner Bindings
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60166; -- Cloudrunner Leng
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60167; -- Skydancer Shun
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60196; -- Dusky Lory
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60198; -- Crimson Lory
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60200; -- Emerald Lory
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60488; -- Water Spout Bunny
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60862; -- Steam Bunny
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60866; -- Upland Eagle
UPDATE creature_template_addon SET bytes1=0 WHERE entry=60920; -- Dafeng
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62091; -- Sik''thik Flyer
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62267; -- Wild Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62309; -- Hei Feng
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62311; -- Shan''ze Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62386; -- Sapfly
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62448; -- Onyx Stormclaw
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62576; -- Sik''thik Drone
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62584; -- Shan''ze Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62585; -- Shan''ze Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62586; -- Shan''ze Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62597; -- Wild Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62598; -- Wild Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=62599; -- Wild Cloudrunner
UPDATE creature_template_addon SET bytes1=0 WHERE entry=63176; -- Ik''thik Egg-Drone
UPDATE creature_template_addon SET bytes1=0 WHERE entry=63289; -- Luyu Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=63361; -- Mei Li Sparkler
UPDATE creature_template_addon SET bytes1=0 WHERE entry=63517; -- The Spring Drifter
UPDATE creature_template_addon SET bytes1=0 WHERE entry=63980; -- Amber Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=64403; -- Alani
UPDATE creature_template_addon SET bytes1=0 WHERE entry=64411; -- Zandalari Pterrorwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=64554; -- Zhao-Ren
UPDATE creature_template_addon SET bytes1=0 WHERE entry=64995; -- Adjunct Sek''ot
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65204; -- Effervescent Glowfly
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65205; -- Gilded Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65335; -- War Serpent
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65559; -- Vision of Zhao-Ren
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65560; -- Vision of Dafeng
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65573; -- Ik''thik Warrior
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65575; -- Vor''thik Swarmborn
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65608; -- [DNT] Fireflies
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65757; -- Azure Flutterwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65759; -- Iridescent Flutterwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65760; -- Sun-Touched Flutterwing
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65787; -- Zan''thik Resonator
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65799; -- Fluttering Swallowtail
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65802; -- Krasari Hissing Wasp
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65814; -- Krasari Hissing Waspling
UPDATE creature_template_addon SET bytes1=0 WHERE entry=65984; -- Tiny Mutated Silkmoth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=66194; -- Waste Scavenger
UPDATE creature_template_addon SET bytes1=0 WHERE entry=66319; -- Fear-Touched Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=66451; -- Gilded Tern
UPDATE creature_template_addon SET bytes1=0 WHERE entry=66514; -- Fluttering Moth
UPDATE creature_template_addon SET bytes1=0 WHERE entry=67160; -- Klaxxi Vess-Guard
UPDATE creature_template_addon SET bytes1=0 WHERE entry=67237; -- General Purpose Bunny JMF (Look 2 - Flying, Gigantic AOI)
