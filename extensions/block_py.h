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

typedef struct {
    PyObject_HEAD
    unsigned short blocktype;
    unsigned short metadata;
    unsigned short lighting;
} Block;

extern PyTypeObject BlockType;
