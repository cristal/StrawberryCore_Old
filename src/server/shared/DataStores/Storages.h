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

#ifndef DBCSTORE_H
#define DBCSTORE_H

#include "StorageLoader.h"
#include "Logging/Log.h"
#include "Field.h"
#include "DatabaseWorkerPool.h"
#include "Implementation/WorldDatabase.h"
#include "DatabaseEnv.h"

struct SqlData
{
    const std::string * formatString;
    const std::string * indexName;
    std::string sqlTableName;
    int32 indexPos;
    int32 sqlIndexPos;
    SqlData(const std::string * _filename, const std::string * _format, const std::string * _idname, const char * fmt)
        :formatString(_format), indexName (_idname), sqlIndexPos(0)
    {
        // Convert dbc file name to sql table name
        sqlTableName = *_filename;
        for (uint32 i = 0; i< sqlTableName.size(); ++i)
        {
            if (isalpha(sqlTableName[i]))
                sqlTableName[i] = tolower(sqlTableName[i]);
            else if (sqlTableName[i] == '.')
                sqlTableName[i] = '_';
        }

        // Get sql index position
        StorageLoader::GetFormatRecordSize(fmt, &indexPos);
        if (indexPos >= 0)
        {
            uint32 uindexPos = uint32(indexPos);
            for (uint32 x = 0; x < formatString->size(); ++x)
            {
                // Count only fields present in sql
                if ((*formatString)[x] == FT_SQL_PRESENT)
                {
                    if (x == uindexPos)
                        break;
                    ++sqlIndexPos;
                }
            }
        }
    }
};

template<class T>
class DATAStorage
{
    typedef std::list<char*> StringPoolList;
    public:
        explicit DATAStorage(const char *f) : fmt(f), nCount(0), fieldCount(0), indexTable(NULL), m_dataTable(NULL) { }
        ~DATAStorage() { Clear(); }

        T const* LookupEntry(uint32 id) const { return (id>=nCount)?NULL:indexTable[id]; }
        T* LookupEntryNoConst(uint32 id) const { return (id>=nCount)?NULL:indexTable[id]; }
        void SetEntry(uint32 id, T* entry) { if(id>=nCount){ printf("ERROR"); return;} indexTable[id]=entry; }
        void SetFormat(const char* format) { fmt = format; }
        uint32  GetNumRows() const { return nCount; }
        void SetNCount(uint32 count) { nCount = count; }
        void SetFieldCount(uint32 fieldcount) { fieldCount = fieldcount; }
        uint32 GetFieldCount() const { return fieldCount; }
        char const* GetFormat() const { return fmt; }
        void SetIndexTable(uint32 row, T* table) { indexTable[row] = table; }
        void SetIndexTable(T** table) { indexTable = table; }

        // This can be merged in one function.
        bool LoadDB2Storage(char const* fn, SqlData* sql)
        {
            StorageLoader db2;
            // Check if load was successful, only then continue
            if (!db2.LoadDB2Storage(fn, fmt))
                return false;

            uint32 sqlRecordCount = 0;
            uint32 sqlHighestIndex = 0;
            char* sqlDataTable = 0;

            fieldCount = db2.GetCols();

            // load raw non-string data
            m_dataTable = (T*)db2.AutoProduceData(fmt, nCount, (char**&)indexTable, sqlRecordCount, sqlHighestIndex, sqlDataTable);

            // create string holders for loaded string fields
            m_stringPoolList.push_back(db2.AutoProduceStringsArrayHolders(fmt,(char*)m_dataTable));

            // load strings from db2 data
            m_stringPoolList.push_back(db2.AutoProduceStrings(fmt,(char*)m_dataTable));

            // error in db2 file at loading if NULL
            return indexTable!=NULL;
        }

        bool LoadDBCStorage(char const* fn, SqlData* sql)
        {
            StorageLoader dbc;
            // Check if load was successful, only then continue
            if (!dbc.LoadDBCStorage(fn, fmt))
                return false;

            uint32 sqlRecordCount = 0;
            uint32 sqlHighestIndex = 0;
            Field* fields = NULL;
            QueryResult result = QueryResult(NULL);
            // Load data from sql
            if (sql)
            {
                std::string query = "SELECT * FROM " + sql->sqlTableName;
                if (sql->indexPos >= 0)
                    query +=" ORDER BY " + *sql->indexName + " DESC";
                query += ";";

                result = WorldDatabase.Query(query.c_str());
                if (result)
                {
                    sqlRecordCount = uint32(result->GetRowCount());
                    if (sql->indexPos >= 0)
                    {
                        fields = result->Fetch();
                        sqlHighestIndex = fields[sql->sqlIndexPos].GetUInt32();
                    }
                    // Check if sql index pos is valid
                    if (int32(result->GetFieldCount()-1) < sql->sqlIndexPos)
                    {
                        sLog->outError("Invalid index pos for dbc:'%s'", sql->sqlTableName.c_str());
                        return false;
                    }
                }
            }
            char* sqlDataTable;
            fieldCount = dbc.GetCols();

            // load raw non-string data
            m_dataTable = (T*)dbc.AutoProduceData(fmt, nCount, (char**&)indexTable, sqlRecordCount, sqlHighestIndex, sqlDataTable);

            // create string holders for loaded string fields
            m_stringPoolList.push_back(dbc.AutoProduceStringsArrayHolders(fmt,(char*)m_dataTable));

            // load strings from dbc data
            m_stringPoolList.push_back(dbc.AutoProduceStrings(fmt,(char*)m_dataTable));

            // Insert sql data into arrays
            if (result)
            {
                if (indexTable)
                {
                    uint32 offset = 0;
                    uint32 rowIndex = dbc.GetNumRows();
                    do
                    {
                        if (!fields)
                            fields = result->Fetch();

                        if(sql->indexPos >= 0)
                        {
                            uint32 id = fields[sql->sqlIndexPos].GetUInt32();
                            if (indexTable[id])
                            {
                                sLog->outError("Index %d already exists in dbc:'%s'", id, sql->sqlTableName.c_str());
                                return false;
                            }
                            indexTable[id]=(T*)&sqlDataTable[offset];
                        }
                        else
                            indexTable[rowIndex]=(T*)&sqlDataTable[offset];
                        uint32 columnNumber = 0;
                        uint32 sqlColumnNumber = 0;

                        for (; columnNumber < sql->formatString->size(); ++columnNumber)
                        {
                            if ((*sql->formatString)[columnNumber] == FT_SQL_ABSENT)
                            {
                                switch(fmt[columnNumber])
                                {
                                    case FT_FLOAT:
                                        *((float*)(&sqlDataTable[offset]))= 0.0f;
                                        offset+=4;
                                        break;
                                    case FT_IND:
                                    case FT_INT:
                                        *((uint32*)(&sqlDataTable[offset]))=uint32(0);
                                        offset+=4;
                                        break;
                                    case FT_BYTE:
                                        *((uint8*)(&sqlDataTable[offset]))=uint8(0);
                                        offset+=1;
                                        break;
                                    case FT_STRING:
                                        // Beginning of the pool - empty string
                                        *((char**)(&sqlDataTable[offset]))=m_stringPoolList.back();
                                        offset+=sizeof(char*);
                                        break;
                                }
                            }
                            else if ((*sql->formatString)[columnNumber] == FT_SQL_PRESENT)
                            {
                                bool validSqlColumn = true;
                                switch(fmt[columnNumber])
                                {
                                    case FT_FLOAT:
                                        *((float*)(&sqlDataTable[offset]))=fields[sqlColumnNumber].GetFloat();
                                        offset+=4;
                                        break;
                                    case FT_IND:
                                    case FT_INT:
                                        *((uint32*)(&sqlDataTable[offset]))=fields[sqlColumnNumber].GetUInt32();
                                        offset+=4;
                                        break;
                                    case FT_BYTE:
                                        *((uint8*)(&sqlDataTable[offset]))=fields[sqlColumnNumber].GetUInt8();
                                        offset+=1;
                                        break;
                                    case FT_STRING:
                                        sLog->outError("Unsupported data type in table '%s' at char %d", sql->sqlTableName.c_str(), columnNumber);
                                        return false;
                                    case FT_SORT:
                                        break;
                                    default:
                                        validSqlColumn = false;
                                }
                                if (validSqlColumn && (columnNumber != (sql->formatString->size()-1)))
                                    sqlColumnNumber++;
                            }
                            else
                            {
                                sLog->outError("Incorrect sql format string '%s' at char %d", sql->sqlTableName.c_str(), columnNumber);
                                return false;
                            }
                        }
                        if (sqlColumnNumber != (result->GetFieldCount()-1))
                        {
                            sLog->outError("SQL and DBC format strings are not matching for table: '%s'", sql->sqlTableName.c_str());
                            return false;
                        }

                        fields = NULL;
                        ++rowIndex;
                    }while (result->NextRow());
                }
            }

            // error in dbc file at loading if NULL
            return indexTable!=NULL;
        }

        bool LoadStringsFrom(char const* fn)
        {
            // DBC must be already loaded using Load
            if(!indexTable)
                return false;

            StorageLoader dbc;
            // Check if load was successful, only then continue
            if (!dbc.LoadDBCStorage(fn, fmt) || !dbc.LoadDB2Storage(fn, fmt))
                return false;

            m_stringPoolList.push_back(dbc.AutoProduceStrings(fmt, (char*)m_dataTable));

            return true;
        }

        void Clear()
        {
            if (!indexTable)
                return;

            delete[] ((char*)indexTable);
            indexTable = NULL;
            delete[] ((char*)m_dataTable);
            m_dataTable = NULL;

            while (!m_stringPoolList.empty())
            {
                delete[] m_stringPoolList.front();
                m_stringPoolList.pop_front();
            }
            nCount = 0;
        }

        void EraseEntry(uint32 id) { indexTable[id] = NULL; }

    public:
        char const* fmt;
        uint32 nCount;
        uint32 fieldCount;
        T** indexTable;
        T* m_dataTable;
        StringPoolList m_stringPoolList;
};

#endif
