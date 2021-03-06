/*
 * Copyright (C) 2011 Thibaut GIRKA <thib@sitedethib.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <assert.h>

#include "sector.h"
#include "blocktypes.h"


//TODO
void octree_update_block_count(short idx, short delta, short size, Octree octree, short x, short y, short z)
{
    short dx, dy, dz;

    if (size == 1)
        return;

    size /= 2;

    octree[idx] += delta;

    dx = (x < size) ? 0 : 1;
    dy = (y < size) ? 0 : 1;
    dz = (z < size) ? 0 : 1;

    octree_update_block_count(OCTREE_GET_CHILD(idx, dx, dy, dz), delta, size, octree,
                              x - dx * size, y - dy * size, z - dz * size);
}


//TODO: cleanup/optimize
inline bool get_block(struct Sector *sector, short x, short z,
                      struct Sector **block_sector,
                      short *block_x, short *block_z)
{
    (*block_sector) = sector;
    *block_z = z;
    *block_x = x;

    if (x >= 0 && x < 16 && z >= 0 && z < 16)
        return true;

    if (x < 0)
    {
        assert(x >= -16);
        if ((*block_sector)->north != NULL)
        {
            *block_sector = (*block_sector)->north;
            *block_x += 16;
        }
        else return false;
    }
    else if (x > 15)
    {
        assert(x < 32);
        if ((*block_sector)->south != NULL)
        {
            *block_sector = (*block_sector)->south;
            *block_x -= 16;
        }
        else return false;
    }


    if (z < 0)
    {
        assert(z >= -16);
        if ((*block_sector)->east != NULL)
        {
            *block_sector = (*block_sector)->east;
            *block_z += 16;
        }
        else return false;
    }
    else if (z > 15)
    {
        assert(z < 32);
        if ((*block_sector)->west != NULL)
        {
            *block_sector = (*block_sector)->west;
            *block_z -= 16;
        }
        else return false;
    }

    return true;
}


// Sector-related functions:
struct Sector *sector_new(int cx, int cz)
{
    struct Sector* sector = calloc(1, sizeof(struct Sector));
    if (sector == NULL)
        return NULL;

    sector->cx = cx;
    sector->cz = cz;

    return sector;
}


void sector_set_chunk(struct Sector *sector,
                      short ox, short oy, short oz,
                      short size_x, short size_y, short size_z,
                      const unsigned char *data)
{
    // TODO: optimize
    const unsigned char *metadata = data + (size_x * size_y * size_z);
    // Block light
    const unsigned char *lighting = data + (size_x * size_y * size_z) * 3 / 2;
    // Sunlight
    const unsigned char *lighting2 = data + (size_x * size_y * size_z) * 2;
    for (unsigned short x=0; x < size_x; x++)
    {
        for (unsigned short z=0; z < size_z; z++)
        {
            memcpy(sector->blocktypes[ox + x][oz + z] + oy,
                   data + (z * size_y) + (x * size_z * size_y),
                   size_y);
            for (unsigned short y=0; y < size_y; y++)
            {
                unsigned int index = (z * size_y) + (x * size_z * size_y) + y;
                if (index & 1)
                {
                    sector->blockdata[ox + x][oz + z][oy + y] = metadata[index / 2] >> 4;
                    sector->lighting[ox + x][oz + z][oy + y] = lighting[index / 2] >> 4;
                    sector->lighting[ox + x][oz + z][oy + y] |= lighting2[index / 2] & 0xF0;
                }
                else
                {
                    sector->blockdata[ox + x][oz + z][oy + y] = metadata[index / 2] & 0x0F;
                    sector->lighting[ox + x][oz + z][oy + y] = lighting[index / 2] & 0x0F;
                    sector->lighting[ox + x][oz + z][oy + y] |= (lighting2[index / 2] & 0x0F) << 4;
                }
            }
        }
    }
}


static inline void _sector_compute_block_faces(struct Sector *sector, short x, short y, short z)
{
    sector->blockfaces[x][z][y] = 0;
    unsigned char blocktype = sector->blocktypes[x][z][y];
    struct Sector *bsector = sector;
    short bx, bz;

    if (blocktype != 0)
    {
        if (blocktypes[blocktype].flags & (BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_ALLFACES))
        {
            sector->blockfaces[x][z][y] = FACE_SOUTH | FACE_TOP | FACE_WEST | FACE_NORTH | FACE_BOTTOM | FACE_EAST;
        }
        else if (blocktypes[blocktype].flags & BLOCKTYPE_FLAG_TRANSPARENT)
        {
            if (!get_block(sector, x + 1, z, &bsector, &bx, &bz) || bsector->blocktypes[bx][bz][y] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_SOUTH;
            if (y == 128 || sector->blocktypes[x][z][y + 1] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_TOP;
            if (!get_block(sector, x, z + 1, &bsector, &bx, &bz) || bsector->blocktypes[bx][bz][y] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_WEST;
            if (!get_block(sector, x - 1, z, &bsector, &bx, &bz) || bsector->blocktypes[bx][bz][y] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_NORTH;
            if (y == 0 || sector->blocktypes[x][z][y - 1] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_BOTTOM;
            if (!get_block(sector, x, z - 1, &bsector, &bx, &bz) || bsector->blocktypes[bx][bz][y] != blocktype)
                sector->blockfaces[x][z][y] |= FACE_EAST;
        }
        else
        {
            if (!get_block(sector, x + 1, z, &bsector, &bx, &bz) || blocktypes[bsector->blocktypes[bx][bz][y]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_SOUTH;
            if (y == 128 || blocktypes[sector->blocktypes[x][z][y + 1]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_TOP;
            if (!get_block(sector, x, z + 1, &bsector, &bx, &bz) || blocktypes[bsector->blocktypes[bx][bz][y]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_WEST;
            if (!get_block(sector, x - 1, z, &bsector, &bx, &bz) || blocktypes[bsector->blocktypes[bx][bz][y]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_NORTH;
            if (y == 0 || blocktypes[sector->blocktypes[x][z][y - 1]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_BOTTOM;
            if (!get_block(sector, x, z - 1, &bsector, &bx, &bz) || blocktypes[bsector->blocktypes[bx][bz][y]].flags & BLOCKTYPE_FLAG_TRANSPARENT)
                sector->blockfaces[x][z][y] |= FACE_EAST;
        }
    }
}

static inline void sector_update_block_faces(struct Sector *sector, short x, short y, short z)
{
    short old_count, new_count;

    old_count = sector->blockfaces[x][z][y] ? 1 : 0;
    _sector_compute_block_faces(sector, x, y, z);
    new_count = sector->blockfaces[x][z][y] ? 1 : 0;

    octree_update_block_count(0, new_count - old_count, 16, sector->octrees[y / 16], x, y % 16, z);
}


void sector_set_block(struct Sector *sector, short x, short y, short z, char blocktype, char blockdata)
{
    struct Sector *block_sector = sector;
    short block_x = 0, block_z = 0;

    sector->blocktypes[x][z][y] = blocktype;
    sector->blockdata[x][z][y] = blockdata;

    // Update faces
    sector_update_block_faces(sector, x, y, z);
    if (get_block(sector, x - 1, z, &block_sector, &block_x, &block_z))
        sector_update_block_faces(block_sector, block_x, y, block_z);
    if (get_block(sector, x, z - 1, &block_sector, &block_x, &block_z))
        sector_update_block_faces(block_sector, block_x, y, block_z);
    if (get_block(sector, x + 1, z, &block_sector, &block_x, &block_z))
        sector_update_block_faces(block_sector, block_x, y, block_z);
    if (get_block(sector, x, z + 1, &block_sector, &block_x, &block_z))
        sector_update_block_faces(block_sector, block_x, y, block_z);
    if (y > 0)
        sector_update_block_faces(sector, x, y - 1, z);
    if (y < 127)
        sector_update_block_faces(sector, x, y + 1, z);
}


void sector_update_boundaries(struct Sector *sector)
{
    for (unsigned int i=0; i < 128; i++)
    {
        for (unsigned int j=0; j < 16; j++)
        {
            if (sector->south != NULL)
                sector_update_block_faces(sector->south, 0, i, j);
            if (sector->north != NULL)
                sector_update_block_faces(sector->north, 15, i, j);
            if (sector->west != NULL)
                sector_update_block_faces(sector->west, j, i, 0);
            if (sector->east != NULL)
                sector_update_block_faces(sector->east, j, i, 15);
        }
    }
}


void sector_gen_faces(struct Sector *sector)
{
    memset(sector->octrees, 0, 16 * (1 + 8 + 64 + 512));

    for(unsigned short x=0; x < 16; x++)
    {
        for (unsigned short y=0; y < 128; y++)
        {
            for (unsigned short z=0; z < 16; z++)
            {
                _sector_compute_block_faces(sector, x, y, z);
                octree_update_block_count(0, sector->blockfaces[x][z][y] ? 1 : 0, 16, sector->octrees[y / 16], x, y % 16, z);
            }
        }
    }
}


inline void get_sector_coords(int *x, int *y, int *z, int *cx, int *cz)
{
    *cx = (*x) >> 4;
    *cz = (*z) >> 4;
    *x = (*x) & 15;
    *z = (*z) & 15;
}

