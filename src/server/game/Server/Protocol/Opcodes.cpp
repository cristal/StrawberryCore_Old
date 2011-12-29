/*
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

#include "gamePCH.h"
#include "Opcodes.h"
#include "WorldSession.h"

// Correspondence between opcodes and their names
OpcodeHandler opcodeTable[NUM_MSG_TYPES];
uint16 opcodesEnumToNumber[MAX_OPCODE_VALUE];

void OpcodeTableHandler::LoadOpcodesFromDB()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT OpcodeName, OpcodeValue FROM opcodes WHERE ClientBuild > 0 AND StructureBuild = 15050");
    if (!result)
        return;

    uint32 count = 0;
    do
    {
        Field *fields = result->Fetch();

        std::string OpcodeName = fields[0].GetString();
        uint16 OpcodeValue     = fields[1].GetInt16();

        OpcodeTableContainer[OpcodeName] = OpcodeValue;

        count++;
    }
    while (result->NextRow());

    sLog->outString(">> Loaded %u opcode definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

int OpcodeTableHandler::GetOpcodeTable(const char* name)
{
    if (OpcodeTableContainer[name])
        return OpcodeTableContainer[name];

    return -1;
}

static void DefineOpcode(Opcodes opcodeEnum, const char* name, SessionStatus status, PacketProcessing packetProcessing, void (WorldSession::*handler)(WorldPacket& recvPacket) )
{
    int opcode = sOpcodeTableHandler->GetOpcodeTable(name);

    if (opcode > 0)
    {
        opcodesEnumToNumber[opcodeEnum]      = opcode;
        opcodeTable[opcode].name             = name;
        opcodeTable[opcode].status           = status;
        opcodeTable[opcode].packetProcessing = packetProcessing;
        opcodeTable[opcode].handler          = handler;
        opcodeTable[opcode].opcodeEnum       = opcodeEnum;
    }
    else
        sLog->outError("SOE: No valid value for %s", name); // Should be removed later. One opcode have the value 0
}

#define OPCODE(name, status, packetProcessing, handler ) DefineOpcode(name, #name, status, packetProcessing, handler )

void InitOpcodeTable()
{
    for(uint16 i = 0; i < NUM_MSG_TYPES; ++i)
    {
        opcodeTable[i].name             = "UNKNOWN";
        opcodeTable[i].status           = STATUS_NEVER;
        opcodeTable[i].packetProcessing = PROCESS_INPLACE;
        opcodeTable[i].handler          = &WorldSession::HandleNULL;
    }

    // Authentication
    OPCODE(MSG_WOW_CONNECTION,            STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess      );
    OPCODE(SMSG_AUTH_CHALLENGE,           STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );
    OPCODE(CMSG_AUTH_SESSION,             STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleEarlyProccess      );
    OPCODE(SMSG_AUTH_RESPONSE,            STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );

    // Realmlist
    OPCODE(CMSG_REALM_SPLIT_STATE,        STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleRealmSplitOpcode   );
    OPCODE(SMSG_REALM_SPLIT_MSG,          STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );

    // Characterlist
    OPCODE(CMSG_REQUEST_CHARACTER_ENUM,   STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleCharEnumOpcode     );
    OPCODE(SMSG_CHAR_ENUM,                STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );
    OPCODE(CMSG_REQUEST_CHARACTER_CREATE, STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleCharCreateOpcode   );
    OPCODE(SMSG_CHAR_CREATE,              STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );
    OPCODE(CMSG_REQUEST_CHARACTER_DELETE, STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleCharDeleteOpcode   );
    OPCODE(SMSG_CHAR_DELETE,              STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );

    // World enter
    OPCODE(CMSG_PLAYER_LOGIN,             STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandlePlayerLoginOpcode  );
    OPCODE(CMSG_LOADING_SCREEN_NOTIFY,    STATUS_AUTHED, PROCESS_THREADUNSAFE, &WorldSession::HandleLoadingScreenNotify);

    // World
    OPCODE(SMSG_UPDATE_OBJECT,            STATUS_NEVER,  PROCESS_INPLACE,      &WorldSession::HandleServerSide         );
};
