# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
"""Version 2 login result slots; kept compatible with PlayerLoginQueryIndex."""

LOGIN_PLAN = (
    ('CHAR_SEL_CHARACTER', 'guid'),  # 0
    ('CHAR_SEL_GROUP_MEMBER', 'guid'),  # 1
    ('CHAR_SEL_CHARACTER_INSTANCE', 'guid'),  # 2
    ('CHAR_SEL_CHARACTER_AURAS', 'guid'),  # 3
    ('CHAR_SEL_CHARACTER_SPELL', 'guid'),  # 4
    ('CHAR_SEL_CHARACTER_QUESTSTATUS', 'guid'),  # 5
    ('CHAR_SEL_CHARACTER_DAILYQUESTSTATUS', 'guid'),  # 6
    ('CHAR_SEL_CHARACTER_REPUTATION', 'guid'),  # 7
    ('CHAR_SEL_CHARACTER_INVENTORY', 'guid'),  # 8
    ('CHAR_SEL_CHARACTER_ACTIONS', 'guid'),  # 9
    ('CHAR_SEL_CHARACTER_MAILCOUNT', 'mail'),  # 10
    ('CHAR_SEL_CHARACTER_MAILDATE', 'guid'),  # 11
    ('CHAR_SEL_CHARACTER_SOCIALLIST', 'guid'),  # 12
    ('CHAR_SEL_CHARACTER_HOMEBIND', 'guid'),  # 13
    ('CHAR_SEL_CHARACTER_SPELLCOOLDOWNS', 'guid'),  # 14
    ('CHAR_SEL_CHARACTER_DECLINEDNAMES', 'guid'),  # 15
    ('CHAR_SEL_GUILD_MEMBER', 'guid'),  # 16
    ('CHAR_SEL_CHARACTER_SPELL_CHARGES', 'guid'),  # 17
    ('CHAR_SEL_CHARACTER_ACHIEVEMENTS', 'guid'),  # 18
    ('CHAR_SEL_CHARACTER_CRITERIAPROGRESS', 'guid'),  # 19
    ('CHAR_SEL_CHARACTER_EQUIPMENTSETS', 'guid'),  # 20
    ('CHAR_SEL_CHARACTER_BGDATA', 'guid'),  # 21
    ('CHAR_SEL_CHARACTER_GLYPHS', 'guid'),  # 22
    ('CHAR_SEL_CHARACTER_TALENTS', 'guid'),  # 23
    ('CHAR_SEL_PLAYER_ACCOUNT_DATA', 'guid'),  # 24
    ('CHAR_SEL_CHARACTER_SKILLS', 'guid'),  # 25
    ('CHAR_SEL_CHARACTER_WEEKLYQUESTSTATUS', 'guid'),  # 26
    ('CHAR_SEL_CHARACTER_RANDOMBG', 'guid'),  # 27
    ('CHAR_SEL_CHARACTER_BANNED', 'guid'),  # 28
    ('CHAR_SEL_CHARACTER_QUESTSTATUSREW', 'guid'),  # 29
    ('CHAR_SEL_ACCOUNT_INSTANCELOCKTIMES', 'account'),  # 30
    ('CHAR_SEL_CHARACTER_SEASONALQUESTSTATUS', 'guid'),  # 31
    ('CHAR_SEL_CHARACTER_MONTHLYQUESTSTATUS', 'guid'),  # 32
    ('CHAR_SEL_CHAR_VOID_STORAGE', 'guid'),  # 33
    ('CHAR_SEL_PLAYER_CURRENCY', 'guid'),  # 34
    ('CHAR_SEL_CHAR_CUF_PROFILES', 'guid'),  # 35
    ('CHAR_SEL_ACCOUNT_BATTLE_PETS', 'account'),  # 36
    ('CHAR_SEL_ACCOUNT_BATTLE_PET_SLOTS', 'account'),  # 37
    ('CHAR_SEL_CHARACTER_QUEST_OBJECTIVE_STATUS', 'guid'),  # 38
    ('CHAR_SEL_CHAR_RESEARCH_DIGSITES', 'guid'),  # 39
    ('CHAR_SEL_CHAR_RESEARCH_HISTORY', 'guid'),  # 40
    ('CHAR_SEL_CHAR_RESEARCH_PROJECTS', 'guid'),  # 41
)
