-- Restore missing 5.4.8 lessons on the Stormwind trainers that offer them.
-- Sources: Wowhead MoP Classic trainer lists, reference trainer data, and 5.4.8 SpellEffect.dbc.
-- Use teaching-spell IDs for profession ranks; do not add Classic-only spell IDs.
-- Keep additions NPC-specific: other trainers sharing the older lists retain their curriculum.
-- Recipe costs/prerequisites follow reference trainer data; Zen Master ranks cost 60 gold.
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`)
VALUES
(1317, 63746, 5000, 333, 225, 0), -- Enchant Boots - Lesser Accuracy
(1317, 103461, 22857, 333, 550, 0), -- Enchant Ring - Greater Agility
(1317, 103462, 22857, 333, 550, 0), -- Enchant Ring - Greater Intellect
(1317, 103463, 22857, 333, 550, 0), -- Enchant Ring - Greater Stamina
(1317, 103465, 22857, 333, 550, 0), -- Enchant Ring - Greater Strength
(1317, 104338, 22857, 333, 500, 0), -- Enchant Bracer - Mastery
(1317, 104385, 22857, 333, 525, 0), -- Enchant Bracer - Major Dodge
(1317, 104392, 22857, 333, 525, 0), -- Enchant Chest - Super Resilience
(1317, 104393, 22857, 333, 525, 0), -- Enchant Chest - Mighty Spirit
(1317, 104395, 22857, 333, 575, 0), -- Enchant Chest - Glorious Stats
(1317, 104397, 22857, 333, 550, 0), -- Enchant Chest - Superior Stamina
(1317, 104398, 22857, 333, 550, 0), -- Enchant Cloak - Accuracy
(1317, 104401, 22857, 333, 575, 0), -- Enchant Cloak - Greater Protection
(1317, 104403, 22857, 333, 525, 0), -- Enchant Cloak - Superior Intellect
(1317, 104404, 22857, 333, 500, 0), -- Enchant Cloak - Superior Critical Strike
(1317, 104407, 22857, 333, 550, 0), -- Enchant Boots - Greater Haste
(1317, 104408, 22857, 333, 525, 0), -- Enchant Boots - Greater Precision
(1317, 104409, 22857, 333, 550, 0), -- Enchant Boots - Blurred Speed
(1317, 104414, 22857, 333, 575, 0), -- Enchant Boots - Pandaren's Step
(1317, 104416, 22857, 333, 525, 0), -- Enchant Gloves - Greater Haste
(1317, 104417, 22857, 333, 550, 0), -- Enchant Gloves - Superior Expertise
(1317, 104419, 22857, 333, 575, 0), -- Enchant Gloves - Super Strength
(1317, 104420, 22857, 333, 575, 0), -- Enchant Gloves - Superior Mastery
(1317, 104425, 22857, 333, 550, 0), -- Enchant Weapon - Windsong
(1317, 104430, 22857, 333, 525, 0), -- Enchant Weapon - Elemental Force
(1317, 104440, 22857, 333, 575, 0), -- Enchant Weapon - Colossus
(1317, 104445, 22857, 333, 575, 0), -- Enchant Off-Hand - Major Intellect
(1317, 110401, 600000, 333, 500, 80), -- Enchanting
(1317, 116497, 22857, 333, 500, 0), -- Mysterious Essence
(1317, 116498, 22857, 333, 550, 0), -- Ethereal Shard
(1317, 116499, 22857, 333, 600, 0), -- Sha Crystal
(1317, 118237, 22857, 333, 535, 0), -- Mysterious Diffusion
(1317, 118238, 22857, 333, 585, 0), -- Ethereal Shatter
(1317, 118239, 22857, 333, 600, 0), -- Sha Shatter
(1317, 130758, 22857, 333, 575, 0), -- Enchant Shield - Greater Parry
(1346, 110427, 600000, 197, 500, 80), -- Tailoring
(1346, 125523, 24000, 197, 555, 0), -- Windwool Hood
(1346, 125524, 24000, 197, 535, 0), -- Windwool Shoulders
(1346, 125525, 24000, 197, 555, 0), -- Windwool Tunic
(1346, 125526, 24000, 197, 525, 0), -- Windwool Gloves
(1346, 125527, 24000, 197, 545, 0), -- Windwool Pants
(1346, 125528, 24000, 197, 525, 0), -- Windwool Bracers
(1346, 125529, 24000, 197, 545, 0), -- Windwool Boots
(1346, 125530, 24000, 197, 535, 0), -- Windwool Belt
(1346, 125551, 24000, 197, 500, 0), -- Bolt of Windwool Cloth
(1346, 125552, 24000, 197, 530, 0), -- Pearlescent Spellthread
(1346, 125553, 24000, 197, 540, 0), -- Cerulean Spellthread
(1346, 125557, 24000, 197, 550, 0), -- Imperial Silk
(1346, 130325, 24000, 197, 560, 0), -- Song of Harmony
(5499, 105208, 600000, 171, 500, 80), -- Alchemy
(5499, 114751, 228571, 171, 500, 0), -- Alchemist's Rejuvenation
(5499, 114752, 228571, 171, 500, 0), -- Master Healing Potion
(5499, 114786, 20000, 171, 300, 0), -- Alchemist's Flask
(5499, 136197, 100000, 171, 500, 0), -- Zen Alchemist Stone
(5511, 110398, 600000, 164, 500, 80), -- Blacksmithing
(5511, 122576, 91428, 164, 550, 0), -- Ghost-Forged Helm
(5511, 122577, 91428, 164, 540, 0), -- Ghost-Forged Shoulders
(5511, 122578, 91428, 164, 550, 0), -- Ghost-Forged Breastplate
(5511, 122579, 91428, 164, 525, 0), -- Ghost-Forged Gauntlets
(5511, 122580, 91428, 164, 530, 0), -- Ghost-Forged Legplates
(5511, 122581, 91428, 164, 500, 0), -- Ghost-Forged Bracers
(5511, 122582, 91428, 164, 530, 0), -- Ghost-Forged Boots
(5511, 122583, 91428, 164, 525, 0), -- Ghost-Forged Belt
(5511, 122633, 91428, 164, 500, 0), -- Ghostly Skeleton Key
(5511, 122635, 91428, 164, 540, 0), -- Lightsteel Shield
(5511, 122636, 91428, 164, 545, 0), -- Spiritguard Shield
(5511, 122637, 91428, 164, 545, 0), -- Forgewire Axe
(5511, 122638, 91428, 164, 545, 0), -- Ghost-Forged Blade
(5511, 122639, 91428, 164, 560, 0), -- Phantasmal Hammer
(5511, 122640, 91428, 164, 565, 0), -- Spiritblade Decimator
(5511, 122641, 91428, 164, 565, 0), -- Ghost Shard
(5518, 3979, 1200, 202, 180, 0), -- Accurate Scope
(5518, 39895, 10000, 202, 275, 0), -- Fused Wiring
(5518, 108789, 57142, 202, 550, 0), -- Phase Fingers
(5518, 109077, 57142, 202, 550, 0), -- Incendiary Fireworks Launcher
(5518, 109099, 57142, 202, 550, 0), -- Watergliding Jets
(5518, 110404, 600000, 202, 500, 80), -- Engineering
(5518, 126392, 57142, 202, 550, 0), -- Goblin Glider
(5518, 126731, 57142, 202, 550, 0), -- Synapse Springs
(5518, 127113, 57142, 202, 500, 0), -- Ghost Iron Bolts
(5518, 127114, 57142, 202, 500, 0), -- High-Explosive Gunpowder
(5518, 127115, 57142, 202, 560, 0), -- Lord Blastington's Scope of Doom
(5518, 127116, 57142, 202, 525, 0), -- Mirror Scope
(5518, 127124, 57142, 202, 500, 0), -- Locksmith's Powderkeg
(5518, 127127, 57142, 202, 525, 0), -- G91 Landshark
(5518, 127128, 57142, 202, 550, 0), -- Goblin Dragon Gun, Mark II
(5518, 127129, 57142, 202, 600, 0), -- Blingtron 4000
(5518, 127130, 57142, 202, 525, 0), -- Mist-Piercing Goggles
(5518, 127131, 57142, 202, 550, 0), -- Thermal Anvil
(5518, 127132, 57142, 202, 570, 0), -- Wormhole Generator: Pandaria
(5518, 127134, 57142, 202, 550, 0), -- Ghost Iron Dragonling
(5518, 127135, 57142, 202, 575, 0), -- Mechanical Pandaren Dragonling
(5518, 127136, 57142, 202, 525, 0), -- Big Game Hunter
(5518, 127137, 57142, 202, 565, 0), -- Long-Range Trillium Sniper
(5518, 127138, 182857, 202, 600, 0), -- Depleted-Kyparium Rocket
(5518, 127139, 182857, 202, 600, 0), -- Geosynchronous World Spinner
(5518, 131211, 57142, 202, 550, 0), -- Flashing Tinker's Gear
(5518, 131212, 57142, 202, 550, 0), -- Fractured Tinker's Gear
(5518, 131213, 57142, 202, 550, 0), -- Precise Tinker's Gear
(5518, 131214, 57142, 202, 550, 0), -- Quick Tinker's Gear
(5518, 131215, 57142, 202, 550, 0), -- Rigid Tinker's Gear
(5518, 131216, 57142, 202, 550, 0), -- Smooth Tinker's Gear
(5518, 131217, 57142, 202, 550, 0), -- Sparkling Tinker's Gear
(5518, 131218, 57142, 202, 550, 0), -- Subtle Tinker's Gear
(5518, 131353, 57142, 202, 500, 0), -- Pandaria Fireworks
(5518, 131563, 57142, 202, 550, 0), -- Tinker's Kit
(30713, 58313, 300, 773, 75, 0), -- Glyph of Flash of Light
(30713, 71101, 10000, 773, 250, 0), -- Glyph of Counterspell
(30713, 71102, 50000, 773, 375, 0), -- Glyph of Eternal Resolve
(30713, 89815, 80000, 773, 465, 0), -- Glyph of Colossus Smash
(30713, 110418, 600000, 773, 500, 80), -- Inscription
(30713, 111645, 222222, 773, 500, 0), -- Ink of Dreams
(30713, 111646, 222222, 773, 500, 0), -- Starlight Ink
(30713, 111830, 222222, 773, 600, 0), -- Darkmoon Card of Mists
(30713, 111908, 222222, 773, 560, 0), -- Inscribed Fan
(30713, 111909, 222222, 773, 560, 0), -- Inscribed Jade Fan
(30713, 111910, 222222, 773, 560, 0), -- Inscribed Red Fan
(30713, 111917, 222222, 773, 560, 0), -- Rain Poppy Staff
(30713, 111918, 222222, 773, 560, 0), -- Inscribed Crane Staff
(30713, 111919, 222222, 773, 560, 0), -- Inscribed Serpent Staff
(30713, 111920, 222222, 773, 560, 0), -- Ghost Iron Staff
(30713, 111921, 222222, 773, 560, 0), -- Inscribed Tiger Staff
(30713, 112045, 222222, 773, 580, 0), -- Runescroll of Fortitude III
(30713, 112437, 4500, 773, 220, 0), -- Glyph of Nimble Brew
(30713, 112440, 50000, 773, 355, 0), -- Glyph of Paralysis
(30713, 112442, 20000, 773, 320, 0), -- Glyph of Life Cocoon
(30713, 112444, 50000, 773, 370, 0), -- Glyph of Touch of Karma
(30713, 112450, 15000, 773, 295, 0), -- Glyph of Leer of the Ox
(30713, 112451, 750, 773, 155, 0), -- Glyph of Afterlife
(30713, 112452, 50000, 773, 360, 0), -- Glyph of Sparring
(30713, 112454, 50000, 773, 365, 0), -- Glyph of Detox
(30713, 112457, 10000, 773, 255, 0), -- Glyph of Fortifying Brew
(30713, 112458, 50000, 773, 380, 0), -- Glyph of Targeted Expulsion
(30713, 112462, 100000, 773, 485, 0), -- Glyph of Crackling Tiger Lightning
(30713, 112463, 100000, 773, 495, 0), -- Glyph of Flying Serpent Kick
(30713, 112469, 100000, 773, 490, 0), -- Glyph of Fighting Pose
(30713, 112883, 222222, 773, 500, 0), -- Tome of the Clear Mind
(30713, 112996, 555555, 773, 525, 0), -- Scroll of Wisdom
(30713, 124442, 80000, 773, 450, 0), -- Glyph of Aspect of the Beast
(30713, 124445, 80000, 773, 460, 0), -- Glyph of Fists of Fury
(30713, 124451, 50000, 773, 380, 0), -- Glyph of Zen Meditation
(30713, 124452, 80000, 773, 440, 0), -- Glyph of Renewing Mists
(30713, 124453, 80000, 773, 445, 0), -- Glyph of Spinning Crane Kick
(30713, 124455, 80000, 773, 465, 0), -- Glyph of Surging Mist
(30713, 124456, 80000, 773, 470, 0), -- Glyph of Touch of Death
(30713, 124459, 80000, 773, 455, 0), -- Glyph of Mind Flay
(30713, 124463, 50000, 773, 380, 0), -- Glyph of Fortuitous Spheres
(30713, 126801, 80000, 773, 440, 0), -- Glyph of Fetch
(30713, 126988, 55555, 773, 600, 0), -- Origami Crane
(30713, 126989, 55555, 773, 600, 0), -- Origami Frog
(30713, 126994, 222222, 773, 570, 0), -- Greater Ox Horn Inscription
(30713, 126995, 222222, 773, 570, 0), -- Greater Crane Wing Inscription
(30713, 126996, 222222, 773, 570, 0), -- Greater Tiger Claw Inscription
(30713, 126997, 222222, 773, 570, 0), -- Greater Tiger Fang Inscription
(30713, 127007, 55555, 773, 600, 0), -- Yu'lon Kite
(30713, 127009, 55555, 773, 600, 0), -- Chi-ji Kite
(30713, 127016, 222222, 773, 540, 0), -- Tiger Fang Inscription
(30713, 127017, 222222, 773, 540, 0), -- Tiger Claw Inscription
(30713, 127018, 222222, 773, 540, 0), -- Crane Wing Inscription
(30713, 127019, 222222, 773, 540, 0), -- Ox Horn Inscription
(30713, 127020, 222222, 773, 575, 0), -- Secret Tiger Fang Inscription
(30713, 127021, 222222, 773, 575, 0), -- Secret Tiger Claw Inscription
(30713, 127023, 222222, 773, 575, 0), -- Secret Crane Wing Inscription
(30713, 127024, 222222, 773, 575, 0), -- Secret Ox Horn Inscription
(30713, 127625, 10000, 773, 255, 0), -- Glyph of Lightspring
(44582, 107598, 45714, 755, 550, 0), -- Balanced Alexandrite
(44582, 107599, 45714, 755, 550, 0), -- Effulgent Alexandrite
(44582, 107600, 45714, 755, 560, 0), -- Energized Alexandrite
(44582, 107601, 45714, 755, 565, 0), -- Forceful Alexandrite
(44582, 107602, 45714, 755, 530, 0), -- Jagged Alexandrite
(44582, 107604, 45714, 755, 525, 0), -- Lightning Alexandrite
(44582, 107605, 45714, 755, 540, 0), -- Misty Alexandrite
(44582, 107606, 45714, 755, 530, 0), -- Nimble Alexandrite
(44582, 107607, 45714, 755, 555, 0), -- Piercing Alexandrite
(44582, 107608, 45714, 755, 570, 0), -- Puissant Alexandrite
(44582, 107609, 45714, 755, 575, 0), -- Radiant Alexandrite
(44582, 107610, 45714, 755, 535, 0), -- Regal Alexandrite
(44582, 107611, 45714, 755, 545, 0), -- Sensei's Alexandrite
(44582, 107612, 45714, 755, 540, 0), -- Shattered Alexandrite
(44582, 107613, 45714, 755, 555, 0), -- Steady Alexandrite
(44582, 107614, 45714, 755, 545, 0), -- Turbid Alexandrite
(44582, 107615, 45714, 755, 525, 0), -- Vivid Alexandrite
(44582, 107616, 45714, 755, 535, 0), -- Zen Alexandrite
(44582, 107617, 45714, 755, 525, 0), -- Rigid Lapis Lazuli
(44582, 107619, 45714, 755, 535, 0), -- Solid Lapis Lazuli
(44582, 107620, 45714, 755, 545, 0), -- Sparkling Lapis Lazuli
(44582, 107621, 45714, 755, 555, 0), -- Stormy Lapis Lazuli
(44582, 107622, 45714, 755, 570, 0), -- Bold Pandarian Garnet
(44582, 107623, 45714, 755, 560, 0), -- Brilliant Pandarian Garnet
(44582, 107624, 45714, 755, 530, 0), -- Delicate Pandarian Garnet
(44582, 107625, 45714, 755, 550, 0), -- Flashing Pandarian Garnet
(44582, 107626, 45714, 755, 540, 0), -- Precise Pandarian Garnet
(44582, 107627, 45714, 755, 550, 0), -- Accurate Roguestone
(44582, 107628, 45714, 755, 555, 0), -- Defender's Roguestone
(44582, 107630, 45714, 755, 525, 0), -- Etched Roguestone
(44582, 107631, 45714, 755, 540, 0), -- Glinting Roguestone
(44582, 107632, 45714, 755, 545, 0), -- Guardian's Roguestone
(44582, 107633, 45714, 755, 525, 0), -- Mysterious Roguestone
(44582, 107634, 45714, 755, 565, 0), -- Purified Roguestone
(44582, 107635, 45714, 755, 575, 0), -- Retaliating Roguestone
(44582, 107636, 45714, 755, 535, 0), -- Shifting Roguestone
(44582, 107637, 45714, 755, 535, 0), -- Sovereign Roguestone
(44582, 107638, 45714, 755, 560, 0), -- Timeless Roguestone
(44582, 107639, 45714, 755, 530, 0), -- Veiled Roguestone
(44582, 107640, 45714, 755, 535, 0), -- Fractured Sunstone
(44582, 107641, 45714, 755, 545, 0), -- Mystic Sunstone
(44582, 107642, 45714, 755, 555, 0), -- Quick Sunstone
(44582, 107643, 45714, 755, 565, 0), -- Smooth Sunstone
(44582, 107644, 45714, 755, 575, 0), -- Subtle Sunstone
(44582, 107645, 45714, 755, 550, 0), -- Adept Tiger Opal
(44582, 107646, 45714, 755, 525, 0), -- Artful Tiger Opal
(44582, 107647, 45714, 755, 545, 0), -- Champion's Tiger Opal
(44582, 107648, 45714, 755, 565, 0), -- Crafty Tiger Opal
(44582, 107649, 45714, 755, 525, 0), -- Deadly Tiger Opal
(44582, 107650, 45714, 755, 530, 0), -- Deft Tiger Opal
(44582, 107651, 45714, 755, 555, 0), -- Fierce Tiger Opal
(44582, 107652, 45714, 755, 535, 0), -- Fine Tiger Opal
(44582, 107653, 45714, 755, 555, 0), -- Inscribed Tiger Opal
(44582, 107654, 45714, 755, 530, 0), -- Keen Tiger Opal
(44582, 107655, 45714, 755, 540, 0), -- Lucent Tiger Opal
(44582, 107656, 45714, 755, 545, 0), -- Polished Tiger Opal
(44582, 107657, 45714, 755, 560, 0), -- Potent Tiger Opal
(44582, 107658, 45714, 755, 540, 0), -- Reckless Tiger Opal
(44582, 107659, 45714, 755, 575, 0), -- Resolute Tiger Opal
(44582, 107660, 45714, 755, 575, 0), -- Resplendent Tiger Opal
(44582, 107661, 45714, 755, 560, 0), -- Skillful Tiger Opal
(44582, 107662, 45714, 755, 570, 0), -- Splendid Tiger Opal
(44582, 107663, 45714, 755, 565, 0), -- Stalwart Tiger Opal
(44582, 107665, 45714, 755, 550, 0), -- Tenuous Tiger Opal
(44582, 107666, 45714, 755, 570, 0), -- Wicked Tiger Opal
(44582, 107667, 45714, 755, 535, 0), -- Willful Tiger Opal
(44582, 110421, 600000, 755, 500, 80), -- Jewelcrafting
(44582, 122661, 45714, 755, 500, 0), -- Ornate Band
(44582, 122662, 45714, 755, 510, 0), -- Shadowfire Necklace
(44582, 122663, 45714, 755, 525, 0), -- Scrying Roguestone
(44582, 130655, 45714, 755, 570, 0), -- Tense Roguestone
(44582, 130656, 45714, 755, 530, 0), -- Assassin's Roguestone
(44582, 131593, 45714, 755, 525, 0), -- River's Heart
(44582, 131686, 45714, 755, 550, 0), -- Primordial Ruby
(44582, 131688, 45714, 755, 535, 0), -- Wild Jade
(44582, 131690, 45714, 755, 535, 0), -- Vermilion Onyx
(44582, 131691, 45714, 755, 545, 0), -- Imperial Amethyst
(44582, 131695, 45714, 755, 525, 0), -- Sun's Radiance
(44582, 131759, 45714, 755, 550, 0) -- Secrets of the Stone
ON DUPLICATE KEY UPDATE `spell` = VALUES(`spell`);
