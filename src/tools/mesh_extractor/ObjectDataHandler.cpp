/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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
 
#include "ObjectDataHandler.h"
#include "Chunk.h"
#include "ADT.h"
#include "ChunkedData.h"

void ObjectDataHandler::ProcessMapChunk( MapChunk* chunk )
{
    if (!Source->HasObjectData)
        return;
    // fuck it blizzard, why is this crap necessary?
    int32 firstIndex = Source->ObjectData->GetFirstIndex("MCNK");
    if (firstIndex == -1)
        return;
    if (uint32(firstIndex + chunk->Index) > Source->ObjectData->Chunks.size())
        return;
    Chunk* ourChunk = Source->ObjectData->Chunks[firstIndex + chunk->Index];
    if (ourChunk->Length == 0)
        return;
    ChunkedData* subChunks = new ChunkedData(ourChunk->GetStream(), ourChunk->Length, 2);
    ProcessInternal(subChunks);
}
