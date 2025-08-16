#pragma once

typedef struct _ArenaAllocator
{
    char* memory;
    u64   size;
    u64   used;
} ArenaAllocator;

typedef struct _Memory
{
    ArenaAllocator persistentStorage;
    ArenaAllocator transientStorage;
} _Memory;

typedef struct _Memory* Memory;

extern Memory g_memory;

static inline bool DROP_CreateArena(ArenaAllocator* pArena, u64 size)
{
    char* memory = (char*) DO_ALLOC(char, size);
    if (!memory)
    {
        DO_ASSERT_MSG(false, "Failed to allocate memory for arena.");
        return false;
    }

    DO_ZERO_MEM(memory, size);

    pArena->memory = memory;
    pArena->size   = size;
    pArena->used   = 0;

    return true;
}

static inline char* DROP_Allocate(ArenaAllocator* pArena, u64 size)
{
    u64 allignedSize = (size + 15) & ~15; // Alligned to 16-bits.

    DO_ASSERT_MSG(pArena->used + allignedSize <= pArena->size, "Current memory left are less than the size provided.");

    char* memory = pArena->memory + pArena->used;
    pArena->used += allignedSize;

    return memory;
}