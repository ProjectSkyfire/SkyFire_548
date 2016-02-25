-- Ra-den creature text
-- http://www.wowhead.com/npc=69473/ra-den
DELETE FROM `creature_text` WHERE `entry` = 69473;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) values
(69473,1,0,"For millennia I have waited in darkness, in silence pierced only by the shrieks of the tormented, my dreams a waking nightmare.",14,0,0,0,0,35756,"Ra-den Intro 1"),
(69473,2,0,"I see the naive hope in your eyes. You... think you are my saviors? Wring that mocking pity from your hearts and focus it inwards, for you shall soon know my agony.",14,0,0,0,0,35757,"Ra-den Intro 2"),
(69473,3,0,"This twisted world is beyond redemption, beyond the reach of deluded heroes. The only answer to corruption is destruction. And that begins... now.",14,0,0,0,0,35758,"Ra-den Intro 3"),
(69473,4,0,"When I am finished with you, I shall not rest until this world is cleansed. Now, perish!",14,0,0,0,0,35754,"Ra-den Aggro"),
(69473,5,1,"Amusing. You really thought you could win.",14,0,0,0,0,35760,"Ra-den Kill 2"),
(69473,5,2,"Accept the inevitable.",14,0,0,0,0,35759,"Ra-den Kill 1"),
(69473,6,0,"You are the best this world has to offer? No wonder it has fallen to ruin.",14,0,0,0,0,35755,"Ra-den Berserk"),
(69473,7,0,"You still think victory possible? Fools!",14,0,0,0,0,35761,"Ra-den Phase 2"),
(69473,8,0,"Feel your own life essence betray you!",14,0,0,0,0,35762,"Ra-den Murderous Strike."),
(69473,9,0,"The touch of creation... and destruction.",14,0,0,0,0,35763,"Ra-den Summon Materials of Creation."),
(69473,10,0,"Ah, the familiar embrace of the storms...",14,0,0,0,0,35764,"Ra-den Power Vita."),
(69473,11,0,"This is but a taste of my power.",14,0,0,0,0,35765,"Ra-den Empower Anima"),
(69473,12,0,"Wait! I am... I am not your enemy. You are powerful, more powerful than he was, even... perhaps you are right. Perhaps there is still hope.
But there is a yawning chasm of darkness beneath you mortals, vast, endless, and all consuming. I do not believe that you can correct this doomed course.
But... you have earned the right to try. Farewell.",14,0,0,0,0,35766,"Ra-den Surrender.");
