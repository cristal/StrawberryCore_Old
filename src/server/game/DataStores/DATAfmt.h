/*
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 Strawberry-Pr0jcts <http://www.strawberry-pr0jcts.com/>
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

#ifndef STRAWBERRY_DBCSFRM_H
#define STRAWBERRY_DBCSFRM_H

#include "Logging/Log.h"

enum DATAFormats
{
    Achievementfmt,
    AchievementCriteriafmt,
    AreaTableEntryfmt,
    AreaGroupEntryfmt,
    AreaPOIEntryfmt,
    AreaTriggerEntryfmt,
    ArmorLocationfmt,
    AuctionHouseEntryfmt,
    BankBagSlotPricesEntryfmt,
    BarberShopStyleEntryfmt,
    BattlemasterListEntryfmt,
    CharStartOutfitEntryfmt,
    CharTitlesEntryfmt,
    ChatChannelsEntryfmt,
    ChrClassesEntryfmt,
    ChrRacesEntryfmt,
    ChrClassesXPowerTypesfmt,
    CinematicSequencesEntryfmt,
    CreatureDisplayInfofmt,
    CreatureFamilyfmt,
    CreatureSpellDatafmt,
    CreatureTypefmt,
    CurrencyTypesfmt,
    DestructibleModelDatafmt,
    DungeonEncounterfmt,
    DurabilityCostsfmt,
    DurabilityQualityfmt,
    EmotesEntryfmt,
    EmotesTextEntryfmt,
    FactionEntryfmt,
    FactionTemplateEntryfmt,
    GameObjectDisplayInfofmt,
    GemPropertiesEntryfmt,
    GlyphPropertiesfmt,
    GlyphSlotfmt,
    GtBarberShopCostBasefmt,
    GtCombatRatingsfmt,
    GtChanceToMeleeCritBasefmt,
    GtChanceToMeleeCritfmt,
    GtChanceToSpellCritBasefmt,
    GtChanceToSpellCritfmt,
    GtOCTClassCombatRatingScalarfmt,
    //GtOCTRegenHPfmt,
    //GtOCTRegenMPfmt,
    //GtRegenHPPerSptfmt,
    GtOCTHpPerStaminafmt,
    GtRegenMPPerSptfmt,
    GtSpellScalingfmt,
    Holidaysfmt,
    ItemArmorQualityfmt,
    ItemArmorShieldfmt,
    ItemArmorTotalfmt,
    ItemBagFamilyfmt,
    //ItemDisplayTemplateEntryfmt,
    //ItemCondExtCostsEntryfmt,
    ItemDamagefmt,
    ItemLimitCategoryEntryfmt,
    ItemRandomPropertiesfmt,
    ItemRandomSuffixfmt,
    ItemSetEntryfmt,
    ItemReforgefmt,
    LFGDungeonEntryfmt,
    LockEntryfmt,
    MailTemplateEntryfmt,
    MapEntryfmt,
    MapDifficultyEntryfmt,
    MountCapabilityfmt,
    MountTypefmt,
    MovieEntryfmt,
    OverrideSpellDatafmt,
    QuestSortEntryfmt,
    QuestXPfmt,
    QuestFactionRewardfmt,
    PvPDifficultyfmt,
    RandomPropertiesPointsfmt,
    ScalingStatDistributionfmt,
    ScalingStatValuesfmt,
    SkillLinefmt,
    SkillLineAbilityfmt,
    SoundEntriesfmt,
    SpellCastTimefmt,
    SpellDifficultyfmt,
    SpellDurationfmt,
    SpellEntryfmt,
    True_SpellEntryfmt,
    SpellAuraOptionsEntryfmt,
    SpellAuraRestrictionsEntryfmt,
    SpellCastingRequirementsEntryfmt,
    SpellCategoriesEntryfmt,
    SpellClassOptionsEntryfmt,
    SpellCooldownsEntryfmt,
    SpellEffectEntryfmt,
    SpellEquippedItemsEntryfmt,
    SpellInterruptsEntryfmt,
    SpellLevelsEntryfmt,
    SpellPowerEntryfmt,
    SpellReagentsEntryfmt,
    SpellScalingEntryfmt,
    SpellShapeshiftEntryfmt,
    SpellTargetRestrictionsEntryfmt,
    SpellTotemsEntryfmt,
    SpellFocusObjectfmt,
    SpellItemEnchantmentfmt,
    SpellItemEnchantmentConditionfmt,
    SpellRadiusfmt,
    SpellRangefmt,
    SpellRuneCostfmt,
    SpellShapeshiftFormfmt,
    SummonPropertiesfmt,
    GuildPerksfmt,
    TalentEntryfmt,
    TalentTabEntryfmt,
    TalentTreePrimarySpellsfmt,
    TaxiNodesEntryfmt,
    TaxiPathEntryfmt,
    TaxiPathNodeEntryfmt,
    TotemCategoryEntryfmt,
    VehicleEntryfmt,
    VehicleSeatEntryfmt,
    WMOAreaTableEntryfmt,
    WorldMapAreaEntryfmt,
    WorldMapOverlayEntryfmt,
    WorldSafeLocsEntryfmt,
    PhaseEntryfmt,
    Itemfmt,
    ItemCurrencyCostfmt,
    ItemExtendedCostEntryfmt,
    LastFormatData
};

extern void LoadDATAStores();

class FormatTableHandler
{
public:
    void LoadDATAFormatsFromDB();
    std::string GetFormatTable(std::string name);
    std::map<std::string, std::string> FormatTableContainer;
};

#define sFormatTableHandler ACE_Singleton<FormatTableHandler, ACE_Null_Mutex>::instance()

struct FormatHandler
{
    const char* name;
    DATAFormats enumValue;
};

#define MAX_FORMAT_TYPES 500
extern FormatHandler formatTable[MAX_FORMAT_TYPES];
extern char* formatEnumToFormat[LastFormatData];

inline const char* LookupFormatName(DATAFormats enumValue)
{
    if (enumValue >= LastFormatData)
        return "Received unknown format name, it's more than max!";

    return formatTable[enumValue].name;
}

inline const char* LookupFormat(DATAFormats enumValue)
{
    return formatEnumToFormat[enumValue];
}

#endif
