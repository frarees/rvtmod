/*
 *  Part of rvglue source code,
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

/*
 * $Id: readprm.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: readprm.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/mesh.h"
#include "rvgl/rvtypes.h"

#include "readprm.h"
#include "errdesc.h"



Mesh *create_ip_mesh(const char *filename)
{
  FILE *f;
  RV_PRM_Mesh prmmesh;
  Mesh *mesh;
  char i_head[16];
  int i;

  f=fopen(filename,"rb");
  if(f==NULL)
    return NULL;

  fread(i_head,16,1,f);
  for(i=0;i<16;i++)
    if(i_head[i] != 0)
      {
	perror(0,ERROR_IM_HEADER, "Intermediate file '%s' has malformatted header - may be corrupt\n",
	       filename);
	break;
      };

  fread(&prmmesh, RV_PRM_MESH_SIZE, 1, f);

  lprintf(3,"    `%s': %d polys, %d vertices\n",filename,prmmesh.num_polys,prmmesh.num_vecs);
  mesh=new Mesh(&prmmesh,f,true);
  fclose(f);

  return mesh;
}


Mesh *create_prm_mesh(const char *filename)
{
  FILE *f;
  RV_PRM_Mesh prmmesh;
  Mesh *mesh;

  f=fopen(filename,"rb");
  if(f==NULL)
    return NULL;

  fread(&prmmesh, RV_PRM_MESH_SIZE, 1, f);

  lprintf(3,"    `%s': %d polys, %d vertices\n",filename,prmmesh.num_polys,prmmesh.num_vecs);
  mesh=new Mesh(&prmmesh,f,false);
  fclose(f);

  return mesh;
}
