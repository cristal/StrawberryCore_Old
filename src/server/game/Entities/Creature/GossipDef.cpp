/*
 * Copyright (C) 2005-2011 MaNGOS <http://www.getmangos.com/>
 *
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 VoragineCore <http://www.projectvoragine.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "gamePCH.h"
#include "QuestDef.h"
#include "GossipDef.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Formulas.h"

GossipMenu::GossipMenu()
{
    _menuId = 0;
}

GossipMenu::~GossipMenu()
{
    ClearMenu();
}

void GossipMenu::AddMenuItem(int32 menuItemId, uint8 icon, std::string const& message, uint32 sender, uint32 action, std::string const& boxMessage, uint32 boxMoney, bool coded /*= false*/)
{
    ASSERT(_menuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    // Find a free new id - script case
    if (menuItemId == -1)
    {
        menuItemId = 0;
        if (!_menuItems.empty())
        {
            for (GossipMenuItemContainer::const_iterator itr = _menuItems.begin(); itr != _menuItems.end(); ++itr)
            {
                if (int32(itr->first) > menuItemId)
                {
                    menuItemId = menuItemId;
                    break;
                }

                menuItemId = itr->first + 1;
            }
        }
    }

    GossipMenuItem& menuItem = _menuItems[menuItemId];

    menuItem.MenuItemIcon       = icon;
    menuItem.Message            = message;
    menuItem.IsCoded            = coded;
    menuItem.Sender             = sender;
    menuItem.OptionType         = action;
    menuItem.BoxMessage         = boxMessage;
    menuItem.BoxMoney           = boxMoney;
}

void GossipMenu::AddGossipMenuItemData(uint32 menuItemId, uint32 gossipActionMenuId, uint32 gossipActionPoi, uint32 gossipActionScriptId)
{
    GossipMenuItemData& itemData = _menuItemData[menuItemId];

    itemData.GossipActionMenuId   = gossipActionMenuId;
    itemData.GossipActionPoi      = gossipActionPoi;
    itemData.GossipActionScriptId = gossipActionScriptId;
}

uint32 GossipMenu::GetMenuItemSender(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.Sender;
}

uint32 GossipMenu::GetMenuItemAction(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return 0;

    return itr->second.OptionType;
}

bool GossipMenu::IsMenuItemCoded(uint32 menuItemId) const
{
    GossipMenuItemContainer::const_iterator itr = _menuItems.find(menuItemId);
    if (itr == _menuItems.end())
        return false;

    return itr->second.IsCoded;
}

void GossipMenu::ClearMenu()
{
    _menuItems.clear();
    _menuItemData.clear();
}

PlayerMenu::PlayerMenu(WorldSession *session) : _session(session)
{
}

PlayerMenu::~PlayerMenu()
{
    ClearMenus();
}

void PlayerMenu::ClearMenus()
{
    _gossipMenu.ClearMenu();
    _questMenu.ClearMenu();
}

void PlayerMenu::SendGossipMenu(uint32 titleTextId, uint64 objectGUID) const
{
    WorldPacket data(SMSG_GOSSIP_MESSAGE, 100);         // guess size
    data << uint64(objectGUID);
    data << uint32(_gossipMenu.GetMenuId());            // new 2.4.0
    data << uint32(titleTextId);
    data << uint32(_gossipMenu.GetMenuItemCount());     // max count 0x10

    for (GossipMenuItemContainer::const_iterator itr = _gossipMenu.GetMenuItems().begin(); itr != _gossipMenu.GetMenuItems().end(); ++itr)
    {
        GossipMenuItem const& item = itr->second;
        data << uint32(itr->first);
        data << uint8(item.MenuItemIcon);
        data << uint8(item.IsCoded);                    // makes pop up box password
        data << uint32(item.BoxMoney);                  // money required to open menu, 2.0.3
        data << item.Message;                           // text for gossip item
        data << item.BoxMessage;                        // accept text (related to money) pop up box, 2.0.3
    }

    data << uint32(_questMenu.GetMenuItemCount());             // max count 0x20

    for (uint32 iI = 0; iI < _questMenu.GetMenuItemCount(); ++iI)
    {
        QuestMenuItem const& item = _questMenu.GetItem(iI);
        uint32 questID = item.QuestId;
        Quest const* quest = sObjectMgr->GetQuestTemplate(questID);

        data << uint32(questID);
        data << uint32(item.QuestIcon);
        data << int32(quest->GetQuestLevel());
        data << uint32(quest->GetFlags());              // 3.3.3 quest flags
        data << uint8(0);                               // 3.3.3 changes icon: blue question or yellow exclamation
        std::string title = quest->GetTitle();

        int locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (QuestLocale const* localeData = sObjectMgr->GetQuestLocale(questID))
                sObjectMgr->GetLocaleString(localeData->Title, locale, title);

        data << title;                                  // max 0x200
    }

    _session->SendPacket(&data);
}

void PlayerMenu::SendCloseGossip() const
{
    WorldPacket data(SMSG_GOSSIP_COMPLETE, 0);
    _session->SendPacket(&data);
}

void PlayerMenu::SendPointOfInterest(uint32 poiId) const
{
    PointOfInterest const* poi = sObjectMgr->GetPointOfInterest(poiId);
    if (!poi)
    {
        sLog->outErrorDb("Request to send non-existing POI (Id: %u), ignored.", poiId);
        return;
    }

    std::string iconText = poi->icon_name;
    int32 locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (PointOfInterestLocale const *localeData = sObjectMgr->GetPointOfInterestLocale(poiId))
            sObjectMgr->GetLocaleString(localeData->IconName, locale, iconText);

    WorldPacket data(SMSG_GOSSIP_POI, 4 + 4 + 4 + 4 + 4 + 10);  // guess size
    data << uint32(poi->flags);
    data << float(poi->x);
    data << float(poi->y);
    data << uint32(poi->icon);
    data << uint32(poi->data);
    data << iconText;

    _session->SendPacket(&data);
}

void PlayerMenu::SendTalking(uint32 textID)
{
    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    data << textID;                                         // can be < 0

    if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i] = pGossip->Options[i].Text_0;
            Text_1[i] = pGossip->Options[i].Text_1;
        }
        int locale = _session->GetSessionDbLocaleIndex();
        if (locale >= 0)
            if (NpcTextLocale const *nl = sObjectMgr->GetNpcTextLocale(textID))
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    sObjectMgr->GetLocaleString(nl->Text_0[i], locale, Text_0[i]);
                    sObjectMgr->GetLocaleString(nl->Text_1[i], locale, Text_1[i]);
                }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }
    _session->SendPacket(&data);

    sLog->outDebug("WORLD: Sent SMSG_NPC_TEXT_UPDATE ");
}

void PlayerMenu::SendTalking(char const * title, char const * text)
{
    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 50);           // guess size
    data << uint32(0);
    for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
    {
        data << float(0);
        data << title;
        data << text;
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
    }

    _session->SendPacket(&data);

    sLog->outDebug("WORLD: Sent SMSG_NPC_TEXT_UPDATE ");
}

/*********************************************************/
/***                    QUEST SYSTEM                   ***/
/*********************************************************/

QuestMenu::QuestMenu()
{
    _questMenuItems.reserve(16);                                   // can be set for max from most often sizes to speedup push_back and less memory use
}

QuestMenu::~QuestMenu()
{
    ClearMenu();
}

void QuestMenu::AddMenuItem(uint32 QuestId, uint8 Icon)
{
    if (!sObjectMgr->GetQuestTemplate(QuestId))
        return;

    ASSERT(_questMenuItems.size() <= GOSSIP_MAX_MENU_ITEMS);

    QuestMenuItem questMenuItem;

    questMenuItem.QuestId        = QuestId;
    questMenuItem.QuestIcon      = Icon;

    _questMenuItems.push_back(questMenuItem);
}

bool QuestMenu::HasItem(uint32 questId) const
{
    for (QuestMenuItemList::const_iterator i = _questMenuItems.begin(); i != _questMenuItems.end(); ++i)
        if (i->QuestId == questId)
            return true;

    return false;
}

void QuestMenu::ClearMenu()
{
    _questMenuItems.clear();
}

void PlayerMenu::SendQuestGiverQuestList(QEmote eEmote, const std::string& Title, uint64 npcGUID)
{
    WorldPacket data(SMSG_QUESTGIVER_QUEST_LIST, 100);    // guess size
    data << uint64(npcGUID);
    data << Title;
    data << uint32(eEmote._Delay);                         // player emote
    data << uint32(eEmote._Emote);                         // NPC emote

    size_t count_pos = data.wpos();
    data << uint8 ( _questMenu.GetMenuItemCount());
    uint32 count = 0;
    for (; count < _questMenu.GetMenuItemCount(); ++count)
    {
        QuestMenuItem const& qmi = _questMenu.GetItem(count);

        uint32 questID = qmi.QuestId;

        if (Quest const *quest = sObjectMgr->GetQuestTemplate(questID))
        {
            std::string title = quest->GetTitle();

            int locale = _session->GetSessionDbLocaleIndex();
            if (locale >= 0)
                if (QuestLocale const *ql = sObjectMgr->GetQuestLocale(questID))
                    sObjectMgr->GetLocaleString(ql->Title, locale, title);

            data << uint32(questID);
            data << uint32(qmi.QuestIcon);
            data << int32(quest->GetQuestLevel());
            data << uint32(quest->GetQuestFlags());        // 3.3.3 quest flags
            data << uint8(0);                               // 3.3.3 changes icon: blue question or yellow exclamation
            data << title;
        }
    }

    data.put<uint8>(count_pos, count);
    _session->SendPacket(&data);
    sLog->outDebug("WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST NPC Guid=%u", GUID_LOPART(npcGUID));
}

void PlayerMenu::SendQuestGiverStatus(uint32 questStatus, uint64 npcGUID) const
{
    WorldPacket data(SMSG_QUESTGIVER_STATUS, 11);
    data << uint64(npcGUID);
    data << uint32(questStatus);

    _session->SendPacket(&data);
    sLog->outDebug("WORLD: Sent SMSG_QUESTGIVER_STATUS NPC Guid=%u, status=%u", GUID_LOPART(npcGUID), questStatus);
}

void PlayerMenu::SendQuestGiverQuestDetails(Quest const *quest, uint64 npcGUID, bool activateAccept) const
{
    std::string questTitle      = quest->GetTitle();
    std::string questDetails    = quest->GetDetails();
    std::string questObjectives = quest->GetObjectives();
    std::string questEndText    = quest->GetEndText();
    std::string questTargetTextWindow = quest->GetQuestGiverPortraitText();
    std::string questTargetName  = quest->GetQuestGiverPortraitUnk();
    std::string unk        = "";

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const *ql = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            sObjectMgr->GetLocaleString(ql->Title, locale, questTitle);
            sObjectMgr->GetLocaleString(ql->Details, locale, questDetails);
            sObjectMgr->GetLocaleString(ql->Objectives, locale, questObjectives);
            sObjectMgr->GetLocaleString(ql->EndText, locale, questEndText);
        }
    }

    WorldPacket data(SMSG_QUESTGIVER_QUEST_DETAILS, 100);   // guess size
    data << uint64(npcGUID);
    data << uint64(0);                                      // in Cata (4.0.6) sometimes npcGUID for quest sharing?
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << questDetails;
    data << questObjectives;
    data << questTargetTextWindow;
    data << questTargetName;
    data << uint16(0);                                      // Unknown Value maybe string                             
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(0);
    data << uint8(activateAccept ? 1 : 0);  
    data << uint32(quest->GetQuestFlags());
    data << uint32(quest->GetSuggestedPlayers());
    data << uint8(0);              //Empty?
    data << uint8(quest->GetQuestStartType());   
    data << uint32(quest->GetRequiredSpell());

    data << uint32(quest->GetRewChoiceItemsCount());
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(quest->RewChoiceItemId[i]);
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(quest->RewChoiceItemCount[i]);
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0x00);
    }

    data << uint32(quest->GetRewItemsCount());

    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(quest->RewItemId[i]);
    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(quest->RewItemCount[i]);
    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(quest->GetRewOrReqMoney());
    data << uint32(quest->XPValue(_session->GetPlayer())*sWorld->getRate(RATE_XP_QUEST));

    data << uint32(quest->GetCharTitleId());
    data << uint32(0); // unknow 4.0.1
    data << uint32(0); // unknow 4.0.1
    data << uint32(quest->GetBonusTalents());
    data << uint32(0); // unknow 4.0.1
    data << uint32(0); // unknow 4.0.1

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << uint32(quest->RewRepFaction[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(quest->RewRepValueId[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(quest->RewRepValue[i]);

    data << int32(quest->GetRewSpellCast());
    data << uint32(0); // unknow 4.0.1 Spellcast?

    for(int i = 0; i < 4; i++)
        data << uint32(0);

    for(int i = 0; i < 4; i++)
        data << uint32(0);

    data << uint32(0);
    data << uint32(0);

    data << uint32(QUEST_EMOTE_COUNT);
    for (uint32 i=0; i < QUEST_EMOTE_COUNT; ++i)
    {
        data << uint32(quest->DetailsEmote[i]);
        data << uint32(quest->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }
    _session->SendPacket(&data);

    sLog->outDebug("WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}

void PlayerMenu::SendQuestQueryResponse(Quest const *quest) const
{
    std::string ObjectiveText[QUEST_OBJECTIVES_COUNT];
    std::string questTitle = quest->GetTitle();
    std::string questDetails = quest->GetDetails();
    std::string questObjectives = quest->GetObjectives();
    std::string questEndText = quest->GetEndText();
    std::string questCompletedText = quest->GetCompletedText();
    std::string questGiverTextWindow = quest->GetQuestGiverPortraitText();
    std::string questGiverTargetName = quest->GetQuestGiverPortraitUnk();
    std::string questTurnTextWindow = quest->GetQuestTurnInPortraitText();
    std::string questTurnTargetName = quest->GetQuestTurnInPortraitUnk();

    for (int i=0; i<QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i]=quest->ObjectiveText[i];

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
    {
        if (QuestLocale const *ql = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            sObjectMgr->GetLocaleString(ql->Title, locale, questTitle);
            sObjectMgr->GetLocaleString(ql->Details, locale, questDetails);
            sObjectMgr->GetLocaleString(ql->Objectives, locale, questObjectives);
            sObjectMgr->GetLocaleString(ql->EndText, locale, questEndText);
            sObjectMgr->GetLocaleString(ql->CompletedText, locale, questCompletedText);

            for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
                sObjectMgr->GetLocaleString(ql->ObjectiveText[i], locale, ObjectiveText[i]);
        }
    }

    WorldPacket data(SMSG_QUEST_QUERY_RESPONSE, 100);     // guess size

    data << uint32(quest->GetQuestId());                   // quest id
    data << uint32(quest->GetQuestMethod());               // Accepted values: 0, 1 or 2. 0 == IsAutoComplete() (skip objectives/details)
    data << uint32(quest->GetQuestLevel());                // may be -1, static data, in other cases must be used dynamic level: Player::GetQuestLevel (0 is not known, but assuming this is no longer valid for quest intended for client)
    data << uint32(quest->GetMinLevel());                  // min level
    data << uint32(quest->GetZoneOrSort());                // zone or sort to display in quest log

    data << uint32(quest->GetType());                      // quest type
    data << uint32(quest->GetSuggestedPlayers());          // suggested players count

    data << uint32(quest->GetRepObjectiveFaction());       // shown in quest log as part of quest objective
    data << uint32(quest->GetRepObjectiveValue());         // shown in quest log as part of quest objective

    data << uint32(quest->GetRepObjectiveFaction2());       // shown in quest log as part of quest objective OPOSITE faction
    data << uint32(quest->GetRepObjectiveValue2());         // shown in quest log as part of quest objective OPPOSITE faction

    data << uint32(quest->GetNextQuestInChain());          // client will request this quest from NPC, if not 0
    data << uint32(quest->GetXPId());                      // used for calculating rewarded experience

    data << uint32(quest->GetRewOrReqMoney());             // reward money (below max lvl)
    data << uint32(quest->GetRewMoneyMaxLevel());
    data << uint32(quest->GetRewSpell());                  // reward spell, this spell will display (icon) (casted if RewSpellCast == 0)
    data << int32(quest->GetRewSpellCast());               // casted spell
    data << uint32(0);
    data << uint32(0);

    data << uint32(quest->GetSrcItemId());                 // source item id
    data << uint32(quest->GetFlags() & 0xFFFF);                      // quest flags
    data << uint32(quest->GetQuestTargetMark());           // Minimap Target Mark, 1-Skull, 16-Unknown
    data << uint32(quest->GetCharTitleId());               // CharTitleId, new 2.4.0, player gets this title (id from CharTitles)
    data << uint32(quest->GetPlayersSlain());              // players slain
    data << uint32(quest->GetBonusTalents());              // bonus talents
    data << uint32(quest->GetRewArenaPoints());            // bonus arena points
    data << uint32(quest->GetRewSkillLineId());            // reward skill line id
    data << uint32(quest->GetRewSkillPoints());            // reward skill points
    data << uint32(quest->GetRewRepMask());                // review rep show mask
    data << uint32(quest->GetQuestGiverPortrait());        // questgiver portrait ID
    data << uint32(quest->GetQuestTurnInPortrait());       // quest turn in portrait ID

    int iI;

    if (quest->HasFlag(QUEST_FLAGS_HIDDEN_REWARDS))
    {
        for (iI = 0; iI < QUEST_REWARDS_COUNT; ++iI)
            data << uint32(0) << uint32(0);
        for (iI = 0; iI < QUEST_REWARD_CHOICES_COUNT; ++iI)
            data << uint32(0) << uint32(0);
    }
    else
    {
        for (iI = 0; iI < QUEST_REWARDS_COUNT; ++iI)
        {
            data << uint32(quest->RewItemId[iI]);
            data << uint32(quest->RewItemCount[iI]);
        }
        for (iI = 0; iI < QUEST_REWARD_CHOICES_COUNT; ++iI)
        {
            data << uint32(quest->RewChoiceItemId[iI]);
            data << uint32(quest->RewChoiceItemCount[iI]);
        }
    }

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)         // reward factions ids
        data << uint32(quest->RewRepFaction[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)         // columnid+1 QuestFactionReward.dbc?
        data << int32(quest->RewRepValueId[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)         // unk (0)
        data << int32(quest->RewRepValue[i]);

    data << uint32(quest->GetPointMapId());
    data << float(quest->GetPointX());
    data << float(quest->GetPointY());
    data << uint32(quest->GetPointOpt());

    data << questTitle;
    data << questObjectives;
    data << questDetails;
    data << questCompletedText;                                  // display in quest objectives window once all objectives are completed
    data << questEndText;

    for (iI = 0; iI < QUEST_OBJECTIVES_COUNT; ++iI)
    {
        if (quest->ReqCreatureOrGOId[iI] < 0)
            // client expected gameobject template id in form (id|0x80000000)
            data << uint32((quest->ReqCreatureOrGOId[iI]*(-1))|0x80000000);
        else
            data << uint32(quest->ReqCreatureOrGOId[iI]);
        data << uint32(quest->ReqCreatureOrGOCount[iI]);
        data << uint32(quest->ReqSourceId[iI]);            // item drop intermediate ID
        data << uint32(quest->ReqSourceCount[iI]);         // item drop intermediate count
    }

    for (iI = 0; iI < QUEST_ITEM_OBJECTIVES_COUNT; ++iI)
    {
        data << uint32(quest->ReqItemId[iI]);
        data << uint32(quest->ReqItemCount[iI]);
    }

    data << uint32(quest->GetRequiredSpell());

    for (iI = 0; iI < QUEST_OBJECTIVES_COUNT; ++iI)
        data << ObjectiveText[iI];

    for(iI = 0; iI < 4; ++iI)                               // 4.0.0 currency reward id and count
    {
        data << uint32(quest->RewCurrencyId[iI]);
        data << uint32(quest->RewCurrencyCount[iI]);
    }

    for(iI = 0; iI < 4; ++iI)                               // 4.0.0 currency required id and count
    {
        data << uint32(quest->ReqCurrencyId[iI]);
        data << uint32(quest->ReqCurrencyCount[iI]);
    }

    data << questGiverTextWindow;
    data << questGiverTargetName;
    data << questTurnTextWindow;
    data << questTurnTargetName;

    data << uint32(quest->GetSoundAccept());
    data << uint32(quest->GetSoundTurnIn());
    
    _session->SendPacket(&data);
    sLog->outDebug("WORLD: Sent SMSG_QUEST_QUERY_RESPONSE questid=%u", quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverOfferReward(Quest const* quest, uint64 npcGUID, bool enableNext) const
{
    std::string questTitle = quest->GetTitle();
    std::string questOfferRewardText = quest->GetOfferRewardText();
    std::string questGiverTextWindow = quest->GetQuestGiverPortraitText();
    std::string questGiverName = quest->GetQuestGiverPortraitUnk();
    std::string questCompleteTextWindow = quest->GetQuestTurnInPortraitText();
    std::string questCompleteName = quest->GetQuestTurnInPortraitUnk();

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (QuestLocale const *ql = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            sObjectMgr->GetLocaleString(ql->Title, locale, questTitle);
            sObjectMgr->GetLocaleString(ql->OfferRewardText, locale, questOfferRewardText);
        }

    WorldPacket data(SMSG_QUESTGIVER_OFFER_REWARD, 80);   // guess size
    data << uint64(npcGUID);
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << questOfferRewardText;

    data << questGiverTextWindow;
    data << questGiverName;
    data << questCompleteTextWindow;
    data << questCompleteName;
    data << uint32(quest->GetQuestGiverPortrait());
    data << uint32(quest->GetQuestTurnInPortrait());         // 4.0.6
    data << uint8(enableNext ? 1 : 0);                        // Auto Finish
    data << uint32(quest->GetFlags());                        // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());            // SuggestedGroupNum

    uint32 EmoteCount = 0;
    for (uint32 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        if (quest->OfferRewardEmote[i] <= 0)
            break;
        ++EmoteCount;
    }

    data << EmoteCount;                                     // Emote Count
    for (uint32 i = 0; i < EmoteCount; ++i)
    {
        data << uint32(quest->OfferRewardEmoteDelay[i]);   // Delay Emote
        data << uint32(quest->OfferRewardEmote[i]);
    }

    data << uint32(quest->GetRewChoiceItemsCount());
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(quest->RewChoiceItemId[i]);
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        data << uint32(quest->RewChoiceItemCount[i]);
    for (uint32 i=0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0x00);
    }

    data << uint32(quest->GetRewItemsCount());

    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(quest->RewItemId[i]);
    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
        data << uint32(quest->RewItemCount[i]);
    for (uint32 i=0; i < QUEST_REWARDS_COUNT; ++i)
    {
        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->RewChoiceItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    data << uint32(quest->GetRewOrReqMoney());

    data << uint32(quest->XPValue(_session->GetPlayer())*sWorld->getRate(RATE_XP_QUEST)); // 4.0.6
    data << uint32(quest->GetCharTitleId());
    data << uint32(0); // Unknown 4.0.6
    data << uint32(0); // Unknown 4.0.6
    data << uint32(quest->GetBonusTalents());
    data << uint32(0); // Unknown 4.0.6
    data << uint32(0); // Unknown 4.0.6

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << uint32(quest->RewRepFaction[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(quest->RewRepValueId[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        data << int32(quest->RewRepValue[i]);

    data << int32(quest->GetRewSpellCast());
    data << uint32(0); // Probably invisible spell cast

    for(int i = 0; i < 4; i++)
        data << uint32(0);

    for(int i = 0; i < 4; i++)
        data << uint32(0);

    data << uint32(0);
    data << uint32(0);

    _session->SendPacket(&data);
    sLog->outDebug("WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}

void PlayerMenu::SendQuestGiverRequestItems(Quest const *quest, uint64 npcGUID, bool completable, bool closeOnCancel) const
{
    // We can always call to RequestItems, but this packet only goes out if there are actually
    // items.  Otherwise, we'll skip straight to the OfferReward
    std::string questTitle = quest->GetTitle();
    std::string questRequestItemsText = quest->GetRequestItemsText();

    int locale = _session->GetSessionDbLocaleIndex();
    if (locale >= 0)
        if (QuestLocale const *ql = sObjectMgr->GetQuestLocale(quest->GetQuestId()))
        {
            sObjectMgr->GetLocaleString(ql->Title, locale, questTitle);
            sObjectMgr->GetLocaleString(ql->RequestItemsText, locale, questRequestItemsText);
        }

    if (!quest->GetReqItemsCount() && completable)
    {
        SendQuestGiverOfferReward(quest, npcGUID, true);
        return;
    }

    WorldPacket data(SMSG_QUESTGIVER_REQUEST_ITEMS, 50);  // guess size
    data << uint64(npcGUID);
    data << uint32(quest->GetQuestId());
    data << questTitle;
    data << questRequestItemsText;

    data << uint32(0x00);                                   // unknown

    if (completable)
        data << quest->GetCompleteEmote();
    else
        data << quest->GetIncompleteEmote();

    // Close Window after cancel
    if (closeOnCancel)
        data << uint32(0x01);
    else
        data << uint32(0x00);

    data << uint32(quest->GetQuestFlags());                // 3.3.3 questFlags
    data << uint32(quest->GetSuggestedPlayers());          // SuggestedGroupNum

    // Required Money
    data << uint32(quest->GetRewOrReqMoney() < 0 ? -quest->GetRewOrReqMoney() : 0);

    data << uint32(quest->GetReqItemsCount());
    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!quest->ReqItemId[i])
            continue;

        data << uint32(quest->ReqItemId[i]);
        data << uint32(quest->ReqItemCount[i]);

        if (ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(quest->ReqItemId[i]))
            data << uint32(itemTemplate->DisplayInfoID);
        else
            data << uint32(0);
    }

    // Added in 4.0.1
    uint32 counter = 0;
    data << counter;
    for(uint32 i = 0; i < counter; i++)
    {
        data << uint32(0);
        data << uint32(0);
    }

    if (!completable)
        data << uint32(0x00);
    else
        data << uint32(0x02);

    data << uint32(0x04);
    data << uint32(0x08);
    data << uint32(0x10);
    data << uint32(0x40); // added in 4.0.1

    _session->SendPacket(&data);
    sLog->outDebug("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS NPCGuid=%u, questid=%u", GUID_LOPART(npcGUID), quest->GetQuestId());
}