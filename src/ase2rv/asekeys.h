/*
 *  Part of ase2rv source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __ASE_KEYS_H
#define __ASE_KEYS_H


const char *id2keyname(int id);
int key2id(const char *str);


#define AK_GEOMOBJECT           1

#define AK_NODE_NAME           2

#define AK_MESH           3

#define AK_NUM_VERTS           4
#define AK_VERT_LIST           5
#define AK_VERTEX           6

#define AK_NUM_FACES           7
#define AK_FACE_LIST           8
#define AK_FACE           9

#define AK_SMOOTH_GROUP           10

#define AK_MAT_ID           11

#define AK_NUM_T_VERTS           12
#define AK_T_VERT_LIST           13
#define AK_T_VERT           14

#define AK_NUM_T_FACES           15
#define AK_T_FACE_LIST           16
#define AK_T_FACE           17

#define AK_NUM_C_VERTS           18
#define AK_C_VERT_LIST           19
#define AK_C_VERT           20

#define AK_NUM_C_FACES           21
#define AK_C_FACE_LIST           22
#define AK_C_FACE           23

#define AK_NORMAL_LIST           24
#define AK_FACE_NORMAL           25
#define AK_VERT_NORMAL           26



#define AK_MATERIAL_LIST         27

#define AK_MATERIAL              28
#define AK_SUB_MATERIAL          29

#define AK_TRANSPARENCY          30

#define AK_SPECULAR              31

#define AK_DIFFUSE_MAP           32

#define AK_BITMAP                33

#define AK_VERSION               34

#define AK_MATERIAL_NAME         35

#define AK_NUM_MATERIALS         36
#define AK_NUM_SUB_MATERIALS     37

#define AK_MASTER_MAT_ID         38

#endif
