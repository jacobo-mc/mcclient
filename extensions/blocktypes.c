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

#include "blocktypes.h"

#define         BOTTOM      0 // -y
#define         TOP         1 // +y
#define         EAST        2 // -z
#define         WEST        3 // +z
#define         NORTH       4 // -x
#define         SOUTH       5 // +x

float uvcorners[8][2] = {{0, 1./16.}, {1./16., 1./16}, {1./16., 0}, {0, 0},
                         {0, 1./16.}, {1./16., 1./16}, {1./16., 0}, {0, 0}};

bool grass_texfunc(unsigned short x, unsigned short y, unsigned short z,
                   struct Sector *sector, unsigned char face,
                   struct vertexattrib *vertices)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];
    if (face == BOTTOM)
    {
        // Dirt
        u = blocktypes[3].texcoords.u;
        v = blocktypes[3].texcoords.v;

        vertices[0].u = u / 16. + uvcorners[0][0];
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

        vertices[1].u = u / 16. + uvcorners[1][0];
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

        vertices[2].u = u / 16. + uvcorners[2][0];
        vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

        vertices[3].u = u / 16. + uvcorners[3][0];
        vertices[3].v = (15 - v) / 16. + uvcorners[3][1];
    }
    else if (face == TOP)
    {
        vertices[0].u = uvcorners[0][0];
        vertices[0].v = 15 / 16. + uvcorners[0][1];

        vertices[1].u = uvcorners[1][0];
        vertices[1].v = 15 / 16. + uvcorners[1][1];

        vertices[2].u = uvcorners[2][0];
        vertices[2].v = 15 / 16. + uvcorners[2][1];

        vertices[3].u = uvcorners[3][0];
        vertices[3].v = 15 / 16. + uvcorners[3][1];

        if (type == 2)
        {
            vertices[3].r *= 0.6;
            vertices[0].r = vertices[1].r = vertices[2].r = vertices[3].r;
            vertices[3].b *= 0.4;
            vertices[0].b = vertices[1].b = vertices[2].b = vertices[3].b;
        }
        else
        {
            //TODO: do better
            vertices[0].r *= 2;
            vertices[0].g *= 2;
            vertices[0].b *= 2;
            for (unsigned int i=1; i < 4; i++)
            {
                vertices[i].r = vertices[0].r;
                vertices[i].g = vertices[0].g;
                vertices[i].b = vertices[0].b;
            }
        }
    }
    else
    {
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;

        vertices[0].u = u / 16. + uvcorners[0][0];
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

        vertices[1].u = u / 16. + uvcorners[1][0];
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

        vertices[2].u = u / 16. + uvcorners[2][0];
        vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

        vertices[3].u = u / 16. + uvcorners[3][0];
        vertices[3].v = (15 - v) / 16. + uvcorners[3][1];
    }

    return true;
}

bool wood_texfunc(unsigned short x, unsigned short y, unsigned short z,
                  struct Sector *sector, unsigned char face,
                  struct vertexattrib *vertices)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];

    if (face == BOTTOM || face == TOP)
    {
        u = blocktypes[type].texcoords.u + 1;
        v = blocktypes[type].texcoords.v;

        vertices[0].u = u / 16. + uvcorners[0][0];
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

        vertices[1].u = u / 16. + uvcorners[1][0];
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

        vertices[2].u = u / 16. + uvcorners[2][0];
        vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

        vertices[3].u = u / 16. + uvcorners[3][0];
        vertices[3].v = (15 - v) / 16. + uvcorners[3][1];
    }
    else
    {
        switch (sector->blockdata[x][z][y])
        {
            case 0:
                u = blocktypes[type].texcoords.u;
                v = blocktypes[type].texcoords.v;
                break;
            case 1:
                u = 4; v = 7; break;
            case 2:
                u = 5; v = 7; break;
            default:
                u = blocktypes[type].texcoords.u;
                v = blocktypes[type].texcoords.v;
        }

        vertices[0].u = u / 16. + uvcorners[0][0];
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

        vertices[1].u = u / 16. + uvcorners[1][0];
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

        vertices[2].u = u / 16. + uvcorners[2][0];
        vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

        vertices[3].u = u / 16. + uvcorners[3][0];
        vertices[3].v = (15 - v) / 16. + uvcorners[3][1];
    }

    return true;
}


bool leaves_texfunc(unsigned short x, unsigned short y, unsigned short z,
                    struct Sector *sector, unsigned char face,
                    struct vertexattrib *vertices)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];

    switch (sector->blockdata[x][z][y] & 0x3)
    {
        case 0:
            u = blocktypes[type].texcoords.u;
            v = blocktypes[type].texcoords.v;
            break;
        case 1:
            u = 5; v = 8; break;
        case 2:
            u = 4; v = 8; break;
        default:
            u = blocktypes[type].texcoords.u;
            v = blocktypes[type].texcoords.v;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    vertices[0].r = vertices[0].b = 0;
    vertices[1].r = vertices[1].b = 0;
    vertices[2].r = vertices[2].b = 0;
    vertices[3].r = vertices[3].b = 0;

    return true;
}


bool dispenser_texfunc(unsigned short x, unsigned short y, unsigned short z,
                       struct Sector *sector, unsigned char face,
                       struct vertexattrib *vertices)
{
    float u, v;
    unsigned char type = sector->blocktypes[x][z][y];

    if (face == TOP || face == BOTTOM)
    {
        u = 14;
        v = 3;
    }
    else if (face == sector->blockdata[x][z][y])
    {
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;
    }
    else
    {
        u = 13;
        v = 2;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool bed_texfunc(unsigned short x, unsigned short y, unsigned short z,
                 struct Sector *sector, unsigned char face,
                 struct vertexattrib *vertices)
{
    float u, v;
    static const unsigned char orientation_to_face[] = {WEST, NORTH, EAST, SOUTH};
    static const unsigned char right_directions[] = {SOUTH, WEST, NORTH, EAST};
    unsigned char orientation;
    bool mirror = false;

    orientation = sector->blockdata[x][z][y] & 7;

    if (face == BOTTOM)
        return false;

    vertices[0].y -= .5;
    vertices[1].y -= .5;
    vertices[2].y -= .5;
    vertices[3].y -= .5;

    if (face == TOP)
    {
        if (sector->blockdata[x][z][y] & 8) // Head
        {
            u = 7;
            v = 8;
            orientation += 1;
        }
        else
        {
            u = 6;
            v = 8;
            orientation = (orientation + 1) % 4;
        }
    }
    else
    {
        if (sector->blockdata[x][z][y] & 8) // Head
        {
            if (orientation_to_face[orientation] == face)
                u = 8;
            else if (orientation_to_face[(orientation + 2) % 4] == face)
                return false;
            else
            {
                if (right_directions[orientation] != face)
                    mirror = true;
                u = 7;
            }
            v = 9;
        }
        else
        {
            if (orientation_to_face[(orientation + 2) % 4] == face)
                u = 5;
            else if (orientation_to_face[orientation] == face)
                return false;
            else
            {
                if (right_directions[orientation] != face)
                    mirror = true;
                u = 6;
            }
            v = 9;
        }
        orientation = 0;
    }

    if (mirror)
        vertices[0].u = u / 16. + 1 / 16. - uvcorners[orientation][0];
    else
        vertices[0].u = u / 16. + uvcorners[orientation][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[orientation][1];

    if (mirror)
        vertices[1].u = u / 16. + 1 / 16. - uvcorners[orientation + 1][0];
    else
        vertices[1].u = u / 16. + uvcorners[orientation + 1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[orientation + 1][1];

    if (mirror)
        vertices[2].u = u / 16. + 1 / 16. - uvcorners[orientation + 2][0];
    else
        vertices[2].u = u / 16. + uvcorners[orientation + 2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[orientation + 2][1];

    if (mirror)
        vertices[3].u = u / 16. + 1 / 16. - uvcorners[orientation + 3][0];
    else
        vertices[3].u = u / 16. + uvcorners[orientation + 3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[orientation + 3][1];

    return true;
}


bool wool_texfunc(unsigned short x, unsigned short y, unsigned short z,
                  struct Sector *sector, unsigned char face,
                  struct vertexattrib *vertices)
{
    float u, v;
    unsigned char data = sector->blockdata[x][z][y];

    switch (data)
    {
        case 0: // Normal (white)
            u = 0; v = 4; break;
        case 1: // Orange
            u = 2; v = 13; break;
        case 2: // Magenta
            u = 2; v = 12; break;
        case 3: // Light blue
            u = 2; v = 11; break;
        case 4: // Yellow
            u = 2; v = 10; break;
        case 5: // Light green
            u = 2; v = 9; break;
        case 6: // Pink
            u = 2; v = 8; break;
        case 7: // Gray
            u = 2; v = 7; break;
        case 8: // Light Gray
            u = 1; v = 14; break;
        case 9: // Cyan
            u = 1; v = 13; break;
        case 10: // Purple
            u = 1; v = 12; break;
        case 11: // Blue
            u = 1; v = 11; break;
        case 12: // Brown
            u = 1; v = 10; break;
        case 13: // Dark Green
            u = 1; v = 9; break;
        case 14: // Red
            u = 1; v = 8; break;
        case 15: // Black
            u = 1; v = 7; break;
        default:
            u = 0; v = 4;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool bookshelf_texfunc(unsigned short x, unsigned short y, unsigned short z,
                       struct Sector *sector, unsigned char face,
                       struct vertexattrib *vertices)
{
    float u, v;

    //TODO: check
    if (face == BOTTOM || face == TOP)
    {
        u = blocktypes[5].texcoords.u;
        v = blocktypes[5].texcoords.v;
    }
    else
    {
        u = blocktypes[47].texcoords.u;
        v = blocktypes[47].texcoords.v;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool crafting_table_texfunc(unsigned short x, unsigned short y, unsigned short z,
                            struct Sector *sector, unsigned char face,
                            struct vertexattrib *vertices)
{
    float u, v;
    static const unsigned char foo[] = {[EAST] = 1, [WEST] = 1, [NORTH] = 0, [SOUTH] = 0};

    if (face == TOP)
    {
        u = 11;
        v = 2;
    }
    else if (face == BOTTOM)
    {
        u = blocktypes[5].texcoords.u;
        v = blocktypes[5].texcoords.v;
    }
    else
    {
        u = 11 + foo[face];
        v = 3;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool seeds_texfunc(unsigned short x, unsigned short y, unsigned short z,
                   struct Sector *sector, unsigned char face,
                   struct vertexattrib *vertices)
{
    //TODO: check
    float u, v;

    if (face == TOP || face == BOTTOM)
    {
        return false;
    }
    else
    {
        u = 8 + (sector->blockdata[x][z][y] & 7);
        v = 5;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool pumpkin_texfunc(unsigned short x, unsigned short y, unsigned short z,
                     struct Sector *sector, unsigned char face,
                     struct vertexattrib *vertices)
{
    //TODO: check
    float u, v;
    unsigned char orientation_to_face[] = {EAST, SOUTH, WEST, NORTH};
    unsigned char type = sector->blocktypes[x][z][y];

    if (face == TOP)
    {
        u = 6;
        v = 6;
    }
    else if (face == orientation_to_face[sector->blockdata[x][z][y] & 5])
    {
        u = blocktypes[type].texcoords.u;
        v = blocktypes[type].texcoords.v;
    }
    else
    {
        u = 6;
        v = 7;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[0].v = (15 - v) / 16. + uvcorners[0][1];

    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[1].v = (15 - v) / 16. + uvcorners[1][1];

    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];

    vertices[3].u = u / 16. + uvcorners[3][0];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    return true;
}


bool slab_texfunc(unsigned short x, unsigned short y, unsigned short z,
                  struct Sector *sector, unsigned char face,
                  struct vertexattrib *vertices)
{
    float u, v;
    switch (sector->blockdata[x][z][y])
    {
        case 0x00:
            // Stone slab
            if (face == TOP || face == BOTTOM)
                { u = 6; v = 0; }
            else
                { u = 5; v = 0; }
            break;
        case 0x01:
            // Sandstone slab
            if (face == TOP || face == BOTTOM)
                { u = 0; v = 11; }
            else
                { u = 0; v = 12; }
            break;
        case 0x02:
            // Wooden slab
            if (face == TOP || face == BOTTOM)
                { u = 4; v = 0; }
            else
                { u = 4; v = 0; }
            break;
        case 0x03:
            // Cobblestone slab
            if (face == TOP || face == BOTTOM)
                { u = 0; v = 1; }
            else
                { u = 0; v = 1; }
            break;
    }

    vertices[0].u = u / 16. + uvcorners[0][0];
    vertices[1].u = u / 16. + uvcorners[1][0];
    vertices[2].u = u / 16. + uvcorners[2][0];
    vertices[3].u = u / 16. + uvcorners[3][0];

    vertices[2].v = (15 - v) / 16. + uvcorners[2][1];
    vertices[3].v = (15 - v) / 16. + uvcorners[3][1];

    if (face == TOP || face == BOTTOM)
    {
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1];
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1];
        if (face == TOP)
        {
            vertices[0].y -= .5;
            vertices[1].y -= .5;
            vertices[2].y -= .5;
            vertices[3].y -= .5;
        }
    }
    else
    { //TODO: only half of the texture
        vertices[0].v = (15 - v) / 16. + uvcorners[0][1] / 2.;
        vertices[1].v = (15 - v) / 16. + uvcorners[1][1] / 2.;
        vertices[0].y -= .5;
        vertices[1].y -= .5;
    }

    return true;
}


struct BlockType blocktypes[256] = {
    [0] = {.name = "air",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT},
    [1] = {.name = "stone",
           .texcoords = {1, 0}},
    [2] = {.name = "grass",
           .texcoords = {3, 0},
           .texfunc = grass_texfunc},
    [3] = {.name = "dirt",
           .texcoords = {2, 0}},
    [4] = {.name = "cobblestone",
           .texcoords = {0, 1}},
    [5] = {.name = "wooden plank",
           .texcoords = {4, 0}},
    [6] = {.name = "sapling",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {15, 0},
           .drawfunc = NULL}, //TODO
    [7] = {.name = "bedrock",
           .texcoords = {1, 1}},
    [8] = {.name = "water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
           .texcoords = {13, 12}, //TODO: check
           .drawfunc = NULL}, //TODO
    [9] = {.name = "stationary water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 12}}, //TODO: check
    [10] = {.name = "lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 14}, //TODO: check
            .drawfunc = NULL}, //TODO
    [11] = {.name = "stationary lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 14}}, //TODO: check
    [12] = {.name = "sand",
            .texcoords = {2, 1}},
    [13] = {.name = "gravel",
            .texcoords = {3, 1}},
    [14] = {.name = "gold ore",
            .texcoords = {2, 0}},
    [15] = {.name = "iron ore",
            .texcoords = {2, 1}},
    [16] = {.name = "coal ore",
            .texcoords = {2, 2}},
    [17] = {.name = "wood",
            .texcoords = {4, 1},
            .texfunc = wood_texfunc},
    [18] = {.name = "leaves",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_ALLFACES,
            .texcoords = {4, 3},
            .texfunc = leaves_texfunc},
    [20] = {.name = "glass",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 3}},
    [21] = {.name = "lapis lazuli ore",
            .texcoords = {0, 10}},
    [22] = {.name = "lapis lazuli block",
            .texcoords = {0, 9}},
    [23] = {.name = "dispenser",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {14, 2},
            .texfunc = dispenser_texfunc},
    [24] = {.name = "sandstone",
            .texcoords = {0, 11}, //TODO
            .texfunc = NULL}, //TODO
    [25] = {.name = "note block",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {10, 3}},
    [26] = {.name = "bed",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texcoords = {6, 8},
            .texfunc = bed_texfunc},
    [27] = {.name = "powered rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 11},
            .drawfunc = NULL}, // TODO
    [28] = {.name = "detector rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 12},
            .drawfunc = NULL}, // TODO
    [35] = {.name = "wool",
            .texcoords = {0, 4}, //TODO
            .texfunc = wool_texfunc}, //TODO: handle more colors
    [37] = {.name = "dandelion",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 0},
            .drawfunc = NULL}, //TODO: for orientation
    [38] = {.name = "rose",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 0},
            .drawfunc = NULL}, //TODO: for orientation
    [39] = {.name = "brown mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 1},
            .drawfunc = NULL}, //TODO: for orientation
    [40] = {.name = "red mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 1},
            .drawfunc = NULL}, //TODO: for orientation
    [41] = {.name = "gold block",
            .texcoords = {7, 1}},
    [42] = {.name = "iron block",
            .texcoords = {6, 1}},
    [43] = {.name = "double slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [44] = {.name = "slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = slab_texfunc}, //TODO
    [45] = {.name = "brick block",
            .texcoords = {7, 0}},
    [46] = {.name = "tnt",
            .texfunc = NULL}, //TODO
    [47] = {.name = "bookshelf",
            .texcoords = {3, 2},
            .texfunc = bookshelf_texfunc}, //TODO: check
    [48] = {.name = "moss stone",
            .texcoords = {4, 2}},
    [49] = {.name = "obsidian",
            .texcoords = {5, 2}},
    [50] = {.name = "torch",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {0, 5},
            .drawfunc = NULL}, //TODO
    [51] = {.name = "fire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [52] = {.name = "monster spawner",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 4}}, //TODO
    [53] = {.name = "wooden stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [54] = {.name = "chest",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 1},
            .texfunc = NULL}, //TODO
    [55] = {.name = "redstone wire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [56] = {.name = "diamond ore",
            .texcoords = {2, 3}},
    [57] = {.name = "diamond block",
            .texcoords = {8, 1}},
    [58] = {.name = "crafting table",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 2},
            .texfunc = crafting_table_texfunc},
    [59] = {.name = "seeds",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {8, 5}, //TODO: use that in function
            .texfunc = seeds_texfunc},
    [60] = {.name = "farmland",
            .texcoords = {6, 5}, //TODO
            .texfunc = NULL}, //TODO
    [61] = {.name = "furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {12, 2},
            .texfunc = dispenser_texfunc},
    [62] = {.name = "burning furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {13, 3},
            .texfunc = dispenser_texfunc},
    [63] = {.name = "sign post",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [64] = {.name = "wooden door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 5},
            .drawfunc = NULL}, //TODO
    [65] = {.name = "ladders",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [66] = {.name = "rails",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [67] = {.name = "cobblestone stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [68] = {.name = "wall sign",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [69] = {.name = "lever",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [70] = {.name = "stone pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {1, 0},
            .drawfunc = NULL}, //TODO
    [71] = {.name = "iron door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [72] = {.name = "wooden pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [73] = {.name = "redstone ore",
            .texcoords = {3, 3}},
    [74] = {.name = "glowing redstone ore",
            .texcoords = {3, 3}}, //TODO
    [75] = {.name = "redstone torch (off)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [76] = {.name = "redstone torch (on)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [77] = {.name = "stone button",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .drawfunc = NULL}, //TODO
    [78] = {.name = "snow",
           .texcoords = {4, 4},
           .texfunc = grass_texfunc},
    [79] = {.name = "ice",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {3, 4}},
    [80] = {.name = "snow block",
            .texcoords = {2, 4}},
    [81] = {.name = "cactus",
            .texcoords = {5, 4}, //Warning: cactus are smaller than other blocks!
            .texfunc = NULL}, //TODO: share with trees?
    [82] = {.name = "clay block",
            .texcoords = {8, 4}},
    [83] = {.name = "sugar cane",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {9, 4},
            .drawfunc = NULL}, //TODO: share with flowers/mushrooms? No.
    [84] = {.name = "jukebox",
            .texcoords = {11, 4},
            .texfunc = NULL}, //TODO: share with bookshelf?
    [85] = {.name = "fence",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [86] = {.name = "pumpkin",
            .texcoords = {7, 7},
            .texfunc = pumpkin_texfunc},
    [87] = {.name = "netherrack",
            .texcoords = {7, 6}},
    [88] = {.name = "soul sand",
            .texcoords = {8, 6}},
    [89] = {.name = "glowstone block",
            .texcoords = {9, 6}},
    [90] = {.name = "portal",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .drawfunc = NULL}, //TODO
    [91] = {.name = "jack-o-lantern",
            .texcoords = {8, 7},
            .texfunc = pumpkin_texfunc},
    [92] = {.name = "cake block",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texfunc = NULL}, //TODO
    [93] = {.name = "redstone repeater (off)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    [94] = {.name = "redstone repeater (on)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    //... TODO...
};
