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

/* ScriptData
SDName: Azuremyst_Isle
SD%Complete: 75
SDComment: Quest support: 9283, 9537, 9582, 9554, 9531, 9303(special flight path, proper model for mount missing). Injured Draenei cosmetic only, 9582.
SDCategory: Azuremyst Isle
EndScriptData */

/* ContentData
npc_draenei_survivor
npc_engineer_spark_overgrind
npc_injured_draenei
npc_magwin
npc_geezle
mob_nestlewood_owlkin
go_ravager_cage
npc_death_ravager
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"
#include <cmath>

/*######
## npc_draenei_survivor
######*/

enum eEnums
{
    SAY_HEAL1           = -1000176,
    SAY_HEAL2           = -1000177,
    SAY_HEAL3           = -1000178,
    SAY_HEAL4           = -1000179,

    SAY_HELP1           = -1000180,
    SAY_HELP2           = -1000181,
    SAY_HELP3           = -1000182,
    SAY_HELP4           = -1000183,

    SPELL_IRRIDATION    = 35046,
    SPELL_STUNNED       = 28630
};

class npc_draenei_survivor : public CreatureScript
{
public:
    npc_draenei_survivor() : CreatureScript("npc_draenei_survivor") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_draenei_survivorAI (pCreature);
    }

    struct npc_draenei_survivorAI : public ScriptedAI
    {
        npc_draenei_survivorAI(Creature *c) : ScriptedAI(c) {}

        uint64 pCaster;

        uint32 SayThanksTimer;
        uint32 RunAwayTimer;
        uint32 SayHelpTimer;

        bool CanSayHelp;

        void Reset()
        {
            pCaster = 0;

            SayThanksTimer = 0;
            RunAwayTimer = 0;
            SayHelpTimer = 10000;

            CanSayHelp = true;

            DoCast(me, SPELL_IRRIDATION, true);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            me->SetHealth(me->CountPctFromMaxHealth(10));
            me->SetStandState(UNIT_STAND_STATE_SLEEP);
        }

        void EnterCombat(Unit * /*who*/) {}

        void MoveInLineOfSight(Unit *who)
        {
            if (CanSayHelp && who->GetTypeId() == TYPEID_PLAYER && me->IsFriendlyTo(who) && me->IsWithinDistInMap(who, 25.0f))
            {
                //Random switch between 4 texts
                DoScriptText(RAND(SAY_HELP1, SAY_HELP2, SAY_HELP3, SAY_HELP4), me, who);

                SayHelpTimer = 20000;
                CanSayHelp = false;
            }
        }

        void SpellHit(Unit *Caster, const SpellEntry *Spell)
        {
            if (Spell->SpellFamilyFlags[2] & 0x080000000)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                me->SetStandState(UNIT_STAND_STATE_STAND);

                DoCast(me, SPELL_STUNNED, true);

                pCaster = Caster->GetGUID();

                SayThanksTimer = 5000;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (SayThanksTimer)
            {
                if (SayThanksTimer <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_IRRIDATION);

                    if (Player* pPlayer = Unit::GetPlayer(*me, pCaster))
                    {
                        DoScriptText(RAND(SAY_HEAL1, SAY_HEAL2, SAY_HEAL3, SAY_HEAL4), me, pPlayer);

                        pPlayer->TalkedToCreature(me->GetEntry(),me->GetGUID());
                    }

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, -4115.053711f, -13754.831055f, 73.508949f);

                    RunAwayTimer = 10000;
                    SayThanksTimer = 0;
                } else SayThanksTimer -= diff;

                return;
            }

            if (RunAwayTimer)
            {
                if (RunAwayTimer <= diff)
                    me->ForcedDespawn();
                else
                    RunAwayTimer -= diff;

                return;
            }

            if (SayHelpTimer <= diff)
            {
                CanSayHelp = true;
                SayHelpTimer = 20000;
            } else SayHelpTimer -= diff;
        }
    };

};

/*######
## npc_engineer_spark_overgrind
######*/

enum eOvergrind
{
    SAY_TEXT        = -1000184,
    SAY_EMOTE       = -1000185,
    ATTACK_YELL     = -1000186,

    AREA_COVE       = 3579,
    AREA_ISLE       = 3639,
    QUEST_GNOMERCY  = 9537,
    FACTION_HOSTILE = 14,
    SPELL_DYNAMITE  = 7978
};

#define GOSSIP_FIGHT "Traitor! You will be brought to justice!"

class npc_engineer_spark_overgrind : public CreatureScript
{
public:
    npc_engineer_spark_overgrind() : CreatureScript("npc_engineer_spark_overgrind") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF)
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->setFaction(FACTION_HOSTILE);
            CAST_AI(npc_engineer_spark_overgrind::npc_engineer_spark_overgrindAI, pCreature->AI())->AttackStart(pPlayer);
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(QUEST_GNOMERCY) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_engineer_spark_overgrindAI (pCreature);
    }

    struct npc_engineer_spark_overgrindAI : public ScriptedAI
    {
        npc_engineer_spark_overgrindAI(Creature *c) : ScriptedAI(c)
        {
            NormFaction = c->getFaction();
            NpcFlags = c->GetUInt32Value(UNIT_NPC_FLAGS);

            if (c->GetAreaId() == AREA_COVE || c->GetAreaId() == AREA_ISLE)
                IsTreeEvent = true;
        }

        uint32 NormFaction;
        uint32 NpcFlags;

        uint32 Dynamite_Timer;
        uint32 Emote_Timer;

        bool IsTreeEvent;

        void Reset()
        {
            Dynamite_Timer = 8000;
            Emote_Timer = 120000 + rand()%30000;

            me->setFaction(NormFaction);
            me->SetUInt32Value(UNIT_NPC_FLAGS, NpcFlags);

            IsTreeEvent = false;
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(ATTACK_YELL, me, who);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!me->isInCombat() && !IsTreeEvent)
            {
                if (Emote_Timer <= diff)
                {
                    DoScriptText(SAY_TEXT, me);
                    DoScriptText(SAY_EMOTE, me);
                    Emote_Timer = 120000 + rand()%30000;
                } else Emote_Timer -= diff;
            }
            else if (IsTreeEvent)
                return;

            if (!UpdateVictim())
                return;

            if (Dynamite_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_DYNAMITE);
                Dynamite_Timer = 8000;
            } else Dynamite_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};




/*######
## npc_injured_draenei
######*/

class npc_injured_draenei : public CreatureScript
{
public:
    npc_injured_draenei() : CreatureScript("npc_injured_draenei") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_injured_draeneiAI (pCreature);
    }

    struct npc_injured_draeneiAI : public ScriptedAI
    {
        npc_injured_draeneiAI(Creature *c) : ScriptedAI(c) {}

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            me->SetHealth(me->CountPctFromMaxHealth(15));
            switch (rand()%2)
            {
                case 0: me->SetStandState(UNIT_STAND_STATE_SIT); break;
                case 1: me->SetStandState(UNIT_STAND_STATE_SLEEP); break;
            }
        }

        void EnterCombat(Unit * /*who*/) {}

        void MoveInLineOfSight(Unit * /*who*/)
        {
        }

        void UpdateAI(const uint32 /*diff*/)
        {
        }

    };

};

/*######
## npc_magwin
######*/

enum eMagwin
{
    SAY_START                   = -1000111,
    SAY_AGGRO                   = -1000112,
    SAY_PROGRESS                = -1000113,
    SAY_END1                    = -1000114,
    SAY_END2                    = -1000115,
    EMOTE_HUG                   = -1000116,

    QUEST_A_CRY_FOR_SAY_HELP    = 9528
};

class npc_magwin : public CreatureScript
{
public:
    npc_magwin() : CreatureScript("npc_magwin") { }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_A_CRY_FOR_SAY_HELP)
        {
            pCreature->setFaction(113);
            if (npc_escortAI* pEscortAI = CAST_AI(npc_escortAI, pCreature->AI()))
                pEscortAI->Start(true, false, pPlayer->GetGUID());
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_magwinAI(pCreature);
    }

    struct npc_magwinAI : public npc_escortAI
    {
        npc_magwinAI(Creature *c) : npc_escortAI(c) {}

        void WaypointReached(uint32 i)
        {
            Player* pPlayer = GetPlayerForEscort();

            if (!pPlayer)
                return;

            switch(i)
            {
            case 0:
                DoScriptText(SAY_START, me, pPlayer);
                break;
            case 17:
                DoScriptText(SAY_PROGRESS, me, pPlayer);
                break;
            case 28:
                DoScriptText(SAY_END1, me, pPlayer);
                break;
            case 29:
                DoScriptText(EMOTE_HUG, me, pPlayer);
                DoScriptText(SAY_END2, me, pPlayer);
                pPlayer->GroupEventHappens(QUEST_A_CRY_FOR_SAY_HELP,me);
                break;
            }
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(SAY_AGGRO, me, who);
        }

        void Reset() { }
    };

};


/*######
## npc_geezle
######*/

enum eGeezle
{
    QUEST_TREES_COMPANY = 9531,

    SPELL_TREE_DISGUISE = 30298,

    GEEZLE_SAY_1    = -1000629,
    SPARK_SAY_2     = -1000630,
    SPARK_SAY_3     = -1000631,
    GEEZLE_SAY_4    = -1000632,
    SPARK_SAY_5     = -1000633,
    SPARK_SAY_6     = -1000634,
    GEEZLE_SAY_7    = -1000635,

    EMOTE_SPARK     = -1000636,

    MOB_SPARK       = 17243,
    GO_NAGA_FLAG    = 181694
};

static float SparkPos[3] = {-5029.91f, -11291.79f, 8.096f};

class npc_geezle : public CreatureScript
{
public:
    npc_geezle() : CreatureScript("npc_geezle") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_geezleAI(pCreature);
    }

    struct npc_geezleAI : public ScriptedAI
    {
        npc_geezleAI(Creature *c) : ScriptedAI(c) {}

        uint64 SparkGUID;

        uint32 Step;
        uint32 SayTimer;

        bool EventStarted;

        void Reset()
        {
            SparkGUID = 0;
            Step = 0;
            StartEvent();
        }

        void EnterCombat(Unit* /*who*/){}

        void StartEvent()
        {
            Step = 0;
            EventStarted = true;
            Creature* Spark = me->SummonCreature(MOB_SPARK, SparkPos[0], SparkPos[1], SparkPos[2], 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 1000);
            if (Spark)
            {
                SparkGUID = Spark->GetGUID();
                Spark->setActive(true);
                Spark->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
            SayTimer = 8000;
        }

        uint32 NextStep(uint32 Step)
        {
            Creature* Spark = Unit::GetCreature(*me, SparkGUID);

            switch(Step)
            {
            case 0:
                if (Spark)
                    Spark->GetMotionMaster()->MovePoint(0, -5080.70f, -11253.61f, 0.56f);
                me->GetMotionMaster()->MovePoint(0, -5092.26f, -11252, 0.71f);
                return 9000; // NPCs are walking up to fire
            case 1:
                DespawnNagaFlag(true);
                DoScriptText(EMOTE_SPARK, Spark);
                return 1000;
            case 2:
                DoScriptText(GEEZLE_SAY_1, me, Spark);
                if (Spark)
                {
                    Spark->SetInFront(me);
                    me->SetInFront(Spark);
                }
                return 5000;
            case 3: DoScriptText(SPARK_SAY_2, Spark); return 7000;
            case 4: DoScriptText(SPARK_SAY_3, Spark); return 8000;
            case 5: DoScriptText(GEEZLE_SAY_4, me, Spark); return 8000;
            case 6: DoScriptText(SPARK_SAY_5, Spark); return 9000;
            case 7: DoScriptText(SPARK_SAY_6, Spark); return 8000;
            case 8: DoScriptText(GEEZLE_SAY_7, me, Spark); return 2000;
            case 9:
                me->GetMotionMaster()->MoveTargetedHome();
                if (Spark)
                    Spark->GetMotionMaster()->MovePoint(0, SparkPos[0], SparkPos[1], SparkPos[2]);
                CompleteQuest();
                return 9000;
            case 10:
                if (Spark)
                    Spark->DisappearAndDie();
                DespawnNagaFlag(false);
                me->DisappearAndDie();
            default: return 99999999;
            }
        }

        // will complete Tree's company quest for all nearby players that are disguised as trees
        void CompleteQuest()
        {
            float radius = 50.0f;
            std::list<Player*> players;
            Strawberry::AnyPlayerInObjectRangeCheck checker(me, radius);
            Strawberry::PlayerListSearcher<Strawberry::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
            me->VisitNearbyWorldObject(radius, searcher);

            for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if ((*itr)->GetQuestStatus(QUEST_TREES_COMPANY) == QUEST_STATUS_INCOMPLETE
                    &&(*itr)->HasAura(SPELL_TREE_DISGUISE))
                {
                    (*itr)->KilledMonsterCredit(MOB_SPARK,0);
                }
            }
        }

        void DespawnNagaFlag(bool despawn)
        {
            std::list<GameObject*> FlagList;
            me->GetGameObjectListWithEntryInGrid(FlagList,GO_NAGA_FLAG, 100.0f);

            if (!FlagList.empty())
            {
                for (std::list<GameObject*>::const_iterator itr = FlagList.begin(); itr != FlagList.end(); ++itr)
                {
                    if (despawn)
                    {
                        (*itr)->SetLootState(GO_JUST_DEACTIVATED);
                    }
                    else
                        (*itr)->Respawn();
                }
            } else sLog->outError("SD2 ERROR: FlagList is empty!");
        }

        void UpdateAI(const uint32 diff)
        {
            if (SayTimer <= diff)
            {
                if (EventStarted)
                {
                    SayTimer = NextStep(Step++);
                }
            } else SayTimer -= diff;
        }
    };

};

/*######
## mob_nestlewood_owlkin
######*/

enum eOwlkin
{
    SPELL_INOCULATE_OWLKIN  = 29528,
    ENTRY_OWLKIN            = 16518,
    ENTRY_OWLKIN_INOC       = 16534
};

class npc_nestlewood_owlkin : public CreatureScript
{
public:
    npc_nestlewood_owlkin() : CreatureScript("npc_nestlewood_owlkin") { }

    struct npc_nestlewood_owlkinAI : public ScriptedAI
    {
        npc_nestlewood_owlkinAI(Creature *c) : ScriptedAI(c) {}

        uint32 DespawnTimer;

        void Reset()
        {
            DespawnTimer = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            //timer gets adjusted by the triggered aura effect
            if (DespawnTimer)
            {
                if (DespawnTimer <= diff)
                {
                    //once we are able to, despawn us
                    me->ForcedDespawn();
                    return;
                } else DespawnTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

    bool EffectDummyCreature(Unit * /*pCaster*/, uint32 spellId, uint32 effIndex, Creature *pCreatureTarget)
    {
        //always check spellid and effectindex
        if (spellId == SPELL_INOCULATE_OWLKIN && effIndex == 0)
        {
            if (pCreatureTarget->GetEntry() != ENTRY_OWLKIN)
                return true;

            pCreatureTarget->UpdateEntry(ENTRY_OWLKIN_INOC);

            //set despawn timer, since we want to remove Creature after a short time
            CAST_AI(npc_nestlewood_owlkin::npc_nestlewood_owlkinAI, pCreatureTarget->AI())->DespawnTimer = 15000;

            //always return true when we are handling this spell and effect
            return true;
        }
        return false;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_nestlewood_owlkinAI (pCreature);
    }

};


enum eRavegerCage
{
    NPC_DEATH_RAVAGER       = 17556,

    SPELL_REND              = 13443,
    SPELL_ENRAGING_BITE     = 30736,

    QUEST_STRENGTH_ONE      = 9582
};

class go_ravager_cage : public GameObjectScript
{
public:
    go_ravager_cage() : GameObjectScript("go_ravager_cage") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        if (pPlayer->GetQuestStatus(QUEST_STRENGTH_ONE) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* ravager = pGo->FindNearestCreature(NPC_DEATH_RAVAGER, 5.0f, true))
            {
                ravager->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                ravager->SetReactState(REACT_AGGRESSIVE);
                ravager->AI()->AttackStart(pPlayer);
            }
        }
        return true ;
    }
};

class npc_death_ravager : public CreatureScript
{
public:
    npc_death_ravager() : CreatureScript("npc_death_ravager") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_death_ravagerAI(pCreature);
    }

    struct npc_death_ravagerAI : public ScriptedAI
    {
        npc_death_ravagerAI(Creature *c) : ScriptedAI(c){}

        uint32 RendTimer;
        uint32 EnragingBiteTimer;

        void Reset()
        {
            RendTimer = 30000;
            EnragingBiteTimer = 20000;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (RendTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_REND);
                RendTimer = 30000;
            }
            else RendTimer -= diff;

            if (EnragingBiteTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ENRAGING_BITE);
                EnragingBiteTimer = 15000;
            }
            else EnragingBiteTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

/*########
## Quest: The Prophecy of Akida
########*/
enum BristlelimbCage
{
    QUEST_THE_PROPHECY_OF_AKIDA         = 9544,
    NPC_STILLPINE_CAPITIVE              = 17375,
    GO_BRISTELIMB_CAGE                  = 181714,
    CAPITIVE_SAY_1                      = -1000474,
    CAPITIVE_SAY_2                      = -1000475,
    CAPITIVE_SAY_3                      = -1000476
};

class npc_stillpine_capitive : public CreatureScript
{
public:
    npc_stillpine_capitive() : CreatureScript("npc_stillpine_capitive") { }

    struct npc_stillpine_capitiveAI : public ScriptedAI
    {
        npc_stillpine_capitiveAI(Creature *c) : ScriptedAI(c){}

        uint32 FleeTimer;

        void Reset()
        {
            FleeTimer = 0;
            GameObject* cage = me->FindNearestGameObject(GO_BRISTELIMB_CAGE, 5.0f);
            if(cage)
                cage->ResetDoorOrButton();
        }

        void UpdateAI(const uint32 diff)
        {
            if(FleeTimer)
            {
                if(FleeTimer <= diff)
                    me->ForcedDespawn();
                else FleeTimer -= diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_stillpine_capitiveAI(pCreature);
    }

};

class go_bristlelimb_cage : public GameObjectScript
{
public:
    go_bristlelimb_cage() : GameObjectScript("go_bristlelimb_cage") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        if(pPlayer->GetQuestStatus(QUEST_THE_PROPHECY_OF_AKIDA) == QUEST_STATUS_INCOMPLETE)
        {
            Creature* pCreature = pGo->FindNearestCreature(NPC_STILLPINE_CAPITIVE, 5.0f, true);
            if(pCreature)
            {
                DoScriptText(RAND(CAPITIVE_SAY_1, CAPITIVE_SAY_2, CAPITIVE_SAY_3), pCreature, pPlayer);
                pCreature->GetMotionMaster()->MoveFleeing(pPlayer, 3500);
                pPlayer->KilledMonsterCredit(pCreature->GetEntry(), pCreature->GetGUID());
                CAST_AI(npc_stillpine_capitive::npc_stillpine_capitiveAI, pCreature->AI())->FleeTimer = 3500;
                return false;
            }
        }
        return true;
    }

};

void AddSC_azuremyst_isle()
{
    new npc_draenei_survivor();
    new npc_engineer_spark_overgrind();
    new npc_injured_draenei();
    new npc_magwin();
    new npc_geezle();
    new npc_nestlewood_owlkin();
    new npc_death_ravager();
    new go_ravager_cage();
    new npc_stillpine_capitive();
    new go_bristlelimb_cage();
}
