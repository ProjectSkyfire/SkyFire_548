/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#define _CRT_SECURE_NO_DEPRECATE

#include "loadlib.h"
#include <cstdio>

u_map_fcc MverMagic = { {'R','E','V','M'} };

ChunkedFile::ChunkedFile()
{
    data = 0;
    data_size = 0;
}

ChunkedFile::~ChunkedFile()
{
    free();
}

bool ChunkedFile::loadFile(HANDLE mpq, char* filename, bool log)
{
    free();
    HANDLE file;
    if (!SFileOpenFileEx(mpq, filename, SFILE_OPEN_FROM_MPQ, &file))
    {
        if (log)
            printf("No such file %s\n", filename);
        return false;
    }

    data_size = SFileGetFileSize(file, NULL);
    data = new uint8[data_size];
    SFileReadFile(file, data, data_size, NULL/*bytesRead*/, NULL);
    parseChunks();
    if (prepareLoadedData())
    {
        SFileCloseFile(file);
        return true;
    }

    printf("Error loading %s\n", filename);
    SFileCloseFile(file);
    free();
    return false;
}

bool ChunkedFile::prepareLoadedData()
{
    FileChunk* chunk = GetChunk("MVER");
    if (!chunk)
        return false;

    // Check version
    file_MVER* version = chunk->As<file_MVER>();
    if (version->fcc != MverMagic.fcc)
        return false;
    if (version->ver != FILE_FORMAT_VERSION)
        return false;
    return true;
}

void ChunkedFile::free()
{
    for (auto chunk : chunks)
        delete chunk.second;

    chunks.clear();

    delete[] data;
    data = 0;
    data_size = 0;
}

u_map_fcc InterestingChunks[] = {
    { 'R', 'E', 'V', 'M' },
    { 'N', 'I', 'A', 'M' },
    { 'O', '2', 'H', 'M' },
    { 'K', 'N', 'C', 'M' },
    { 'T', 'V', 'C', 'M' },
    { 'Q', 'L', 'C', 'M' }
};

bool IsInterestingChunk(u_map_fcc const& fcc)
{
    for (u_map_fcc const& f : InterestingChunks)
        if (f.fcc == fcc.fcc)
            return true;

    return false;
}

void ChunkedFile::parseChunks()
{
    uint8* ptr = GetData();
    uint8* end = GetData() + GetDataSize();
    while (ptr + 8 <= end)
    {
        u_map_fcc header = *(u_map_fcc*)ptr;
        // every chunk has a fourcc+size header regardless of whether we care about its
        // contents, so size must always be read to correctly skip past unrecognized chunks
        uint32 size = *(uint32*)(ptr + 4);
        // a corrupt/garbage size could overflow "ptr += size + 8" back to little or no
        // forward movement, looping forever; bail out once it no longer fits the buffer
        if (size > (uint32)(end - ptr - 8))
            break;

        if (IsInterestingChunk(header) && size <= data_size)
        {
            std::swap(header.fcc_txt[0], header.fcc_txt[3]);
            std::swap(header.fcc_txt[1], header.fcc_txt[2]);

            FileChunk* chunk = new FileChunk{ ptr, size };
            chunk->parseSubChunks();
            chunks.insert({ std::string(header.fcc_txt, 4), chunk });
        }

        // move to next chunk
        ptr += size + 8;
    }
}

FileChunk* ChunkedFile::GetChunk(std::string const& name)
{
    auto range = chunks.equal_range(name);
    if (std::distance(range.first, range.second) == 1)
        return range.first->second;

    return NULL;
}

FileChunk::~FileChunk()
{
    for (auto subchunk : subchunks)
        delete subchunk.second;

    subchunks.clear();
}

void FileChunk::parseSubChunks()
{
    // size is the payload after fourcc+size. Subchunks occupy [data+8, data+8+size).
    uint8* ptr = data + 8;
    uint8* end = data + 8 + size;

    // MCNK (on-disk KNCM) has a 0x80-byte header after fourcc+size. Starting at
    // data+8 treats flags/ix as a subchunk and never finds MCVT.
    if (data[0] == 'K' && data[1] == 'N' && data[2] == 'C' && data[3] == 'M')
        ptr = data + 8 + 0x80;

    while (ptr + 8 <= end)
    {
        u_map_fcc header = *(u_map_fcc*)ptr;
        // every chunk has a fourcc+size header regardless of whether we care about its
        // contents, so subsize must always be read to correctly skip past unrecognized chunks
        uint32 subsize = *(uint32*)(ptr + 4);
        // a corrupt/garbage subsize could overflow "ptr += subsize + 8" back to little or no
        // forward movement, looping forever; bail out once it no longer fits the buffer
        if (subsize > (uint32)(end - ptr - 8))
            break;

        if (IsInterestingChunk(header) && subsize < size)
        {
            std::swap(header.fcc_txt[0], header.fcc_txt[3]);
            std::swap(header.fcc_txt[1], header.fcc_txt[2]);

            FileChunk* chunk = new FileChunk{ ptr, subsize };
            chunk->parseSubChunks();
            subchunks.insert({ std::string(header.fcc_txt, 4), chunk });
        }

        // move to next chunk
        ptr += subsize + 8;
    }
}

FileChunk* FileChunk::GetSubChunk(std::string const& name)
{
    auto range = subchunks.equal_range(name);
    if (std::distance(range.first, range.second) == 1)
        return range.first->second;

    return NULL;
}
