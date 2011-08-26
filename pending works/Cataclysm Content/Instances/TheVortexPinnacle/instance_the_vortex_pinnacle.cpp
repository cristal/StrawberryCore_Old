#include"ScriptPCH.h"
#include"the_vortex_pinnacle.h"

class instance_the_vortex_pinnacle: public InstanceMapScript
{
public:
    instance_the_vortex_pinnacle() : InstanceMapScript("instance_the_vortex_pinnacle", 657) { }
    
    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_the_vortex_pinnacle_InstanceMapScript(map);
    }

    struct instance_the_vortex_pinnacle_InstanceMapScript: public InstanceScript
    {
        instance_the_vortex_pinnacle_InstanceMapScript(Map *map) : InstanceScript(map) { }

        uint32 uiEncounter[ENCOUNTERS];

        uint32 uiGrandVizierErtan;
        uint32 uiAltairus;
        uint32 uiAsaad;

        void Initialize()
        {
            uiGrandVizierErtan = 0;
            uiAltairus = 0;
            uiAsaad = 0;

        for(uint8 i=0; i < ENCOUNTERS; ++i)
                uiEncounter[i] = NOT_STARTED;
        }

        bool IsEncounterInProgress() const
        {
            for(uint8 i=0; i < ENCOUNTERS; ++i)
            {
                if (uiEncounter[i] == IN_PROGRESS)
                    return true;
            }

            return false;
        }

        void OnCreatureCreate(Creature* pCreature, bool)
        {
            switch(pCreature->GetEntry())
            {
                case BOSS_GRAND_VIZIER_ERTAN:
                    uiGrandVizierErtan = pCreature->GetGUID();
                    break;
                case BOSS_ALTAIRUS:
                    uiAltairus = pCreature->GetGUID();
                    break;
                case BOSS_ASAAD:
                    uiAsaad = pCreature->GetGUID();
                    break;
            }
            
        }

        uint64 GetData64(uint32 identifier)
        {
            switch(identifier)
            {
                case DATA_GRAND_VIZIER_ERTAN:
                    return uiGrandVizierErtan;
                case DATA_ALTAIRUS:
                    return uiAltairus;
                case DATA_ASAAD:
                    return uiAsaad;
            }
            return 0;
        }

        void SetData(uint32 type,uint32 data)
        {
            switch(type)
            {
                case DATA_GRAND_VIZIER_ERTAN:
                    uiEncounter[0] = data;
                    break;
                case DATA_ALTAIRUS:
                    uiEncounter[1] = data;
                    break;
                case DATA_ASAAD:
                    uiEncounter[2] = data;
                    break;
            }
         
         if (data == DONE)
             SaveToDB();
        }

        uint32 GetData(uint32 type)
        {
            switch(type)
            {
                case DATA_GRAND_VIZIER_ERTAN:
                    return uiEncounter[0];
                case DATA_ALTAIRUS:
                    return uiEncounter[1];
                case DATA_ASAAD:
                    return uiEncounter[2];
            }   
            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;
            std::ostringstream saveStream;
            saveStream << "V P" << uiEncounter[0] << " " << uiEncounter[1]  << " " << uiEncounter[2]; 
            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
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
            uint16 data0, data1, data2;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2;

            if (dataHead1 == 'V' && dataHead2 == 'P')
            {
                uiEncounter[0] = data0;
                uiEncounter[1] = data1;
                uiEncounter[2] = data2;
                
                for(uint8 i=0; i < ENCOUNTERS; ++i)
                    if (uiEncounter[i] == IN_PROGRESS)
                        uiEncounter[i] = NOT_STARTED;
            }
            else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

    };
};

void AddSC_instance_the_vortex_pinnacle()
{
    new instance_the_vortex_pinnacle();
}
