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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asekeys.h"


static struct s_keyentry
{
  const char *name;
  int id;
} keylist[] = {
  { "*GEOMOBJECT", AK_GEOMOBJECT },
  { "*NODE_NAME", AK_NODE_NAME },
  { "*MESH", AK_MESH },
  { "*MESH_NUMVERTEX", AK_NUM_VERTS },
  { "*MESH_NUMFACES", AK_NUM_FACES },
  { "*MESH_VERTEX_LIST", AK_VERT_LIST },
  { "*MESH_VERTEX", AK_VERTEX },
  { "*MESH_FACE_LIST", AK_FACE_LIST },
  { "*MESH_FACE", AK_FACE },
  { "*MESH_SMOOTHING", AK_SMOOTH_GROUP },
  { "*MESH_MTLID", AK_MAT_ID },
  { "*MESH_NUMTVERTEX", AK_NUM_T_VERTS },
  { "*MESH_TVERTLIST", AK_T_VERT_LIST },
  { "*MESH_TVERT", AK_T_VERT },
  { "*MESH_NUMTVFACES", AK_NUM_T_FACES },
  { "*MESH_TFACELIST", AK_T_FACE_LIST },
  { "*MESH_TFACE", AK_T_FACE },
  { "*MESH_NUMCVERTEX", AK_NUM_C_VERTS },
  { "*MESH_CVERTLIST", AK_C_VERT_LIST },
  { "*MESH_VERTCOL", AK_C_VERT },
  { "*MESH_NUMCVFACES", AK_NUM_C_FACES },
  { "*MESH_CFACELIST", AK_C_FACE_LIST },
  { "*MESH_CFACE", AK_C_FACE },
  { "*MESH_NORMALS", AK_NORMAL_LIST },
  { "*MESH_FACENORMAL", AK_FACE_NORMAL },
  { "*MESH_VERTEXNORMAL", AK_VERT_NORMAL },
  { "*MATERIAL_LIST", AK_MATERIAL_LIST },
  { "*MATERIAL", AK_MATERIAL },
  { "*SUBMATERIAL", AK_SUB_MATERIAL },
  { "*MATERIAL_TRANSPARENCY", AK_TRANSPARENCY },
  { "*MATERIAL_SPECULAR", AK_SPECULAR },
  { "*MAP_DIFFUSE", AK_DIFFUSE_MAP },
  { "*BITMAP", AK_BITMAP },
  { "*3DSMAX_ASCIIEXPORT", AK_VERSION },
  { "*MATERIAL_NAME", AK_MATERIAL_NAME },
  { "*MATERIAL_COUNT", AK_NUM_MATERIALS },
  { "*NUMSUBMTLS", AK_NUM_SUB_MATERIALS },
  { "*MATERIAL_REF", AK_MASTER_MAT_ID },
  {"",-1}
};

static char unknown_key[5] = "[?]";

const char *id2keyname(int id)
{
  int i=0;

  while(keylist[i].id != -1)
    {
      if(keylist[i].id == id)
	return keylist[i].name;
      i++;
    };
  return unknown_key;
}

int key2id(const char *str)
{
  int i=0;

  while(keylist[i].id != -1)
    {
      if(strcasecmp(str,keylist[i].name) == 0)
	return keylist[i].id;
      i++;
    };
  return -1;
}
