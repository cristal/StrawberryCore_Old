/*
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://www.strawberry-pr0jcts.com/>
 *
 * Copyright (C) 2011 TrilliumEMU <http://www.trilliumemu.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "bastion_of_twilight.h"
#include "SpellScript.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "ScriptPCH.h"

#define MAX_DAZZLIN_DESTRUCTION 6

enum ePhases
{
    PHASE_GROUND = 1,
    PHASE_TRANSITION = 2,
    PHASE_AIR = 3,
};

class boss_theralion : public CreatureScript
{
public:
    boss_theralion() : CreatureScript("boss_theralion") {}

    struct boss_theralionAI : public BossAI
    {
        boss_theralionAI(Creature * pCreature) : BossAI(pCreature,DATA_THERALION), summons(me)
        {
            pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        void Reset()
        {
            uiPhase = 0;
            uiPhaseTimer = 900000;
            uiEngulfingMagicCount = 0;
            uiEngulfingMagicTimer = 180000;
            uiFabulousFlamesTimer = 160000;
            uiTwilightBlastTimer = 3000;
            uiDazzlingDestructionTimer = 3000;
            uiDazzlingDestructionDelay = 4000;
        }

        void SetData(uint32 id,uint32 value)
        {
            switch (id)
            {
            case DATA_PHASE:
                uiPhase = value;
            }
        }

        uint32 GetData(uint32 id)
        {
            switch (id)
            {
            case DATA_PHASE:
                return uiPhase;
            case DATA_ENGULFING_COUNT:
                return uiEngulfingMagicCount;
            }
            return -1;
        }

        void JustSummoned(Creature * pSummoned)
        {
            switch(pSummoned->GetEntry())
            {
            case NPC_DAZZLING_DESTRUCTION:
                uiDazzlingDestructionVector.push_back(pSummoned->GetGUID());
                DoCast(pSummoned,SPELL_DESTRUCTION_VISUAL);
                break;
            case NPC_THERALION_FLIGHT_TARGET_STALKER:
                DoCast(pSummoned,SPELL_TWILIGHT_BLAST);
                break;
            }
        }

        Creature * GetValiona()
        {
            return me->GetCreature(*me,pInstance->GetData64(DATA_VALIONA));
        }

        void AttackStart()
        {
            uiPhase = PHASE_GROUND;
            GetValiona()->AI()->DoAction(ACTION_VALIONA_TAKEOFF);
        }

        void DoAction(const uint32 action)
        {
            switch (action)
            {
            case ACTION_THERALION_TAKEOFF:
                me->GetMotionMaster()->MoveTakeoff(POINT_THERALION_TAKEOFF,Positions[POINT_THERALION_TAKEOFF],1.0f);
                break;
            case ACTION_THERALION_LAND:
                me->GetMotionMaster()->MoveLand(POINT_THERALION_LAND,Positions[POINT_THERALION_LAND],1.0f);
                break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                switch (id)
                {
                case POINT_THERALION_TAKEOFF:
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveIdle();
                case POINT_THERALION_LAND:
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveIdle();
                }
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            switch (uiPhase)
            {
            case PHASE_GROUND:
                if (uiEngulfingMagicTimer <= uiDiff && uiEngulfingMagicTimer <=2)
                {
                    uiEngulfingMagicTimer = 180000;
                    uiEngulfingMagicCount++;
                    DoCast(SelectTarget(SELECT_TARGET_RANDOM),SPELL_ENGULFING_MAGIC);
                } else uiEngulfingMagicTimer -= uiDiff;
                if (uiFabulousFlamesTimer <= uiDiff)
                {
                    DoCast(SPELL_FABILOUS_FLAMES);
                } else uiFabulousFlamesTimer -= uiDiff;
                DoMeleeAttackIfReady();
            case PHASE_TRANSITION:
                if (uiDazzlingDestructionTimer <= uiDiff && uiDazzlingDestructionCount <= MAX_DAZZLIN_DESTRUCTION)
                {
                    for(uint8 i = 0; i < 2; i++)
                    {
                        Unit * Target = SelectTarget(SELECT_TARGET_RANDOM);
                        me->SummonCreature(NPC_DAZZLING_DESTRUCTION,Target->GetPositionX(),Target->GetPositionY(),Target->GetPositionZ());
                    }
                    uiDazzlingDestructionCount += 2;
                    uiDazzlingDestructionTimer = 2000;
                } else uiDazzlingDestructionTimer -= uiDiff;
                if(uiDazzlingDestructionDelay <= uiDiff)
                {
                    for(uint8 i = 0; i <= 2; ++i)
                    {
                        me->CastSpell(ObjectAccessor::GetUnit(*me,uiDazzlingDestructionVector.back()),SPELL_DESTRUCTION_MISSILE,false);
                        uiDazzlingDestructionVector.pop_back();
                    }
                    uiDazzlingDestructionDelay = 4000;
                } else uiDazzlingDestructionDelay -= uiDiff;
                if (uiDazzlingDestructionVector.size() == 0)
                {
                    uiPhase--;
                    GetValiona()->AI()->DoAction(ACTION_VALIONA_TAKEOFF);
                    DoAction(ACTION_THERALION_LAND);
                }
            case PHASE_AIR:
                if (uiTwilightBlastTimer <= uiDiff)
                {
                    uiTwilightBlastTimer = 3000;
                    me->SummonCreature(NPC_THERALION_FLIGHT_TARGET_STALKER,me->getVictim()->GetPositionX(),me->getVictim()->GetPositionY(),me->getVictim()->GetPositionZ(),0.0f,TEMPSUMMON_MANUAL_DESPAWN);
                } else uiTwilightBlastTimer -= uiDiff;
            }

            if (uiPhaseTimer <= uiDiff && uiPhase != PHASE_TRANSITION)
            {
                uiPhaseTimer = 900000;
                switch (uiPhase)
                {
                case PHASE_GROUND:
                    uiPhase += 2;
                    DoAction(ACTION_THERALION_TAKEOFF);
                case PHASE_AIR:
                    uiPhase--;
                }
            } else uiPhaseTimer -= uiDiff;
        }
    private:
        InstanceScript* pInstance;
        SummonList summons;

        uint8  uiEngulfingMagicCount;
        uint8  uiDazzlingDestructionCount;
        uint32 uiBerserkTimer;
        uint32 uiPhase;
        uint32 uiPhaseTimer;
        uint32 uiEngulfingMagicTimer;
        uint32 uiFabulousFlamesTimer;
        uint32 uiTwilightBlastTimer;
        uint32 uiDazzlingDestructionTimer;
        uint32 uiDazzlingDestructionDelay;
        std::vector<uint64> uiDazzlingDestructionVector;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_theralionAI(pCreature);
    }


};
class boss_valiona : public CreatureScript
{
public:
    boss_valiona() : CreatureScript("boss_valiona") { }

    struct boss_valionaAI : public BossAI
    {
        boss_valionaAI(Creature * pCreature) : BossAI(pCreature,DATA_VALIONA)
        {
            pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        void Reset()
        {
            uiTheralionPhase = 0;
            uiBlackoutCount = 0;
            uiBlackoutTimer = 122000;
            uiDevouringFlamesTimer = 132000;
            uiTwilightMeteoriteTimer = 40000;
        }

        Creature * GetTheralion()
        {
            return me->GetCreature(*me,pInstance->GetData64(DATA_THERALION));
        }

        void AttackStart()
        {
            GetTheralion()->AI()->DoAction(ACTION_THERALION_TAKEOFF);
            GetTheralion()->AI()->SetData(DATA_PHASE,PHASE_AIR);
        }

        void DoAction(const uint32 action)
        {
            switch (action)
            {
            case ACTION_VALIONA_TAKEOFF:
                me->GetMotionMaster()->MoveTakeoff(POINT_VALIONA_TAKEOFF,Positions[POINT_VALIONA_TAKEOFF],1.0f);
                break;
            case ACTION_VALIONA_LAND:
                me->GetMotionMaster()->MoveLand(POINT_VALIONA_LAND,Positions[POINT_VALIONA_LAND],1.0f);
                break;
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == POINT_MOTION_TYPE)
            {
                switch (id)
                {
                case POINT_VALIONA_TAKEOFF:
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveIdle();
                }
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            uiTheralionPhase = GetTheralion()->AI()->GetData(DATA_PHASE);
            switch (uiTheralionPhase)
            {
            case PHASE_AIR:
                if (!UpdateVictim())
                    return;

                if (uiBlackoutTimer <= uiDiff && uiBlackoutCount <= 2)
                {
                    uiBlackoutTimer = 122000;
                    uiBlackoutCount++;
                    DoCastAOE(SPELL_BLACKOUT);
                } else uiBlackoutTimer -= uiDiff;
                if (uiDevouringFlamesTimer <= uiDiff)
                {
                    uiDevouringFlamesTimer = 132000;
                    DoCastAOE(SPELL_DEVOURING_FLAMES);
                } else uiDevouringFlamesTimer -= uiDiff;
                DoMeleeAttackIfReady();
            case PHASE_GROUND:
                if (GetTheralion()->AI()->GetData(DATA_ENGULFING_COUNT) == 2)
                {
                    DoCast(SPELL_DEEP_BREATH);
                }
                if (uiTwilightMeteoriteTimer <= uiDiff)
                {
                    uiTwilightMeteoriteTimer = 40000;
                    Unit * Target = SelectTarget(SELECT_TARGET_RANDOM, 500);
                    DoCast(Target,SPELL_TWILIGHT_METEORITE);
                } else uiTwilightMeteoriteTimer -= uiDiff;
            }
        }

    private:
        InstanceScript* pInstance;
        uint8  uiBlackoutCount;
        uint32 uiTheralionPhase;
        uint32 uiBlackoutTimer;
        uint32 uiDevouringFlamesTimer;
        uint32 uiTwilightMeteoriteTimer;
    };

    CreatureAI * GetAI(Creature * pCreature) const
    {
        return new boss_valionaAI(pCreature);
    }

};
class spell_dazzling_destruction : public SpellScriptLoader
{
public:
    spell_dazzling_destruction() : SpellScriptLoader("spell_dazzling_destruction") { }

    class spell_dazzling_destructionSpellScript : public SpellScript
    {
        int32 spell_trigger;
        PrepareSpellScript(spell_dazzling_destructionSpellScript);
        bool Validate(SpellEntry const * spellEntry)
        {
            if (!sSpellStore.LookupEntry(spellEntry->Id))
                return false;
            return true;
        }

        bool Load()
        {
            spell_trigger = GetSpellInfo()->EffectBasePoints[EFFECT_0];
            return true;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->CastSpell(GetTargetUnit(),spell_trigger,false);
        }

        void HandleOnHit()
        {
            std::list<Unit*> players;
            std::list<Unit*>::const_iterator itr;
            GetTargetUnit()->GetRaidMember(players,5.0f);
            for (itr=players.begin(); itr!=players.end(); ++itr)
            {
                if ((*itr)->GetTypeId() == TYPEID_PLAYER)
                {
                    GetCaster()->CastSpell((*itr),SPELL_DESTRUCTION_PROCS,true);
                }
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_dazzling_destructionSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
            OnHit += SpellHitFn(spell_dazzling_destructionSpellScript::HandleOnHit);
        }
    };

    SpellScript * GetSpellScript() const
    {
        return new spell_dazzling_destructionSpellScript();
    }
};

class spell_theralion_dazzling_destruction_triggered : public SpellScriptLoader
{
    spell_theralion_dazzling_destruction_triggered() : SpellScriptLoader("spell_theralion_dazzling_destruction_triggered") { }

    class spell_theralion_dazzling_destruction_triggeredSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_theralion_dazzling_destruction_triggeredSpellScript);
        uint32 trigger_spell[2];

        bool Load()
        {
            for(uint8 i = 0; i < 2; i++)
            {
                trigger_spell[i] = GetSpellInfo()->EffectBasePoints[i+1];
            }
            return true;
        }

        void HandleShift()
        {
            GetCaster()->CastSpell(GetHitUnit(),trigger_spell[0],true);
        }

        void HandleProtBuff()
        {
            GetCaster()->CastSpell(GetHitUnit(),trigger_spell[1],true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_theralion_dazzling_destruction_triggeredSpellScript::HandleShift);
            AfterHit += SpellHitFn(spell_theralion_dazzling_destruction_triggeredSpellScript::HandleProtBuff);
        }
    };

    SpellScript * GetSpellScript() const
    {
        return new spell_theralion_dazzling_destruction_triggeredSpellScript();
    }
};

/*class spell_valiona_blackout : public SpellScriptLoader
{
public:
    spell_valiona_blackout() : SpellScriptLoader("spell_valiona_blackout") { }

    class spell_valiona_blackoutSpellScript : public SpellScript
    {
        int32 spell_trigger[2];
        PrepareSpellScript(spell_valiona_blackoutSpellScript);

        void HandleDummy()
        {
            switch(GetCaster()->GetMap()->GetDifficulty())
            {
            case RAID_DIFFICULTY_10MAN_NORMAL:
                spell_trigger[0] = 87688;
                spell_trigger[1] = 86825;
                break;
            case RAID_DIFFICULTY_25MAN_NORMAL:
                spell_trigger[0] = 92876;
                spell_trigger[1] = 92879;
                break;
            case RAID_DIFFICULTY_10MAN_HEROIC:
                spell_trigger[0] = 92877;
                spell_trigger[1] = 92880;
                break;
            case RAID_DIFFICULTY_25MAN_HEROIC:
                spell_trigger[0] = 92878;
                spell_trigger[1] = 92881;
                break;
            }
            GetCaster()->CastSpell(GetHitUnit(),spell_trigger[0],true);
        }

        void HandleExprireOrRemove()
        {
            std::list<Unit*> Players;
            std::list<Unit*>::iterator itr;
            GetHitUnit()->GetPartyMemberInDist(Players,8.0f);
            uint8 players_close = Players.size();
            uint32 damage;
            for(itr = Players.begin(); itr != Players.end(); ++itr)
            {
                damage = 4;
                GetCaster()->DealDamage(*itr,damage);
            }
        }

        void Register()
        {

        }
    };

    SpellScript * GetSpellScript() const
    {
        return new spell_valiona_blackoutSpellScript();
    }
};*/

void AddSC_boss_theralion_and_valiona()
{
    new boss_theralion();
    new boss_valiona();
    new spell_dazzling_destruction();
}
