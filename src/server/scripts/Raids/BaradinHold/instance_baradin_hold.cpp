/*
 * Copyright (C) 2008-2012 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2012 Strawberry-Pr0jcts <http://www.strawberry-pr0jcts.com/>
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

#include "ScriptPCH.h"
#include "baradin_hold.h"

class instance_baradin_hold : public InstanceMapScript
{
    public:
        instance_baradin_hold() : InstanceMapScript("instance_baradin_hold", 757) { }

        struct instance_baradin_hold_InstanceMapScript : public InstanceScript
        {
            instance_baradin_hold_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);
            }
            uint64 uiArgaloth;
            uint64 uiOccuthar;
            uint32 uiEncounter[MAX_ENCOUNTER];

            void Initialize()
            {
                uiArgaloth = 0;
                uiOccuthar = 0;
            }

            bool IsEncounterInProgress() const
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (uiEncounter[i] == IN_PROGRESS) return true;

                return false;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                case CREATURE_ARGALOTH:
                    uiArgaloth = creature->GetGUID();
                    break;

                case CREATURE_OCCUTHAR:
                    uiOccuthar = creature->GetGUID();
                    break;

                default:
                    break;
                }
            }

            uint64 GetData64(uint32 identifier)
            {
                switch (identifier)
                {
                case DATA_ARGALOTH: return uiArgaloth;
                case DATA_OCCUTHAR: return uiOccuthar;
                }

                return 0;
            }
            void SetData(uint32 type,uint32 data)
            {
                switch(type)
                {
                case DATA_ARGALOTH:
                    uiEncounter[0] = data;
                    break;

                case DATA_OCCUTHAR:
                    uiEncounter[1] = data;
                    break;
                }

                if (data == DONE)
                    SaveToDB();
            }

            uint32 GetData(uint32 type)
            {
                switch(type)
                {
                case DATA_ARGALOTH:
                    return uiEncounter[0];

                case DATA_OCCUTHAR:
                    return uiEncounter[1];
                }   
                return 0;
            }
            
            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "B H " << uiEncounter[0] << uiEncounter[1];

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;
                uint16 data0,data1;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2 >> data0 >> data1;

                if (dataHead1 == 'B' && dataHead2 == 'H')
                {
                    uiEncounter[0] = data0;
                    uiEncounter[1] = data1;

                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                        if (uiEncounter[i] == IN_PROGRESS)
                            uiEncounter[i] = NOT_STARTED;

                } else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_baradin_hold_InstanceMapScript(map);
        }
};

void AddSC_instance_baradin_hold()
{
    new instance_baradin_hold();
}
