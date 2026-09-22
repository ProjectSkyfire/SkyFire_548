/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Player.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DisableMgr.h"
#include "GameEventMgr.h"
#include "GameObject.h"
#include "GossipDef.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Map.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "QuestDef.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "UpdateData.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void Player::PrepareQuestMenu(uint64 guid)
{
    QuestRelationBounds objectQR;
    QuestRelationBounds objectQIR;

    // pets also can have quests
    Creature* creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid);
    if (creature)
    {
        objectQR = sObjectMgr->GetCreatureQuestRelationBounds(creature->GetEntry());
        objectQIR = sObjectMgr->GetCreatureQuestInvolvedRelationBounds(creature->GetEntry());
    }
    else
    {
        //we should obtain map pointer from GetMap() in 99% of cases. Special case
        //only for quests which cast teleport spells on player
        Map* _map = IsInWorld() ? GetMap() : sMapMgr->FindMap(GetMapId(), GetInstanceId());
        ASSERT(_map);
        GameObject* gameObject = _map->GetGameObject(guid);
        if (gameObject)
        {
            objectQR = sObjectMgr->GetGOQuestRelationBounds(gameObject->GetEntry());
            objectQIR = sObjectMgr->GetGOQuestInvolvedRelationBounds(gameObject->GetEntry());
        }
        else
            return;
    }

    QuestMenu& qm = PlayerTalkClass->GetQuestMenu();
    qm.ClearMenu();

    for (QuestRelations::const_iterator i = objectQIR.first; i != objectQIR.second; ++i)
    {
        uint32 quest_id = i->second;
        QuestStatus status = GetQuestStatus(quest_id);
        if (status == QUEST_STATUS_COMPLETE)
            qm.AddMenuItem(quest_id, 4);
        else if (status == QUEST_STATUS_INCOMPLETE)
            qm.AddMenuItem(quest_id, 4);
        //else if (status == QUEST_STATUS_AVAILABLE)
        //    qm.AddMenuItem(quest_id, 2);
    }

    for (QuestRelations::const_iterator i = objectQR.first; i != objectQR.second; ++i)
    {
        uint32 quest_id = i->second;
        Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id);
        if (!quest)
            continue;

        if (!CanTakeQuest(quest, false))
            continue;

        if (quest->IsAutoComplete())
            qm.AddMenuItem(quest_id, 4);
        else if (GetQuestStatus(quest_id) == QUEST_STATUS_NONE)
            qm.AddMenuItem(quest_id, 2);
    }
}

void Player::SendPreparedQuest(uint64 guid)
{
    QuestMenu& questMenu = PlayerTalkClass->GetQuestMenu();
    if (questMenu.Empty())
        return;

    // single element case
    if (questMenu.GetMenuItemCount() == 1)
    {
        QuestMenuItem const& qmi0 = questMenu.GetItem(0);
        uint32 questId = qmi0.QuestId;

        // Auto open -- maybe also should verify there is no greeting
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
        {
            if (qmi0.QuestIcon == 4)
                PlayerTalkClass->SendQuestGiverRequestItems(quest, guid, CanRewardQuest(quest, false), true);
            // Send completable on repeatable and autoCompletable quest if player don't have quest
            /// @todo verify if check for !quest->IsDaily() is really correct (possibly not)
            else
            {
                Object* object = ObjectAccessor::GetObjectByTypeMask(*this, guid, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_ITEM);
                if (!object || (!object->hasQuest(questId) && !object->hasInvolvedQuest(questId)))
                {
                    PlayerTalkClass->SendCloseGossip();
                    return;
                }

                if (quest->IsAutoAccept() && CanAddQuest(quest, true) && CanTakeQuest(quest, true))
                {
                    AddQuest(quest, object);
                    if (CanCompleteQuest(questId))
                        CompleteQuest(questId);
                }

                if ((quest->IsAutoComplete() && quest->IsRepeatable() && !quest->IsDailyOrWeekly()) || quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
                    PlayerTalkClass->SendQuestGiverRequestItems(quest, guid, CanCompleteRepeatableQuest(quest), true);
                else
                    PlayerTalkClass->SendQuestGiverQuestDetails(quest, guid, true);
            }
        }
    }
    // multiple entries
    else
    {
        QEmote qe;
        qe._Delay = 0;
        qe._Emote = 0;
        std::string title = "";

        // need pet case for some quests
        Creature* creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid);
        if (creature)
        {
            uint32 textid = GetGossipTextId(creature);
            GossipText const* gossiptext = sObjectMgr->GetGossipText(textid);
            if (!gossiptext)
            {
                qe._Delay = 0;                              //TEXTEMOTE_MESSAGE;              //zyg: player emote
                qe._Emote = 0;                              //TEXTEMOTE_HELLO;                //zyg: NPC emote
                title = "";
            }
            else
            {
                qe = gossiptext->Options[0].Emotes[0];

                if (!gossiptext->Options[0].Text_0.empty())
                {
                    title = gossiptext->Options[0].Text_0;

                    int loc_idx = GetSession()->GetSessionDbLocaleIndex();
                    if (loc_idx >= 0)
                        if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textid))
                            ObjectMgr::GetLocaleString(nl->Text_0[0], loc_idx, title);
                }
                else
                {
                    title = gossiptext->Options[0].Text_1;

                    int loc_idx = GetSession()->GetSessionDbLocaleIndex();
                    if (loc_idx >= 0)
                        if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textid))
                            ObjectMgr::GetLocaleString(nl->Text_1[0], loc_idx, title);
                }
            }
        }
        PlayerTalkClass->SendQuestGiverQuestList(qe, title, guid);
    }
}

bool Player::IsActiveQuest(uint32 quest_id) const
{
    return m_QuestStatus.find(quest_id) != m_QuestStatus.end();
}

Quest const* Player::GetNextQuest(uint64 guid, Quest const* quest)
{
    QuestRelationBounds objectQR;
    uint32 nextQuestID = quest->GetNextQuestInChain();

    switch (GUID_HIPART(guid))
    {
        case HIGHGUID_PLAYER:
            // Remote autocomplete / auto-submit turn-in (no NPC GUID).
            if (!quest->HasFlag(QUEST_FLAGS_PLAYER_CAST_ACCEPT) &&
                !quest->HasFlag(QUEST_FLAGS_AUTOCOMPLETE) &&
                !quest->HasFlag(QUEST_FLAGS_PLAYER_CAST_COMPLETE))
                return NULL;
            return sObjectMgr->GetQuestTemplate(nextQuestID);
        case HIGHGUID_UNIT:
        case HIGHGUID_PET:
        case HIGHGUID_VEHICLE:
        {
            if (Creature* creature = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, guid))
                objectQR = sObjectMgr->GetCreatureQuestRelationBounds(creature->GetEntry());
            else
                return NULL;
            break;
        }
        case HIGHGUID_GAMEOBJECT:
        {
            //we should obtain map pointer from GetMap() in 99% of cases. Special case
            //only for quests which cast teleport spells on player
            Map* _map = IsInWorld() ? GetMap() : sMapMgr->FindMap(GetMapId(), GetInstanceId());
            ASSERT(_map);
            if (GameObject* gameObject = _map->GetGameObject(guid))
                objectQR = sObjectMgr->GetGOQuestRelationBounds(gameObject->GetEntry());
            else
                return NULL;
            break;
        }
        default:
            return NULL;
    }

    // for unit and go state
    for (QuestRelations::const_iterator itr = objectQR.first; itr != objectQR.second; ++itr)
    {
        if (itr->second == nextQuestID)
            return sObjectMgr->GetQuestTemplate(nextQuestID);
    }

    return NULL;
}

bool Player::CanSeeStartQuest(Quest const* quest)
{
    if (!DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, quest->GetQuestId(), this) && SatisfyQuestClass(quest, false) && SatisfyQuestRace(quest, false) &&
        SatisfyQuestSkill(quest, false) && SatisfyQuestExclusiveGroup(quest, false) && SatisfyQuestReputation(quest, false) &&
        SatisfyQuestPreviousQuest(quest, false) && SatisfyQuestNextChain(quest, false) &&
        SatisfyQuestPrevChain(quest, false) && SatisfyQuestDay(quest, false) && SatisfyQuestWeek(quest, false) &&
        SatisfyQuestMonth(quest, false) && SatisfyQuestSeasonal(quest, false))
    {
        return getLevel() + sWorld->getIntConfig(WorldIntConfigs::CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF) >= quest->GetMinLevel();
    }

    return false;
}

bool Player::CanTakeQuest(Quest const* quest, bool msg)
{
    return !DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, quest->GetQuestId(), this)
        && SatisfyQuestStatus(quest, msg) && SatisfyQuestExclusiveGroup(quest, msg)
        && SatisfyQuestClass(quest, msg) && SatisfyQuestRace(quest, msg) && SatisfyQuestLevel(quest, msg)
        && SatisfyQuestSkill(quest, msg) && SatisfyQuestReputation(quest, msg)
        && SatisfyQuestPreviousQuest(quest, msg) && SatisfyQuestTimed(quest, msg)
        && SatisfyQuestNextChain(quest, msg) && SatisfyQuestPrevChain(quest, msg)
        && SatisfyQuestDay(quest, msg) && SatisfyQuestWeek(quest, msg)
        && SatisfyQuestMonth(quest, msg) && SatisfyQuestSeasonal(quest, msg)
        && SatisfyQuestConditions(quest, msg);
}

bool Player::CanAddQuest(Quest const* quest, bool msg)
{
    if (!SatisfyQuestLog(msg))
        return false;

    uint32 srcitem = quest->GetSrcItemId();
    if (srcitem > 0)
    {
        ItemPosCountVec dest;
        InventoryResult msg2 = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, srcitem, QUEST_SOURCE_ITEM_COUNT);

        // player already have max number (in most case 1) source item, no additional item needed and quest can be added.
        if (msg2 == EQUIP_ERR_ITEM_MAX_COUNT)
            return true;
        else if (msg2 != EQUIP_ERR_OK)
        {
            SendEquipError(msg2, NULL, NULL, srcitem);
            return false;
        }
    }
    return true;
}

bool Player::CanCompleteQuest(uint32 questId)
{
    if (questId)
    {
        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            return false;

        if (!qInfo->IsRepeatable() && m_RewardedQuests.find(questId) != m_RewardedQuests.end())
            return false;                                   // not allow re-complete quest

        // Method==0 quests complete on accept (no objectives). Do NOT treat
        // QUEST_FLAGS_AUTOCOMPLETE here — that flag only controls remote turn-in UI.
        if (qInfo->IsAutoComplete() && CanTakeQuest(qInfo, false))
            return true;

        QuestStatusMap::iterator itr = m_QuestStatus.find(questId);
        if (itr == m_QuestStatus.end())
            return false;

        QuestStatusData& questStatus = itr->second;
        if (questStatus.Status == QUEST_STATUS_INCOMPLETE)
        {
            if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT) && !questStatus.Explored)
                return false;

            if (qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED) && questStatus.Timer == 0)
                return false;

            for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            {
                QuestObjective const* questObjective = *citr;
                switch (questObjective->Type)
                {
                    case QUEST_OBJECTIVE_TYPE_DUMMY:
                    case QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER:
                    case QUEST_OBJECTIVE_TYPE_PET_BATTLE_ELITE:
                    case QUEST_OBJECTIVE_TYPE_PET_BATTLE_PVP:
                    {
                        // These are event/state objectives — never treat as done by default.
                        // DB Amount is often 0; retail still requires the event (areatrigger etc.).
                        uint32 required = questObjective->Amount > 0 ? uint32(questObjective->Amount) : 1;
                        if (GetQuestObjectiveCounter(questObjective->Id) >= required)
                            break;

                        // 18414 also tracks progress in quest-log state bits (0x100 << index).
                        uint16 slot = FindQuestSlot(questId);
                        if (slot < MAX_QUEST_LOG_SIZE && questObjective->Index < 24 &&
                            (GetQuestSlotState(slot) & (QUEST_STATE_OBJECTIVE_0 << questObjective->Index)))
                            break;

                        return false;
                    }
                    case QUEST_OBJECTIVE_TYPE_SPELL:
                    {
                        if (!HasSpell(questObjective->ObjectId))
                            return false;

                        break;
                    }
                    case QUEST_OBJECTIVE_TYPE_FACTION_REP:
                    case QUEST_OBJECTIVE_TYPE_FACTION_REP2:
                    {
                        if (GetReputationMgr().GetReputation(questObjective->ObjectId) < questObjective->Amount)
                            return false;

                        break;
                    }
                    case QUEST_OBJECTIVE_TYPE_MONEY:
                    {
                        if (!HasEnoughMoney(uint64(questObjective->Amount)))
                            return false;

                        break;
                    }
                    default:
                    {
                        if (GetQuestObjectiveCounter(questObjective->Id) < uint32(questObjective->Amount))
                            return false;
                    }
                }
            }

            return true;
        }
    }
    return false;
}

bool Player::CanCompleteRepeatableQuest(Quest const* quest)
{
    // Solve problem that player don't have the quest and try complete it.
    // if repeatable she must be able to complete event if player don't have it.
    // Seem that all repeatable quest are DELIVER Flag so, no need to add more.
    if (!CanTakeQuest(quest, false))
        return false;

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
        if ((*citr)->Type == QUEST_OBJECTIVE_TYPE_ITEM)
            if (!HasItemCount((*citr)->ObjectId, (*citr)->Amount))
                return false;

    if (!CanRewardQuest(quest, false))
        return false;

    return true;
}

bool Player::CanRewardQuest(Quest const* quest, bool msg)
{
    // not auto complete quest and not completed quest (only cheating case, then ignore without message)
    if (!quest->IsDFQuest() && !quest->IsAutoComplete() && !(quest->GetFlags() & QUEST_FLAGS_AUTOCOMPLETE) && GetQuestStatus(quest->GetQuestId()) != QUEST_STATUS_COMPLETE)
        return false;

    // daily quest can't be rewarded (25 daily quest already completed)
    if (!SatisfyQuestDay(quest, true) || !SatisfyQuestWeek(quest, true) || !SatisfyQuestMonth(quest, true) || !SatisfyQuestSeasonal(quest, true))
        return false;

    // rewarded and not repeatable quest (only cheating case, then ignore without message)
    if (GetQuestRewardStatus(quest->GetQuestId()))
        return false;

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        switch (questObjective->Type)
        {
            case QUEST_OBJECTIVE_TYPE_ITEM:
            {
                if (GetItemCount(questObjective->ObjectId) < uint32(questObjective->Amount))
                {
                    if (msg)
                        SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL, questObjective->ObjectId);

                    return false;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_CURRENCY:
            {
                if (!HasCurrency(questObjective->ObjectId, questObjective->Amount))
                    return false;

                break;
            }
            case QUEST_OBJECTIVE_TYPE_MONEY:
            {
                if (!HasEnoughMoney(uint64(questObjective->Amount)))
                    return false;

                break;
            }
            default:
                break;
        }
    }

    return true;
}

bool Player::CanRewardQuest(Quest const* quest, uint32 reward, bool msg)
{
    // prevent receive reward with quest items in bank or for not completed quest
    if (!CanRewardQuest(quest, msg))
        return false;

    if (quest->GetRewardPackageItemId() > 0)
    {
        if (reward)
        {
            ItemPosCountVec dest;
            InventoryResult res = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, reward, 1);
            if (res != EQUIP_ERR_OK)
            {
                SendEquipError(res, NULL, NULL, reward);
                return false;
            }
        }
    }

    if (quest->GetRewChoiceItemsCount() > 0)
    {
        if (!quest->IsRewChoiceItemValid(reward))
            return false;

        ItemPosCountVec dest;
        InventoryResult res = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, reward, quest->GetRewChoiceItemCount(reward));
        if (res != EQUIP_ERR_OK)
        {
            SendEquipError(res, NULL, NULL, reward);
            return false;
        }
    }

    if (quest->GetRewItemsCount() > 0)
    {
        for (uint32 i = 0; i < quest->GetRewItemsCount(); ++i)
        {
            if (quest->RewardItemId[i])
            {
                ItemPosCountVec dest;
                InventoryResult res = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, quest->RewardItemId[i], quest->RewardItemIdCount[i]);
                if (res != EQUIP_ERR_OK)
                {
                    SendEquipError(res, NULL, NULL, quest->RewardItemId[i]);
                    return false;
                }
            }
        }
    }

    return true;
}

void Player::AddQuest(Quest const* quest, Object* questGiver)
{
    uint16 logSlot = FindQuestSlot(0);

    if (logSlot >= MAX_QUEST_LOG_SIZE) // Player does not have any free slot in the quest log
        return;

    uint32 questId = quest->GetQuestId();

    // if not exist then created with set uState == NEW and rewarded=false
    QuestStatusData& questStatusData = m_QuestStatus[questId];

    // check for repeatable quests status reset
    questStatusData.Status = QUEST_STATUS_INCOMPLETE;
    questStatusData.Explored = false;

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        if (questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP || questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP2)
            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(questObjective->ObjectId))
                GetReputationMgr().SetVisible(factionEntry);

        // not all Quest Objective types need to be tracked, some such as reputation are handled/checked externally
        if (questObjective->Type == QUEST_OBJECTIVE_TYPE_CURRENCY
            || questObjective->Type == QUEST_OBJECTIVE_TYPE_SPELL
            || questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP
            || questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP2
            || questObjective->Type == QUEST_OBJECTIVE_TYPE_MONEY
            || questObjective->Type == QUEST_OBJECTIVE_TYPE_DUMMY)
            continue;

        m_questObjectiveStatus.insert(std::make_pair(questObjective->Id, uint32(0)));
        m_questObjectiveStatusSave.insert(std::make_pair(questObjective->Id, true));
    }

    GiveQuestSourceItem(quest);
    AdjustQuestReqItemCount(quest, questStatusData);

    uint32 qtime = 0;
    if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
    {
        uint32 limittime = quest->GetLimitTime();

        // shared timed quest
        if (questGiver && questGiver->GetTypeId() == TypeID::TYPEID_PLAYER)
            limittime = questGiver->ToPlayer()->getQuestStatusMap()[questId].Timer / IN_MILLISECONDS;

        AddTimedQuest(questId);
        questStatusData.Timer = limittime * IN_MILLISECONDS;
        qtime = static_cast<uint32>(time(NULL)) + limittime;
    }
    else
        questStatusData.Timer = 0;

    if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
    {
        pvpInfo.IsHostile = true;
        UpdatePvPState();
    }

    // Quest log slot must exist before CompleteQuest touches QUEST_STATE bits.
    SetQuestSlot(logSlot, questId, qtime);

    if (CanCompleteQuest(questId))
        CompleteQuest(questId);

    m_QuestStatusSave[questId] = true;

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_QUEST, questId);

    UpdateForQuestWorldObjects();
    UpdatePhasing();

    UpdateObjectVisibility();

    if (questGiver) // script managment for every quest
    {
        switch (questGiver->GetTypeId())
        {
            case TypeID::TYPEID_UNIT:
                sScriptMgr->OnQuestAccept(this, (questGiver->ToCreature()), quest);
                questGiver->ToCreature()->AI()->sQuestAccept(this, quest);
                break;
            case TypeID::TYPEID_ITEM:
            case TypeID::TYPEID_CONTAINER:
            {
                Item* item = (Item*)questGiver;
                sScriptMgr->OnQuestAccept(this, item, quest);
                break;
            }
            case TypeID::TYPEID_GAMEOBJECT:
                sScriptMgr->OnQuestAccept(this, questGiver->ToGameObject(), quest);
                break;
            default:
                break;
        }
    }

    // Always notify PlayerScripts (questGiver may be NULL for share/confirm/sync).
    sScriptMgr->OnPlayerQuestAdd(this, quest);

    uint32 zone = 0, area = 0;

    GetZoneAndAreaId(zone, area);
    UpdateZoneDependentAuras(zone);
    UpdateAreaDependentAuras(area);
    UpdateForQuestWorldObjects();
}

void Player::CompleteQuest(uint32 quest_id)
{
    if (!quest_id)
        return;

    SetQuestStatus(quest_id, QUEST_STATUS_COMPLETE);

    uint16 log_slot = FindQuestSlot(quest_id);
    if (log_slot < MAX_QUEST_LOG_SIZE)
        SetQuestSlotState(log_slot, QUEST_STATE_COMPLETE);

    Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest_id);
    if (!qInfo)
        return;

    if (qInfo->HasFlag(QUEST_FLAGS_TRACKING_EVENT))
        RewardQuest(qInfo, 0, this, false);
    else if (IsInWorld() && GetSession())
    {
        // 18414 right-side COMPLETE toast is NOT driven by SMSG_QUESTUPDATE_COMPLETE
        // (that handler hardcodes AddAutoQuestPopUp(OFFER)). Stock client fires
        // QUEST_AUTOCOMPLETE (FireEvent 181) from quest-log UF / quest-query
        // callbacks when:
        //   - quest cache flags include QUEST_FLAGS_AUTOCOMPLETE (0x10000)
        //   - DUMMY objectives have state bit (0x100 << index) in the quest log UF
        //   - QUEST_STATE_COMPLETE is set and FAIL is not
        // Order: flush UF first (state bits), then refresh query cache (flags +
        // objectives) so the second callback can succeed even if the first raced
        // ahead of a stale cache, then completion NPCs for turn-in UI.
        UpdateData upd(GetMapId());
        BuildValuesUpdateBlockForPlayer(&upd, this);
        if (upd.HasData())
        {
            WorldPacket updatePacket;
            upd.BuildPacket(&updatePacket);
            GetSession()->SendPacket(&updatePacket);
        }

        PlayerTalkClass->SendQuestQueryResponse(qInfo);
        SendQuestCompletionNPCs(quest_id);

        // SMSG_QUESTUPDATE_COMPLETE hardcodes AddAutoQuestPopUp(OFFER) on 18414.
        // AUTOCOMPLETE toasts come from QUEST_AUTOCOMPLETE (UF/query above); sending
        // this packet would inject a wrong OFFER popup. Keep it for normal quests
        // (yellow text / legacy cue only).
        if (!qInfo->HasFlag(QUEST_FLAGS_AUTOCOMPLETE))
            SendQuestComplete(qInfo);
    }

    uint32 zone = 0, area = 0;
    GetZoneAndAreaId(zone, area);
    UpdateZoneDependentAuras(zone);
    UpdateAreaDependentAuras(area);
    UpdateForQuestWorldObjects();
}

void Player::IncompleteQuest(uint32 quest_id)
{
    if (quest_id)
    {
        SetQuestStatus(quest_id, QUEST_STATUS_INCOMPLETE);

        uint16 log_slot = FindQuestSlot(quest_id);
        if (log_slot < MAX_QUEST_LOG_SIZE)
            RemoveQuestSlotState(log_slot, QUEST_STATE_COMPLETE);
    }
}

void Player::RewardQuest(Quest const* quest, uint32 reward, Object* questGiver, bool announce)
{
    //this THING should be here to protect code from quest, which cast on player far teleport as a reward
    //should work fine, cause far teleport will be executed in Player::Update()
    SetCanDelayTeleport(true);

    uint32 quest_id = quest->GetQuestId();

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        switch (questObjective->Type)
        {
            case QUEST_OBJECTIVE_TYPE_ITEM:
            {
                DestroyItemCount(questObjective->ObjectId, questObjective->Amount, true);
                break;
            }
            case QUEST_OBJECTIVE_TYPE_CURRENCY:
            {
                ModifyCurrency(questObjective->ObjectId, -int32(questObjective->Amount));
                break;
            }
            case QUEST_OBJECTIVE_TYPE_MONEY:
            {
                ModifyMoney(-int64(questObjective->Amount));
                break;
            }
            default:
                break;
        }
    }

    for (uint8 i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
    {
        if (quest->RequiredSourceItemId[i])
        {
            uint32 count = quest->RequiredSourceItemCount[i];
            DestroyItemCount(quest->RequiredSourceItemId[i], count ? count : 9999, true);
        }
    }

    RemoveTimedQuest(quest_id);

    if (quest->GetRewardPackageItemId() > 0)
    {
        ItemPosCountVec dest;
        if (CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, reward, 1) == EQUIP_ERR_OK)
        {
            Item* item = StoreNewItem(dest, reward, true, Item::GenerateItemRandomPropertyId(reward));
            SendNewItem(item, 1, true, false);
        }
    }

    if (quest->GetRewChoiceItemsCount() > 0)
    {
        ItemPosCountVec dest;
        if (CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, reward, quest->GetRewChoiceItemCount(reward)) == EQUIP_ERR_OK)
        {
            Item* item = StoreNewItem(dest, reward, true, Item::GenerateItemRandomPropertyId(reward));
            SendNewItem(item, quest->GetRewChoiceItemCount(reward), true, false);
        }
    }

    if (quest->GetRewItemsCount() > 0)
    {
        for (uint32 i = 0; i < quest->GetRewItemsCount(); ++i)
        {
            if (uint32 itemId = quest->RewardItemId[i])
            {
                ItemPosCountVec dest;
                if (CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, quest->RewardItemIdCount[i]) == EQUIP_ERR_OK)
                {
                    Item* item = StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));
                    SendNewItem(item, quest->RewardItemIdCount[i], true, false);
                }
            }
        }
    }

    for (uint8 i = 0; i < QUEST_REWARD_CURRENCY_COUNT; ++i)
        if (quest->RewardCurrencyId[i])
            ModifyCurrency(quest->RewardCurrencyId[i], quest->RewardCurrencyCount[i]);

    if (uint32 skill = quest->GetRewardSkillId())
        UpdateSkillPro(skill, 1000, quest->GetRewardSkillPoints());

    RewardReputation(quest);

    uint16 log_slot = FindQuestSlot(quest_id);
    if (log_slot < MAX_QUEST_LOG_SIZE)
        SetQuestSlot(log_slot, 0);

    bool rewarded = (m_RewardedQuests.find(quest_id) != m_RewardedQuests.end());

    // Not give XP in case already completed once repeatable quest
    uint32 XP = rewarded ? 0 : uint32(quest->XPValue(this) * sWorld->getRate(Rates::RATE_XP_QUEST));

    // handle SPELL_AURA_MOD_XP_QUEST_PCT auras
    Unit::AuraEffectList const& ModXPPctAuras = GetAuraEffectsByType(SPELL_AURA_MOD_XP_QUEST_PCT);
    for (Unit::AuraEffectList::const_iterator i = ModXPPctAuras.begin(); i != ModXPPctAuras.end(); ++i)
        AddPct(XP, (*i)->GetAmount());

    uint32 moneyRew = 0;
    if (getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
        GiveXP(XP, NULL);
    else
        moneyRew = uint32(quest->GetRewMoneyMaxLevel() * sWorld->getRate(Rates::RATE_DROP_MONEY));

    if (Guild* guild = sGuildMgr->GetGuildById(GetGuildId()))
        guild->GiveXP(uint32(quest->XPValue(this) * sWorld->getRate(Rates::RATE_XP_QUEST) * sWorld->getRate(Rates::RATE_XP_GUILD_MODIFIER)), this);

    moneyRew += quest->GetRewMoney();
    ModifyMoney(moneyRew);

    if (moneyRew > 0)
        UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD, uint32(moneyRew));

    // honor reward
    if (uint32 honor = quest->CalculateHonorGain(getLevel()))
        RewardHonor(NULL, 0, honor);

    // title reward
    if (quest->GetCharTitleId())
    {
        if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(quest->GetCharTitleId()))
            SetTitle(titleEntry);
    }

    // Send reward mail
    if (uint32 mail_template_id = quest->GetRewMailTemplateId())
    {
        /// @todo Poor design of mail system
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        MailDraft(mail_template_id).SendMailTo(trans, this, questGiver, MAIL_CHECK_MASK_HAS_BODY, quest->GetRewMailDelaySecs());
        CharacterDatabase.CommitTransaction(trans);
    }

    if (quest->IsDaily() || quest->IsDFQuest())
    {
        SetDailyQuestStatus(quest_id);
        if (quest->IsDaily())
        {
            UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST, quest_id);
            UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY, quest_id);
        }
    }
    else if (quest->IsWeekly())
        SetWeeklyQuestStatus(quest_id);
    else if (quest->IsMonthly())
        SetMonthlyQuestStatus(quest_id);
    else if (quest->IsSeasonal())
        SetSeasonalQuestStatus(quest_id);

    RemoveActiveQuest(quest_id);
    m_RewardedQuests.insert(quest_id);
    m_RewardedQuestsSave[quest_id] = true;
    // StoreNewItem, mail reward, etc. save data directly to the database
    // to prevent exploitable data desynchronisation we save the quest status to the database too
    // (to prevent rewarding this quest another time while rewards were already given out)
    SQLTransaction trans = SQLTransaction(NULL);
    _SaveQuestStatus(trans);

    if (announce)
        SendQuestReward(quest, XP);

    // cast spells after mark quest complete (some spells have quest completed state requirements in spell_area data)
    if (quest->GetRewSpellCast() > 0)
        CastSpell(this, quest->GetRewSpellCast(), true);
    else if (quest->GetRewSpell() > 0)
        CastSpell(this, quest->GetRewSpell(), true);

    if (quest->GetZoneOrSort() > 0)
        UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE, quest->GetZoneOrSort());
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST, quest->GetQuestId());

    if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
    {
        pvpInfo.IsHostile = pvpInfo.IsInHostileArea || HasPvPForcingQuest();
        UpdatePvPState();
    }

    //lets remove flag for delayed teleports
    SetCanDelayTeleport(false);
}

void Player::FailQuest(uint32 questId)
{
    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        SetQuestStatus(questId, QUEST_STATUS_FAILED);

        uint16 log_slot = FindQuestSlot(questId);

        if (log_slot < MAX_QUEST_LOG_SIZE)
        {
            SetQuestSlotTimer(log_slot, 1);
            SetQuestSlotState(log_slot, QUEST_STATE_FAIL);
        }

        if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
        {
            QuestStatusData& q_status = m_QuestStatus[questId];

            RemoveTimedQuest(questId);
            q_status.Timer = 0;

            SendQuestTimerFailed(questId);
        }
        else
            SendQuestFailed(questId);

        // Destroy quest items on quest failure.
        if (quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
            for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
                if ((*citr)->Type == QUEST_OBJECTIVE_TYPE_ITEM)
                    DestroyItemCount((*citr)->ObjectId, (*citr)->Amount, true, true);

        for (uint8 i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
            if (quest->RequiredSourceItemId[i] > 0 && quest->RequiredSourceItemCount[i] > 0)
                // Destroy items received during the quest.
                DestroyItemCount(quest->RequiredSourceItemId[i], quest->RequiredSourceItemCount[i], true, true);
    }
}

bool Player::SatisfyQuestSkill(Quest const* qInfo, bool msg) const
{
    uint32 skill = qInfo->GetRequiredSkill();

    // skip 0 case RequiredSkill
    if (skill == 0)
        return true;

    // check skill value
    if (GetSkillValue(skill) < qInfo->GetRequiredSkillValue())
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestSkill: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required skill value.", qInfo->GetQuestId());
        }

        return false;
    }

    return true;
}

bool Player::SatisfyQuestLevel(Quest const* qInfo, bool msg)
{
    if (getLevel() < qInfo->GetMinLevel())
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_QUEST_FAILED_LOW_LEVEL);
            SF_LOG_DEBUG("misc", "SatisfyQuestLevel: Sent INVALIDREASON_QUEST_FAILED_LOW_LEVEL (questId: %u) because player does not have required (min) level.", qInfo->GetQuestId());
        }
        return false;
    }
    else if (qInfo->GetMaxLevel() > 0 && getLevel() > qInfo->GetMaxLevel())
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ); // There doesn't seem to be a specific response for too high player level
            SF_LOG_DEBUG("misc", "SatisfyQuestLevel: Sent INVALIDREASON_QUEST_FAILED_LOW_LEVEL (questId: %u) because player does not have required (max) level.", qInfo->GetQuestId());
        }
        return false;
    }
    return true;
}

bool Player::SatisfyQuestLog(bool msg)
{
    // exist free slot
    if (FindQuestSlot(0) < MAX_QUEST_LOG_SIZE)
        return true;

    if (msg)
    {
        WorldPacket data(SMSG_QUESTLOG_FULL, 0);
        GetSession()->SendPacket(&data);
        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTLOG_FULL");
    }
    return false;
}

bool Player::SatisfyQuestPreviousQuest(Quest const* qInfo, bool msg)
{
    // No previous quest (might be first quest in a series)
    if (qInfo->prevQuests.empty())
        return true;

    for (Quest::PrevQuests::const_iterator iter = qInfo->prevQuests.begin(); iter != qInfo->prevQuests.end(); ++iter)
    {
        uint32 prevId = abs(*iter);

        Quest const* qPrevInfo = sObjectMgr->GetQuestTemplate(prevId);

        if (qPrevInfo)
        {
            // If any of the positive previous quests completed, return true
            if (*iter > 0 && m_RewardedQuests.find(prevId) != m_RewardedQuests.end())
            {
                // skip one-from-all exclusive group
                if (qPrevInfo->GetExclusiveGroup() >= 0)
                    return true;

                // each-from-all exclusive group (< 0)
                // can be start if only all quests in prev quest exclusive group completed and rewarded
                ObjectMgr::ExclusiveQuestGroupsBounds range(sObjectMgr->mExclusiveQuestGroups.equal_range(qPrevInfo->GetExclusiveGroup()));

                for (; range.first != range.second; ++range.first)
                {
                    uint32 exclude_Id = range.first->second;

                    // skip checked quest id, only state of other quests in group is interesting
                    if (exclude_Id == prevId)
                        continue;

                    // alternative quest from group also must be completed and rewarded (reported)
                    if (m_RewardedQuests.find(exclude_Id) == m_RewardedQuests.end())
                    {
                        if (msg)
                        {
                            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                            SF_LOG_DEBUG("misc", "SatisfyQuestPreviousQuest: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required quest (1).", qInfo->GetQuestId());
                        }
                        return false;
                    }
                }
                return true;
            }

            // If any of the negative previous quests active, return true
            if (*iter < 0 && GetQuestStatus(prevId) != QUEST_STATUS_NONE)
            {
                // skip one-from-all exclusive group
                if (qPrevInfo->GetExclusiveGroup() >= 0)
                    return true;

                // each-from-all exclusive group (< 0)
                // can be start if only all quests in prev quest exclusive group active
                ObjectMgr::ExclusiveQuestGroupsBounds range(sObjectMgr->mExclusiveQuestGroups.equal_range(qPrevInfo->GetExclusiveGroup()));

                for (; range.first != range.second; ++range.first)
                {
                    uint32 exclude_Id = range.first->second;

                    // skip checked quest id, only state of other quests in group is interesting
                    if (exclude_Id == prevId)
                        continue;

                    // alternative quest from group also must be active
                    if (GetQuestStatus(exclude_Id) != QUEST_STATUS_NONE)
                    {
                        if (msg)
                        {
                            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                            SF_LOG_DEBUG("misc", "SatisfyQuestPreviousQuest: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required quest (2).", qInfo->GetQuestId());
                        }
                        return false;
                    }
                }
                return true;
            }
        }
    }

    // Has only positive prev. quests in non-rewarded state
    // and negative prev. quests in non-active state
    if (msg)
    {
        SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
        SF_LOG_DEBUG("misc", "SatisfyQuestPreviousQuest: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required quest (3).", qInfo->GetQuestId());
    }

    return false;
}

bool Player::SatisfyQuestClass(Quest const* qInfo, bool msg) const
{
    uint32 reqClass = qInfo->GetRequiredClasses();

    if (reqClass == 0)
        return true;

    if ((reqClass & getClassMask()) == 0)
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestClass: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required class.", qInfo->GetQuestId());
        }

        return false;
    }

    return true;
}

bool Player::SatisfyQuestRace(Quest const* qInfo, bool msg)
{
    uint32 reqraces = qInfo->GetRequiredRaces();
    if (reqraces == 0)
        return true;
    if ((reqraces & getRaceMask()) == 0)
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_QUEST_FAILED_WRONG_RACE);
            SF_LOG_DEBUG("misc", "SatisfyQuestRace: Sent INVALIDREASON_QUEST_FAILED_WRONG_RACE (questId: %u) because player does not have required race.", qInfo->GetQuestId());
        }
        return false;
    }
    return true;
}

bool Player::SatisfyQuestReputation(Quest const* qInfo, bool msg)
{
    uint32 fIdMin = qInfo->GetRequiredMinRepFaction();      //Min required rep
    if (fIdMin && GetReputationMgr().GetReputation(fIdMin) < qInfo->GetRequiredMinRepValue())
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestReputation: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required reputation (min).", qInfo->GetQuestId());
        }
        return false;
    }

    uint32 fIdMax = qInfo->GetRequiredMaxRepFaction();      //Max required rep
    if (fIdMax && GetReputationMgr().GetReputation(fIdMax) >= qInfo->GetRequiredMaxRepValue())
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestReputation: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required reputation (max).", qInfo->GetQuestId());
        }
        return false;
    }

    // ReputationObjective2 does not seem to be an objective requirement but a requirement
    // to be able to accept the quest

    if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_FACTION_REP2))
        return true;

    for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        if (questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP2 && GetReputationMgr().GetReputation(questObjective->ObjectId) >= questObjective->Amount)
        {
            if (msg)
            {
                SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                SF_LOG_DEBUG("misc", "SatisfyQuestReputation: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not have required reputation (ReputationObjective2).", qInfo->GetQuestId());
            }

            return false;
        }
    }

    return true;
}

bool Player::SatisfyQuestStatus(Quest const* qInfo, bool msg)
{
    if (GetQuestStatus(qInfo->GetQuestId()) != QUEST_STATUS_NONE)
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_QUEST_ALREADY_ON);
            SF_LOG_DEBUG("misc", "SatisfyQuestStatus: Sent INVALIDREASON_QUEST_ALREADY_ON (questId: %u) because player quest status is not NONE.", qInfo->GetQuestId());
        }
        return false;
    }
    return true;
}

bool Player::SatisfyQuestConditions(Quest const* qInfo, bool msg)
{
    ConditionList conditions = sConditionMgr->GetConditionsForNotGroupedEntry(CONDITION_SOURCE_TYPE_QUEST_ACCEPT, qInfo->GetQuestId());
    if (!sConditionMgr->IsObjectMeetToConditions(this, conditions))
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestConditions: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player does not meet conditions.", qInfo->GetQuestId());
        }
        SF_LOG_DEBUG("condition", "Player::SatisfyQuestConditions: conditions not met for quest %u", qInfo->GetQuestId());
        return false;
    }
    return true;
}

bool Player::SatisfyQuestTimed(Quest const* qInfo, bool msg)
{
    if (!m_timedquests.empty() && qInfo->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_QUEST_ONLY_ONE_TIMED);
            SF_LOG_DEBUG("misc", "SatisfyQuestTimed: Sent INVALIDREASON_QUEST_ONLY_ONE_TIMED (questId: %u) because player is already on a timed quest.", qInfo->GetQuestId());
        }
        return false;
    }
    return true;
}

bool Player::SatisfyQuestExclusiveGroup(Quest const* qInfo, bool msg)
{
    // non positive exclusive group, if > 0 then can be start if any other quest in exclusive group already started/completed
    if (qInfo->GetExclusiveGroup() <= 0)
        return true;

    ObjectMgr::ExclusiveQuestGroupsBounds range(sObjectMgr->mExclusiveQuestGroups.equal_range(qInfo->GetExclusiveGroup()));

    for (; range.first != range.second; ++range.first)
    {
        uint32 exclude_Id = range.first->second;

        // skip checked quest id, only state of other quests in group is interesting
        if (exclude_Id == qInfo->GetQuestId())
            continue;

        // not allow have daily quest if daily quest from exclusive group already recently completed
        Quest const* Nquest = sObjectMgr->GetQuestTemplate(exclude_Id);
        if (!SatisfyQuestDay(Nquest, false) || !SatisfyQuestWeek(Nquest, false) || !SatisfyQuestSeasonal(Nquest, false))
        {
            if (msg)
            {
                SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                SF_LOG_DEBUG("misc", "SatisfyQuestExclusiveGroup: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player already did daily quests in exclusive group.", qInfo->GetQuestId());
            }

            return false;
        }

        // alternative quest already started or completed - but don't check rewarded states if both are repeatable
        if (GetQuestStatus(exclude_Id) != QUEST_STATUS_NONE || (!(qInfo->IsRepeatable() && Nquest->IsRepeatable()) && (m_RewardedQuests.find(exclude_Id) != m_RewardedQuests.end())))
        {
            if (msg)
            {
                SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                SF_LOG_DEBUG("misc", "SatisfyQuestExclusiveGroup: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player already did quest in exclusive group.", qInfo->GetQuestId());
            }
            return false;
        }
    }
    return true;
}

bool Player::SatisfyQuestNextChain(Quest const* qInfo, bool msg)
{
    uint32 nextQuest = qInfo->GetNextQuestInChain();
    if (!nextQuest)
        return true;

    // next quest in chain already started or completed
    if (GetQuestStatus(nextQuest) != QUEST_STATUS_NONE) // GetQuestStatus returns QUEST_STATUS_COMPLETED for rewarded quests
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
            SF_LOG_DEBUG("misc", "SatisfyQuestNextChain: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player already did or started next quest in chain.", qInfo->GetQuestId());
        }
        return false;
    }

    // check for all quests further up the chain
    // only necessary if there are quest chains with more than one quest that can be skipped
    //return SatisfyQuestNextChain(qInfo->GetNextQuestInChain(), msg);
    return true;
}

bool Player::SatisfyQuestPrevChain(Quest const* qInfo, bool msg)
{
    // No previous quest in chain
    if (qInfo->prevChainQuests.empty())
        return true;

    for (Quest::PrevChainQuests::const_iterator iter = qInfo->prevChainQuests.begin(); iter != qInfo->prevChainQuests.end(); ++iter)
    {
        QuestStatusMap::const_iterator itr = m_QuestStatus.find(*iter);

        // If any of the previous quests in chain active, return false
        if (itr != m_QuestStatus.end() && itr->second.Status != QUEST_STATUS_NONE)
        {
            if (msg)
            {
                SendCanTakeQuestResponse(INVALIDREASON_DONT_HAVE_REQ);
                SF_LOG_DEBUG("misc", "SatisfyQuestNextChain: Sent INVALIDREASON_DONT_HAVE_REQ (questId: %u) because player already did or started next quest in chain.", qInfo->GetQuestId());
            }
            return false;
        }

        // check for all quests further down the chain
        // only necessary if there are quest chains with more than one quest that can be skipped
        //if (!SatisfyQuestPrevChain(prevId, msg))
        //    return false;
    }

    // No previous quest in chain active
    return true;
}

bool Player::SatisfyQuestDay(Quest const* qInfo, bool msg)
{
    if (!qInfo->IsDaily() && !qInfo->IsDFQuest())
        return true;

    if (qInfo->IsDFQuest())
    {
        if (!m_DFQuests.empty())
            return false;

        return true;
    }

    bool have_slot = false;
    for (uint32 quest_daily_idx = 0; quest_daily_idx < PLAYER_MAX_DAILY_QUESTS; ++quest_daily_idx)
    {
        uint32 id = 0;//GetUInt32Value(PLAYER_FIELD_DAILY_QUESTS_1+quest_daily_idx);
        if (qInfo->GetQuestId() == id)
            return false;

        if (!id)
            have_slot = true;
    }

    if (!have_slot)
    {
        if (msg)
        {
            SendCanTakeQuestResponse(INVALIDREASON_DAILY_QUESTS_REMAINING);
            SF_LOG_DEBUG("misc", "SatisfyQuestDay: Sent INVALIDREASON_DAILY_QUESTS_REMAINING (questId: %u) because player already did all possible quests today.", qInfo->GetQuestId());
        }
        return false;
    }

    return true;
}

bool Player::SatisfyQuestWeek(Quest const* qInfo, bool /*msg*/)
{
    if (!qInfo->IsWeekly() || m_weeklyquests.empty())
        return true;

    // if not found in cooldown list
    return m_weeklyquests.find(qInfo->GetQuestId()) == m_weeklyquests.end();
}

bool Player::SatisfyQuestSeasonal(Quest const* qInfo, bool /*msg*/)
{
    if (!qInfo->IsSeasonal() || m_seasonalquests.empty())
        return true;

    uint16 eventId = sGameEventMgr->GetEventIdForQuest(qInfo);
    if (m_seasonalquests.find(eventId) == m_seasonalquests.end() || m_seasonalquests[eventId].empty())
        return true;

    // if not found in cooldown list
    return m_seasonalquests[eventId].find(qInfo->GetQuestId()) == m_seasonalquests[eventId].end();
}

bool Player::SatisfyQuestMonth(Quest const* qInfo, bool /*msg*/)
{
    if (!qInfo->IsMonthly() || m_monthlyquests.empty())
        return true;

    // if not found in cooldown list
    return m_monthlyquests.find(qInfo->GetQuestId()) == m_monthlyquests.end();
}

bool Player::GiveQuestSourceItem(Quest const* quest)
{
    uint32 srcitem = quest->GetSrcItemId();
    if (srcitem > 0)
    {
        ItemPosCountVec dest;
        InventoryResult msg = CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, srcitem, QUEST_SOURCE_ITEM_COUNT);
        if (msg == EQUIP_ERR_OK)
        {
            Item* item = StoreNewItem(dest, srcitem, true);
            SendNewItem(item, QUEST_SOURCE_ITEM_COUNT, true, false);
            return true;
        }
        // player already have max amount required item, just report success
        else if (msg == EQUIP_ERR_ITEM_MAX_COUNT)
            return true;
        else
            SendEquipError(msg, NULL, NULL, srcitem);
        return false;
    }

    return true;
}

bool Player::TakeQuestSourceItem(uint32 questId, bool msg)
{
    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (quest)
    {
        uint32 srcItemId = quest->GetSrcItemId();
        ItemTemplate const* item = sObjectMgr->GetItemTemplate(srcItemId);

        if (srcItemId > 0)
        {
            // exist two cases when destroy source quest item not possible:
            // a) non un-equippable item (equipped non-empty bag, for example)
            // b) when quest is started from an item and item also is needed in
            // the end as RequiredItemId
            InventoryResult res = CanUnequipItems(srcItemId, QUEST_SOURCE_ITEM_COUNT);
            if (res != EQUIP_ERR_OK)
            {
                if (msg)
                    SendEquipError(res, NULL, NULL, srcItemId);
                return false;
            }

            if (!quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
                return true;

            bool destroyItem = true;
            for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
                if (item->StartQuest == questId && srcItemId == (*citr)->ObjectId)
                    destroyItem = false;

            if (destroyItem)
                DestroyItemCount(srcItemId, QUEST_SOURCE_ITEM_COUNT, true, true);
        }
    }

    return true;
}

bool Player::GetQuestRewardStatus(uint32 quest_id) const
{
    Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest_id);
    if (qInfo)
    {
        // for repeatable quests: rewarded field is set after first reward only to prevent getting XP more than once
        if (!qInfo->IsRepeatable())
            return m_RewardedQuests.find(quest_id) != m_RewardedQuests.end();

        return false;
    }
    return false;
}

QuestStatus Player::GetQuestStatus(uint32 quest_id) const
{
    if (quest_id)
    {
        QuestStatusMap::const_iterator itr = m_QuestStatus.find(quest_id);
        if (itr != m_QuestStatus.end())
            return itr->second.Status;

        if (Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest_id))
            if (!qInfo->IsRepeatable() && m_RewardedQuests.find(quest_id) != m_RewardedQuests.end())
                return QUEST_STATUS_REWARDED;
    }
    return QUEST_STATUS_NONE;
}

bool Player::CanShareQuest(uint32 quest_id) const
{
    Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest_id);
    if (qInfo && qInfo->HasFlag(QUEST_FLAGS_PUSHABLE))
    {
        QuestStatusMap::const_iterator itr = m_QuestStatus.find(quest_id);
        if (itr != m_QuestStatus.end())
            return itr->second.Status == QUEST_STATUS_INCOMPLETE;
    }
    return false;
}

void Player::SetQuestStatus(uint32 quest_id, QuestStatus status)
{
    if (sObjectMgr->GetQuestTemplate(quest_id))
    {
        m_QuestStatus[quest_id].Status = status;
        m_QuestStatusSave[quest_id] = true;
    }

    uint32 zone = 0, area = 0;

    SpellAreaForQuestMapBounds saBounds = sSpellMgr->GetSpellAreaForQuestMapBounds(quest_id);
    if (saBounds.first != saBounds.second)
    {
        GetZoneAndAreaId(zone, area);

        for (SpellAreaForAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
            if (itr->second->autocast && itr->second->IsFitToRequirements(this, zone, area))
                if (!HasAura(itr->second->spellId))
                    CastSpell(this, itr->second->spellId, true);
    }

    saBounds = sSpellMgr->GetSpellAreaForQuestEndMapBounds(quest_id);
    if (saBounds.first != saBounds.second)
    {
        if (!zone || !area)
            GetZoneAndAreaId(zone, area);

        for (SpellAreaForAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
            if (!itr->second->IsFitToRequirements(this, zone, area))
                RemoveAurasDueToSpell(itr->second->spellId);
    }

    UpdateForQuestWorldObjects();
    UpdatePhasing();
}

void Player::RemoveActiveQuest(uint32 quest_id)
{
    QuestStatusMap::iterator itr = m_QuestStatus.find(quest_id);
    if (itr != m_QuestStatus.end())
    {
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(quest_id))
        {
            for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
            {
                uint32 objectiveId = (*citr)->Id;
                m_questObjectiveStatus.erase(objectiveId);
                m_questObjectiveStatusSave[objectiveId] = false;
            }
        }

        m_QuestStatus.erase(itr);
        m_QuestStatusSave[quest_id] = false;
        return;
    }
}

void Player::RemoveRewardedQuest(uint32 quest_id)
{
    RewardedQuestSet::iterator rewItr = m_RewardedQuests.find(quest_id);
    if (rewItr != m_RewardedQuests.end())
    {
        m_RewardedQuests.erase(rewItr);
        m_RewardedQuestsSave[quest_id] = false;
    }
}

// not used in Skyfire, but used in scripting code
uint16 Player::GetReqKillOrCastCurrentCount(uint32 quest_id, int32 entry)
{
    Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest_id);
    if (!qInfo)
        return 0;

    if (qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_NPC))
        for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            if ((*citr)->Type == QUEST_OBJECTIVE_TYPE_NPC && (*citr)->ObjectId == entry)
                return GetQuestObjectiveCounter((*citr)->Id);

    return 0;
}

void Player::ResetQuestObjectiveCounter(uint32 questId, uint8 type, uint32 objectId)
{
    if (GetQuestStatus(questId) != QUEST_STATUS_INCOMPLETE)
        return;

    Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
    if (!quest || !quest->GetQuestObjectiveCountType(type))
        return;

    uint16 logSlot = FindQuestSlot(questId);

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        if (questObjective->Type != type || questObjective->ObjectId != objectId)
            continue;

        m_questObjectiveStatus[questObjective->Id] = 0;
        m_questObjectiveStatusSave[questObjective->Id] = true;
        m_QuestStatusSave[questId] = true;

        if (logSlot < MAX_QUEST_LOG_SIZE)
            SetQuestSlotCounter(logSlot, questObjective->Index, 0);

        break;
    }
}

void Player::AdjustQuestReqItemCount(Quest const* quest, QuestStatusData& questStatusData)
{
    if (!quest->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
        return;

    for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
    {
        QuestObjective const* questObjective = *citr;
        if (questObjective->Type == QUEST_OBJECTIVE_TYPE_ITEM)
        {
            m_questObjectiveStatus[questObjective->Id] = std::min(GetItemCount(questObjective->ObjectId, true), uint32(questObjective->Amount));
            m_QuestStatusSave[quest->GetQuestId()] = true;
        }
    }
}

uint16 Player::FindQuestSlot(uint32 quest_id) const
{
    for (uint16 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
        if (GetQuestSlotQuestId(i) == quest_id)
            return i;

    return MAX_QUEST_LOG_SIZE;
}

uint32 Player::GetQuestSlotQuestId(uint16 slot) const
{
    return GetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_ID_OFFSET);
}

uint32 Player::GetQuestSlotState(uint16 slot)   const
{
    return GetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET);
}

uint16 Player::GetQuestSlotCounter(uint16 slot, uint8 counter) const
{
    return (uint16)(GetUInt64Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET) >> (counter * 16));
}

uint32 Player::GetQuestSlotTime(uint16 slot) const
{
    return GetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_TIME_OFFSET);
}

void Player::SetQuestSlot(uint16 slot, uint32 quest_id, uint32 timer /*= 0*/)
{
    SetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_ID_OFFSET, quest_id);
    SetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, 0);
    SetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, 0);
    SetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_TIME_OFFSET, timer);
}

void Player::SetQuestSlotCounter(uint16 slot, uint8 counter, uint16 count)
{
    uint64 val = GetUInt64Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET);
    val &= ~((uint64)0xFFFF << (counter * 16));
    val |= ((uint64)count << (counter * 16));
    SetUInt64Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_COUNTS_OFFSET, val);
}

void Player::SetQuestSlotState(uint16 slot, uint32 state)
{
    SetFlag(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, state);
}

void Player::RemoveQuestSlotState(uint16 slot, uint32 state)
{
    RemoveFlag(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_STATE_OFFSET, state);
}

void Player::SetQuestSlotTimer(uint16 slot, uint32 timer)
{
    SetUInt32Value(PLAYER_FIELD_QUEST_LOG + slot * MAX_QUEST_OFFSET + QUEST_TIME_OFFSET, timer);
}

void Player::SwapQuestSlot(uint16 slot1, uint16 slot2)
{
    for (int i = 0; i < MAX_QUEST_OFFSET; ++i)
    {
        uint32 temp1 = GetUInt32Value(PLAYER_FIELD_QUEST_LOG + MAX_QUEST_OFFSET * slot1 + i);
        uint32 temp2 = GetUInt32Value(PLAYER_FIELD_QUEST_LOG + MAX_QUEST_OFFSET * slot2 + i);

        SetUInt32Value(PLAYER_FIELD_QUEST_LOG + MAX_QUEST_OFFSET * slot1 + i, temp2);
        SetUInt32Value(PLAYER_FIELD_QUEST_LOG + MAX_QUEST_OFFSET * slot2 + i, temp1);
    }
}

void Player::AreaExploredOrEventHappens(uint32 questId)
{
    if (!questId)
        return;

    QuestStatusMap::iterator itr = m_QuestStatus.find(questId);
    if (itr == m_QuestStatus.end())
        return;

    // Always mark explored on the status entry (not only when a log slot exists).
    if (!itr->second.Explored)
    {
        itr->second.Explored = true;
        m_QuestStatusSave[questId] = true;
    }

    // DUMMY (explore) objectives are tracked on 18414 via quest-log STATE bits
    // (0x100 << index). Client FFD60/FD4A0 gates QUEST_AUTOCOMPLETE on that bit.
    // Send SMSG_QUESTUPDATE_ADD_CREDIT for the yellow
    // "objective text (Complete)" line. CompleteQuest skips SMSG_QUESTUPDATE_COMPLETE
    // for AUTOCOMPLETE quests (that packet hardcodes an OFFER popup), so this is
    // the single yellow notification for those quests.
    if (Quest const* quest = sObjectMgr->GetQuestTemplate(questId))
    {
        uint16 logSlot = FindQuestSlot(questId);
        for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin();
            citr != quest->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* objective = *citr;
            if (!objective || objective->Type != QUEST_OBJECTIVE_TYPE_DUMMY)
                continue;

            uint16 required = objective->Amount > 0 ? uint16(objective->Amount) : 1;
            uint16 oldCount = GetQuestObjectiveCounter(objective->Id);
            bool hasStateBit = logSlot < MAX_QUEST_LOG_SIZE && objective->Index < 24 &&
                (GetQuestSlotState(logSlot) & (QUEST_STATE_OBJECTIVE_0 << objective->Index));

            if (oldCount >= required && hasStateBit)
                continue;

            if (oldCount < required)
            {
                uint16 addCount = required - oldCount;
                m_questObjectiveStatus[objective->Id] = required;
                m_questObjectiveStatusSave[objective->Id] = true;
                m_QuestStatusSave[questId] = true;

                // Client prints ERR_QUEST_OBJECTIVE_COMPLETE_S from the cached
                // objective Description. Refresh query first so an empty/stale
                // cache cannot swallow the yellow "(Complete)" line.
                if (PlayerTalkClass)
                    PlayerTalkClass->SendQuestQueryResponse(quest);

                // Updates counters + DUMMY state bit and shows yellow complete text.
                SendQuestUpdateAddCredit(quest, objective, ObjectGuid(0), oldCount, addCount);
            }
            else if (!hasStateBit && logSlot < MAX_QUEST_LOG_SIZE && objective->Index < 24)
                SetQuestSlotState(logSlot, QUEST_STATE_OBJECTIVE_0 << objective->Index);
        }
    }

    if (CanCompleteQuest(questId))
        CompleteQuest(questId);
}

//not used in Skyfired, function for external script library
void Player::GroupEventHappens(uint32 questId, WorldObject const* pEventObject)
{
    if (Group* group = GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* player = itr->GetSource();

            // for any leave or dead (with not released body) group member at appropriate distance
            if (player && player->IsAtGroupRewardDistance(pEventObject) && !player->GetCorpse())
                player->AreaExploredOrEventHappens(questId);
        }
    }
    else
        AreaExploredOrEventHappens(questId);
}

void Player::ItemAddedQuestCheck(uint32 entry, uint32 count)
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questid = GetQuestSlotQuestId(i);
        if (questid == 0)
            continue;

        QuestStatusData& questStatus = m_QuestStatus[questid];
        if (questStatus.Status != QUEST_STATUS_INCOMPLETE)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questid);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
            continue;

        for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* questObjective = *citr;
            if (questObjective->Type == QUEST_OBJECTIVE_TYPE_ITEM && questObjective->ObjectId == entry)
            {
                uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                uint32 requiredCounter = uint32(questObjective->Amount);

                if (currentCounter < requiredCounter)
                {
                    uint16 addCount = currentCounter + count <= requiredCounter ? count : requiredCounter - currentCounter;
                    m_questObjectiveStatus[questObjective->Id] += addCount;

                    m_questObjectiveStatusSave[questObjective->Id] = true;
                    m_QuestStatusSave[questid] = true;

                    SendQuestUpdateAddCredit(qInfo, questObjective, ObjectGuid(0), currentCounter, addCount);
                }

                if (CanCompleteQuest(questid))
                    CompleteQuest(questid);

                return;
            }
        }
    }

    UpdateForQuestWorldObjects();
}

void Player::ItemRemovedQuestCheck(uint32 entry, uint32 count)
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questid = GetQuestSlotQuestId(i);
        if (!questid)
            continue;

        QuestStatusData& questStatus = m_QuestStatus[questid];

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questid);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
            continue;

        for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* questObjective = *citr;
            if (questObjective->Type == QUEST_OBJECTIVE_TYPE_ITEM && questObjective->ObjectId == entry)
            {
                uint32 currentCounter = questStatus.Status != QUEST_STATUS_COMPLETE ? GetQuestObjectiveCounter(questObjective->Id) : GetItemCount(entry, true);
                uint32 requiredCounter = uint32(questObjective->Amount);

                if (currentCounter < requiredCounter)
                {
                    uint16 remainingItems = currentCounter <= requiredCounter ? count : count + requiredCounter - currentCounter;
                    m_questObjectiveStatus[questObjective->Id] = (currentCounter <= remainingItems) ? 0 : currentCounter - remainingItems;

                    m_questObjectiveStatusSave[questObjective->Id] = true;
                    m_QuestStatusSave[questid] = true;

                    IncompleteQuest(questid);
                }

                break;
            }
        }
    }

    UpdateForQuestWorldObjects();
}

void Player::KilledMonster(CreatureTemplate const* cInfo, uint64 guid)
{
    ASSERT(cInfo);

    if (cInfo->Entry)
        KilledMonsterCredit(cInfo->Entry, guid);

    for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
        if (cInfo->KillCredit[i])
            KilledMonsterCredit(cInfo->KillCredit[i], 0);
}

void Player::KilledMonsterCredit(uint32 entry, uint64 guid /*= 0*/)
{
    uint16 addKillCount = 1;
    uint32 realEntry = entry;
    Creature* killed = NULL;

    if (guid)
    {
        killed = GetMap()->GetCreature(guid);
        if (killed && killed->GetEntry())
            realEntry = killed->GetEntry();
    }

    StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_CREATURE, realEntry);   // MUST BE CALLED FIRST
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, realEntry, addKillCount, 0, killed);

    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (!questId)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_NPC))
            continue;

        // just if !ingroup || !noraidgroup || raidgroup
        QuestStatusData& questStatus = m_QuestStatus[questId];
        if (questStatus.Status == QUEST_STATUS_INCOMPLETE && (!GetGroup() || !GetGroup()->isRaidGroup() || qInfo->IsAllowedInRaid(GetMap()->GetDifficulty())))
        {
            for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            {
                QuestObjective const* questObjective = *citr;
                if (questObjective->Type == QUEST_OBJECTIVE_TYPE_NPC && questObjective->ObjectId == realEntry)
                {
                    uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                    if (currentCounter < uint32(questObjective->Amount))
                    {
                        m_questObjectiveStatus[questObjective->Id] += addKillCount;

                        m_questObjectiveStatusSave[questObjective->Id] = true;
                        m_QuestStatusSave[questId] = true;

                        SendQuestUpdateAddCredit(qInfo, questObjective, ObjectGuid(guid), currentCounter, addKillCount);
                    }

                    if (CanCompleteQuest(questId))
                        CompleteQuest(questId);

                    break;
                }
            }
        }
    }
}

void Player::KilledPlayerCredit()
{
    uint16 addKillCount = 1;

    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (!questId)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_PLAYER))
            continue;

        // just if !ingroup || !noraidgroup || raidgroup
        QuestStatusData& questStatus = m_QuestStatus[questId];
        if (questStatus.Status == QUEST_STATUS_INCOMPLETE && (!GetGroup() || !GetGroup()->isRaidGroup() || qInfo->IsAllowedInRaid(GetMap()->GetDifficulty())))
        {
            for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            {
                QuestObjective const* questObjective = *citr;
                if (questObjective->Type == QUEST_OBJECTIVE_TYPE_PLAYER)
                {
                    uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                    if (currentCounter < uint32(questObjective->Amount))
                    {
                        m_questObjectiveStatus[questObjective->ObjectId] = currentCounter + addKillCount;

                        m_questObjectiveStatusSave[questObjective->Id] = true;
                        m_QuestStatusSave[questId] = true;

                        SendQuestUpdateAddPlayer(qInfo, questObjective, currentCounter, addKillCount);
                    }

                    if (CanCompleteQuest(questId))
                        CompleteQuest(questId);

                    break;
                }
            }
        }
    }
}

void Player::KillCreditGO(uint32 entry, uint64 guid)
{
    uint16 addKillCount = 1;
    uint32 realEntry = entry;

    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (!questId)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_GO))
            continue;

        // just if !ingroup || !noraidgroup || raidgroup
        QuestStatusData& questStatus = m_QuestStatus[questId];
        if (questStatus.Status == QUEST_STATUS_INCOMPLETE && (!GetGroup() || !GetGroup()->isRaidGroup() || qInfo->IsAllowedInRaid(GetMap()->GetDifficulty())))
        {
            for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            {
                QuestObjective const* questObjective = *citr;
                if (questObjective->Type == QUEST_OBJECTIVE_TYPE_GO && questObjective->ObjectId == realEntry)
                {
                    uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                    if (currentCounter < uint32(questObjective->Amount))
                    {
                        m_questObjectiveStatus[questObjective->Id] += addKillCount;

                        m_questObjectiveStatusSave[questObjective->Id] = true;
                        m_QuestStatusSave[questId] = true;

                        SendQuestUpdateAddCredit(qInfo, questObjective, ObjectGuid(guid), currentCounter, addKillCount);
                    }

                    if (CanCompleteQuest(questId))
                        CompleteQuest(questId);

                    break;
                }
            }
        }
    }
}

void Player::TalkedToCreature(uint32 entry, uint64 guid)
{
    int16 addTalkCount = 1;
    uint32 realEntry = entry;
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questid = GetQuestSlotQuestId(i);
        if (!questid)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questid);
        if (!qInfo)
            continue;

        QuestStatusData& q_status = m_QuestStatus[questid];
        if (q_status.Status == QUEST_STATUS_INCOMPLETE)
        {
            for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
            {
                QuestObjective const* questObjective = *citr;
                {
                    if (questObjective->Type == QUEST_OBJECTIVE_TYPE_NPC_INTERACT && questObjective->ObjectId == realEntry)
                    {
                        uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                        if (currentCounter < uint32(questObjective->Amount))
                        {
                            m_questObjectiveStatus[questObjective->Id] += addTalkCount;

                            m_questObjectiveStatusSave[questObjective->Id] = true;
                            m_QuestStatusSave[questid] = true;

                            SendQuestUpdateAddCredit(qInfo, questObjective, ObjectGuid(guid), currentCounter, addTalkCount);
                        }

                        if (CanCompleteQuest(questid))
                            CompleteQuest(questid);

                        break;
                    }
                }
            }
        }
    }
}

void Player::MoneyChanged(uint32 count)
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (!questId)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_MONEY))
            continue;

        QuestStatusData& questStatus = m_QuestStatus[questId];

        for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* questObjective = *citr;
            if (questObjective->Type == QUEST_OBJECTIVE_TYPE_MONEY)
            {
                if (questStatus.Status == QUEST_STATUS_INCOMPLETE)
                {
                    if (count >= uint32(questObjective->Amount))
                        if (CanCompleteQuest(questId))
                            CompleteQuest(questId);
                }
                else if (questStatus.Status == QUEST_STATUS_COMPLETE)
                {
                    if (count < uint32(questObjective->Amount))
                        IncompleteQuest(questId);
                }
            }
        }
    }
}

void Player::ReputationChanged(FactionEntry const* factionEntry)
{
    ReputationChangedQuestCheck(factionEntry);
}

void Player::ReputationChanged2(FactionEntry const* factionEntry)
{
    ReputationChangedQuestCheck(factionEntry);
}

void Player::ReputationChangedQuestCheck(FactionEntry const* factionEntry)
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (questId)
            continue;

        Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
        if (!qInfo)
            continue;

        if (!qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_FACTION_REP2))
            continue;

        QuestStatusData& questStatus = m_QuestStatus[questId];

        for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* questObjective = *citr;
            if (questObjective->Type == QUEST_OBJECTIVE_TYPE_FACTION_REP2)
            {
                if (questStatus.Status == QUEST_STATUS_INCOMPLETE)
                {
                    if (GetReputationMgr().GetReputation(factionEntry) >= questObjective->Amount)
                        if (CanCompleteQuest(questId))
                            CompleteQuest(questId);
                }
                else if (questStatus.Status == QUEST_STATUS_COMPLETE)
                {
                    if (GetReputationMgr().GetReputation(factionEntry) < questObjective->Amount)
                        IncompleteQuest(questId);
                }
            }
        }
    }
}

void Player::QuestObjectiveSatisfy(uint32 objectId, uint32 amount, uint8 type, uint64 guid)
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (!questId)
            continue;

        Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
        if (!quest)
            continue;

        QuestStatusData& questStatus = m_QuestStatus[questId];
        if (questStatus.Status != QUEST_STATUS_INCOMPLETE)
            continue;

        if (type)
            if (!quest->GetQuestObjectiveCountType(type))
                continue;

        for (QuestObjectiveSet::const_iterator citr = quest->m_questObjectives.begin(); citr != quest->m_questObjectives.end(); ++citr)
        {
            QuestObjective const* questObjective = *citr;
            if (questObjective->Type == type && questObjective->ObjectId == objectId)
            {
                uint32 currentCounter = GetQuestObjectiveCounter(questObjective->Id);
                uint32 requiredCounter = uint32(questObjective->Amount);
                uint32 addCounter = currentCounter + amount > requiredCounter ? requiredCounter - currentCounter : amount;

                m_questObjectiveStatus[questObjective->Id] = addCounter;

                m_questObjectiveStatusSave[questObjective->Id] = true;
                m_QuestStatusSave[questId] = true;

                SendQuestUpdateAddCredit(quest, questObjective, ObjectGuid(guid), currentCounter, amount);

                if (CanCompleteQuest(questId))
                    CompleteQuest(questId);

                break;
            }
        }
    }
}

bool Player::HasQuestForItem(uint32 itemId) const
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (questId == 0)
            continue;

        QuestStatusMap::const_iterator citrQs = m_QuestStatus.find(questId);
        if (citrQs == m_QuestStatus.end())
            continue;

        QuestStatusData const& questStatus = citrQs->second;

        if (questStatus.Status == QUEST_STATUS_INCOMPLETE)
        {
            Quest const* qInfo = sObjectMgr->GetQuestTemplate(questId);
            if (!qInfo)
                continue;

            // hide quest if player is in raid-group and quest is no raid quest
            if (GetGroup() && GetGroup()->isRaidGroup() && !qInfo->IsAllowedInRaid(GetMap()->GetDifficulty()))
            {
                if (!InBattleground()) //there are two ways.. we can make every bg-quest a raidquest, or add this code here.. i don't know if this can be exploited by other quests, but i think all other quests depend on a specific area.. but keep this in mind, if something strange happens later
                    continue;
            }

            // This part for ReqItem drop
            if (qInfo->GetQuestObjectiveCountType(QUEST_OBJECTIVE_TYPE_ITEM))
            {
                for (QuestObjectiveSet::const_iterator citr = qInfo->m_questObjectives.begin(); citr != qInfo->m_questObjectives.end(); ++citr)
                    if ((*citr)->Type == QUEST_OBJECTIVE_TYPE_ITEM)
                        if (itemId == (*citr)->ObjectId && GetQuestObjectiveCounter((*citr)->Id) < uint32((*citr)->Amount))
                            return true;
            }
            // Deliberately not an `else`: a quest can have an item objective AND a source
            // item. The container that yields the objective item is registered in
            // RequiredSourceItemId, so gating this branch behind `else` hid such containers
            // from HasQuestForItem() and LootMgr refused to drop them.
            {
                // This part - for ReqSource
                for (uint8 j = 0; j < QUEST_SOURCE_ITEM_IDS_COUNT; ++j)
                {
                    // examined item is a source item
                    if (qInfo->RequiredSourceItemId[j] == itemId)
                    {
                        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemId);

                        // 'unique' item
                        if (pProto->MaxCount && int32(GetItemCount(itemId, true)) < pProto->MaxCount)
                            return true;

                        // allows custom amount drop when not 0
                        if (qInfo->RequiredSourceItemCount[j])
                        {
                            if (GetItemCount(itemId, true) < qInfo->RequiredSourceItemCount[j])
                                return true;
                        }
                        else if (GetItemCount(itemId, true) < pProto->GetMaxStackSize())
                            return true;
                    }
                }
            }
        }
    }

    return false;
}

void Player::SendQuestComplete(Quest const* quest)
{
    if (quest)
    {
        WorldPacket data(SMSG_QUESTUPDATE_COMPLETE, 4);
        data << uint32(quest->GetQuestId());
        GetSession()->SendPacket(&data);
        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTUPDATE_COMPLETE quest = %u", quest->GetQuestId());
    }
}

void Player::SendQuestCompletionNPCs(uint32 questId)
{
    if (!questId || !sObjectMgr->GetQuestTemplate(questId))
        return;

    std::vector<uint32> entries;
    auto creatures = sObjectMgr->GetCreatureQuestInvolvedRelationReverseBounds(questId);
    for (auto it = creatures.first; it != creatures.second; ++it)
        entries.push_back(it->second);

    auto gos = sObjectMgr->GetGOQuestInvolvedRelationReverseBounds(questId);
    for (auto it = gos.first; it != gos.second; ++it)
        entries.push_back(it->second | 0x80000000); // GO mask

    WorldPacket data(SMSG_QUEST_NPC_QUERY_RESPONSE, 3 + 4 + entries.size() * 4);
    data.WriteBits(1, 21);
    data.WriteBits(entries.size(), 22);
    data.FlushBits();
    data << uint32(questId);
    for (uint32 entry : entries)
        data << uint32(entry);

    GetSession()->SendPacket(&data);
}

void Player::SendQuestReward(Quest const* quest, uint32 XP)
{
    uint32 questId = quest->GetQuestId();
    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_COMPLETE quest = %u", questId);
    sGameEventMgr->HandleQuestComplete(questId);

    uint32 xp;
    uint32 moneyReward;

    if (getLevel() < sWorld->getIntConfig(WorldIntConfigs::CONFIG_MAX_PLAYER_LEVEL))
    {
        xp = XP;
        moneyReward = quest->GetRewMoney();
    }
    else // At max level, increase gold reward
    {
        xp = 0;
        moneyReward = uint32(quest->GetRewMoney() + int32(quest->GetRewMoneyMaxLevel() * sWorld->getRate(Rates::RATE_DROP_MONEY)));
    }

    WorldPacket data(SMSG_QUESTGIVER_QUEST_COMPLETE, 4 + 4 + 4 + 4 + 4 + 4 + 1);
    data.WriteBit(1);                                      // FIXME: unknown bits, common values sent
    data.WriteBit(0);
    data.FlushBits();

    data << uint32(quest->GetBonusTalents());              // bonus talents (still sent to 5.4.x client)
    data << uint32(moneyReward);
    data << uint32(questId);
    data << uint32(quest->GetRewardSkillId());             // 4.x bonus skill id
    data << uint32(xp);
    data << uint32(quest->GetRewardSkillPoints());         // 4.x bonus skill points

    GetSession()->SendPacket(&data);
}

void Player::SendQuestFailed(uint32 questId, InventoryResult reason)
{
    if (questId)
    {
        WorldPacket data(SMSG_QUESTGIVER_QUEST_FAILED, 4 + 4);
        data << uint32(questId);
        data << uint32(reason);                             // failed reason (valid reasons: 4, 16, 50, 17, 74, other values show default message)
        GetSession()->SendPacket(&data);
        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_FAILED");
    }
}

void Player::SendQuestTimerFailed(uint32 quest_id)
{
    if (quest_id)
    {
        WorldPacket data(SMSG_QUESTUPDATE_FAILEDTIMER, 4);
        data << uint32(quest_id);
        GetSession()->SendPacket(&data);
        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTUPDATE_FAILEDTIMER");
    }
}

void Player::SendCanTakeQuestResponse(QuestFailedReason msg) const
{
    WorldPacket data(SMSG_QUESTGIVER_QUEST_INVALID, 5);
    data.WriteBit(1);
    data.FlushBits();
    data << uint32(msg);
    GetSession()->SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTGIVER_QUEST_INVALID");
}

void Player::SendQuestConfirmAccept(const Quest* quest, Player* pReceiver)
{
    if (pReceiver)
    {
        std::string strTitle = quest->GetTitle();

        int loc_idx = pReceiver->GetSession()->GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
            if (const QuestLocale* pLocale = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
                ObjectMgr::GetLocaleString(pLocale->Title, loc_idx, strTitle);

        ObjectGuid guid = GetGUID();

        WorldPacket data(SMSG_QUEST_CONFIRM_ACCEPT, 1 + 8 + 2 + strTitle.size() + 4);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[3]);
        data.WriteBit(0);       // has quest title
        data.WriteBit(guid[2]);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[6]);
        data.WriteBit(guid[4]);
        data.WriteBit(guid[1]);
        data.WriteBits(strTitle.size(), 10);
        data.WriteBit(guid[7]);
        data.FlushBits();

        data.WriteByteSeq(guid[6]);
        data.WriteString(strTitle);
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[2]);
        data.WriteByteSeq(guid[7]);
        data << uint32(quest->GetQuestId());

        pReceiver->GetSession()->SendPacket(&data);

        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUEST_CONFIRM_ACCEPT");
    }
}

void Player::SendPushToPartyResponse(Player* player, uint8 msg)
{
    if (player)
    {
        ObjectGuid guid = player->GetGUID();

        WorldPacket data(SMSG_QUEST_PUSH_RESULT, 1 + 8 + 1);
        data.WriteGuidMask(guid, 3, 0, 1, 4, 7, 5, 6, 2);

        data.WriteGuidBytes(guid, 4);
        data << uint8(msg);
        data.WriteGuidBytes(guid, 1, 5, 3, 7, 6, 2, 0);

        GetSession()->SendPacket(&data);

        SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUEST_PUSH_RESULT");
    }
}

void Player::SendQuestUpdateAddCredit(Quest const* quest, QuestObjective const* objective, ObjectGuid guid, uint16 oldCount, uint16 addCount)
{
    WorldPacket data(SMSG_QUESTUPDATE_ADD_CREDIT, 1 + 8 + 2 + 1 + 4 + 2 + 4);
    data << uint16(oldCount + addCount);
    data << uint8(objective->Type);
    data << uint32(quest->GetQuestId());
    data << uint16(objective->Amount);
    data << uint32(objective->ObjectId);

    data.WriteGuidMask(guid, 0, 4, 2, 6, 1, 5, 7, 3);
    data.WriteGuidBytes(guid, 2, 7, 3, 0, 4, 5, 1, 6);

    GetSession()->SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTUPDATE_ADD_KILL");

    uint16 logSlot = FindQuestSlot(quest->GetQuestId());
    if (logSlot >= MAX_QUEST_LOG_SIZE)
        return;

    // Kill/collect objectives store progress in per-index counters.
    SetQuestSlotCounter(logSlot, objective->Index, GetQuestSlotCounter(logSlot, objective->Index) + addCount);

    // DUMMY / pet-battle objectives are flagged in the state field instead.
    // Without this bit, GetQuestLogTitle reports incomplete and WatchFrame
    // will not render the COMPLETE autocomplete toast.
    switch (objective->Type)
    {
        case QUEST_OBJECTIVE_TYPE_DUMMY:
        case QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER:
        case QUEST_OBJECTIVE_TYPE_PET_BATTLE_ELITE:
        case QUEST_OBJECTIVE_TYPE_PET_BATTLE_PVP:
            if (objective->Index < 24)
                SetQuestSlotState(logSlot, QUEST_STATE_OBJECTIVE_0 << objective->Index);
            break;
        default:
            break;
    }
}

void Player::SendQuestUpdateAddPlayer(Quest const* quest, QuestObjective const* objective, uint16 oldCount, uint16 addCount)
{
    WorldPacket data(SMSG_QUESTUPDATE_ADD_PVP_KILL, 4 + 2);
    data << uint32(quest->GetQuestId());
    data << uint32(oldCount + addCount);

    GetSession()->SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_QUESTUPDATE_ADD_PVP_KILL");

    uint16 logSlot = FindQuestSlot(quest->GetQuestId());
    if (logSlot < MAX_QUEST_LOG_SIZE)
        SetQuestSlotCounter(logSlot, objective->Index, GetQuestSlotCounter(logSlot, objective->Index) + addCount);
}

bool Player::HasPvPForcingQuest() const
{
    for (uint8 i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        uint32 questId = GetQuestSlotQuestId(i);
        if (questId == 0)
            continue;

        Quest const* quest = sObjectMgr->GetQuestTemplate(questId);
        if (!quest)
            continue;

        if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
            return true;
    }

    return false;
}

/*********************************************************/
/***                   LOAD SYSTEM                     ***/
/*********************************************************/

uint32 Player::GetQuestObjectiveCounter(uint32 objectiveId) const
{
    QuestObjectiveStatusMap::const_iterator citr = m_questObjectiveStatus.find(objectiveId);
    if (citr != m_questObjectiveStatus.end())
        return citr->second;

    return 0;
}
