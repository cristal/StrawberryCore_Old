/*
 * Copyright (C) 2010-2011 Strawberry-Pr0jcts <http://www.strawberry-pr0jcts.com/>
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

#include "ScriptPCH.h"
#include "bastion_of_twilight.h"

class boss_feludius : public CreatureScript
{
public:
    boss_feludius() : CreatureScript("boss_feludius") { }

    struct boss_feludiusAI : public BossAI
    {
        boss_feludiusAI(Creature * pCreature) : BossAI(pCreature,DATA_FELUDIUS), summons(me)
        {
            pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        void Reset()
        {
            uiHydrolanceTimer = 11000;
            uiWaterBombTimer = 16000;
            uiHeartofIceTimer = 39000;
            uiGlaciateTimer = 32000;
        }
        
        void SpellHitTarget(Unit * pTarget, const SpellEntry * spell)
        {

        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (uiHydrolanceTimer <= uiDiff)
            {
                uiHydrolanceTimer = 11000;
                Unit * Victim = SelectTarget(SELECT_TARGET_RANDOM);
                DoCast(Victim,SPELL_HYDROLANCE);
            } else uiHydrolanceTimer -= uiDiff;
            if (uiWaterBombTimer <= uiDiff)
            {
                uiHydrolanceTimer = 11000;
                me->AddAura(SPELL_WATERBOLT,me);
            } else uiWaterBombTimer -= uiDiff;
            if (uiHeartofIceTimer <= uiDiff)
            {
                uiHeartofIceTimer = 39000;
                Unit * Victim = SelectTarget(SELECT_TARGET_RANDOM);
                DoCast(Victim,SPELL_HEART_OF_ICE);
            } else uiHeartofIceTimer -= uiDiff;
            if (uiGlaciateTimer <= uiDiff)
            {
                uiGlaciateTimer = 32000;
                DoCast(SPELL_GLACIATE);
            } else uiGlaciateTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* pInstance;
        SummonList summons;

        uint32 uiHydrolanceTimer;
        uint32 uiWaterBombTimer;
        uint32 uiHeartofIceTimer;
        uint32 uiGlaciateTimer;
    };

    CreatureAI * GetAI(Creature* pCreature) const
    {
        return new boss_feludiusAI(pCreature);
    }
};

class boss_ignacious : public CreatureScript
{
public:
    boss_ignacious() : CreatureScript("boss_ignacious") { }

    struct boss_ignaciousAI : public BossAI
    {
        boss_ignaciousAI(Creature * pCreature) : BossAI(pCreature,DATA_IGNACIOUS)
        {
            pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        void Reset()
        {
            uiBurningBloodTimer = 31000;
            uiFlameTorrentTimer = 10000;
            uiAegisofFlameTimer = 54000;
            uiInfernoLeapTimer = 20000;
        }

        void DoAction(uint32 action)
        {
            switch(action)
            {
                case ACTION_IGNACIOUS_JUMPS:
                {
                    Unit * Leap = ObjectAccessor::GetUnit(*me,summoned[NPC_INFERNO_LEAP]);
                    me->GetMotionMaster()->MoveJump(Leap->GetPositionX(),Leap->GetPositionY(),Leap->GetPositionZ(),1.0f,0.9f);
                }
                case ACTION_INGACIOUS_CHARGE:
                {
                    Creature * Inferno = ObjectAccessor::GetCreature(*me,summoned[NPC_INFERNO_RUSH]);
                    DoCast(Inferno,82859);
                }
                case ACTION_IGNACIOUS_SUMMON_RUSH:
                {
                    me->SummonCreature(NPC_INFERNO_RUSH,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation());
                }
                case ACTION_IGNACIOUS_SUMMON_LEAP:
                {
                    Unit * Target = SelectTarget(SELECT_TARGET_RANDOM,NULL,40.0f);
                    me->SummonCreature(NPC_INFERNO_LEAP,Target->GetPositionX(),Target->GetPositionY(),Target->GetPositionZ(),Target->GetOrientation());
                }
            }
        }

        void MovementInForm()
        {

        }

        void JustSummoned(Creature * pSummoned)
        {
            switch(pSummoned->GetEntry())
            {
                case NPC_INFERNO_RUSH:
                    summoned[NPC_INFERNO_RUSH] = pSummoned->GetGUID();
                    DoAction(ACTION_INGACIOUS_CHARGE);
                case NPC_INFERNO_LEAP:
                    summoned[NPC_INFERNO_LEAP] = pSummoned->GetGUID();
                    DoAction(ACTION_IGNACIOUS_JUMPS);
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if(HealthBelowPct(50))
            {
                //me->GetMap()->ToInstanceMap()->GetInstanceScript()->
            }

            if(uiBurningBloodTimer <= uiDiff)
            {
                uiBurningBloodTimer = 31000;
                DoCastVictim(SPELL_BURNING_BLOOD,false);
            } else uiBurningBloodTimer -= uiDiff;

            if (uiFlameTorrentTimer <= uiDiff)
            {
                uiFlameTorrentTimer = 10000;
                DoCast(SPELL_FLAME_TORRENT);
            } else uiFlameTorrentTimer -= uiDiff;

            if(uiAegisofFlameTimer <= uiDiff)
            {
                uiAegisofFlameTimer = 54000;
                DoCast(me,SPELL_AEGIS_OF_FLAMES);
            } else uiAegisofFlameTimer -= uiDiff;

            if(uiInfernoLeapTimer <= uiDiff)
            {
                DoAction(ACTION_IGNACIOUS_SUMMON_LEAP);
            } else uiInfernoLeapTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

    private:
        InstanceScript * pInstance;
        uint64 uiBurningBloodTimer;
        uint64 uiFlameTorrentTimer;
        uint64 uiAegisofFlameTimer;
        uint64 uiInfernoLeapTimer;
        std::map<uint32,uint64> summoned;
    };

    CreatureAI * GetAI(Creature * pCreature) const
    {
        return new boss_ignaciousAI(pCreature);
    }
};
class spell_ignacious_inferno_ping : public SpellScriptLoader
{
    spell_ignacious_inferno_ping() : SpellScriptLoader("spell_ignacious_inferno_ping") { }

    class spell_ignacious_inferno_pingSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ignacious_inferno_pingSpellScript);

        enum eSpells
        {
            SPELL_INFERNO_LEAP_TRIGGER = 87645,
            SPELL_INFERNO_LEAP_KNOCK = 92520,
        };

        Unit * pCaster;

        bool Validate(SpellEntry * /* spellEntry*/)
        {
            pCaster = GetCaster();
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            pCaster->GetAI()->DoAction(ACTION_IGNACIOUS_SUMMON_RUSH);
            pCaster->CastSpell(pCaster->GetPositionX(),pCaster->GetPositionY(),pCaster->GetPositionZ(),SPELL_INFERNO_LEAP_KNOCK,true);
            pCaster->CastSpell(pCaster,SPELL_INFERNO_LEAP_TRIGGER,true);
            pCaster->GetAI()->DoAction(ACTION_INGACIOUS_CHARGE);
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_ignacious_inferno_pingSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript * GetSpellScript() const
    {
        return new spell_ignacious_inferno_pingSpellScript();
    }
};
void AddSC_boss_ascendant_council()
{
    new boss_feludius();
    new boss_ignacious();
}