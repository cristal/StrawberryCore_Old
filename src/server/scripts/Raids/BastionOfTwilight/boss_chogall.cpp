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

#include "ScriptPCH.h"
#include "bastion_of_twilight.h"

class boss_chogall : public CreatureScript
{
public:
    boss_chogall() : CreatureScript("boss_chogall") { }

    struct boss_chogallAI : public BossAI
    {
        boss_chogallAI(Creature * pCreature) : BossAI(pCreature,DATA_CHOGALL)
        {
            pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        void UpdateAI(const uint32 uiDiff)
        {

        }

    private:
        InstanceScript * pInstance;
    };

    CreatureAI * GetAI(Creature * pCreature) const
    {
        return new boss_chogallAI(pCreature);
    }
};