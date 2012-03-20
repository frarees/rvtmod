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
 * $Id: collapsedmesh.cpp,v 1.1 2000/05/03 08:50:04 ali Exp ali $
 *
 * $Log: collapsedmesh.cpp,v $
 * Revision 1.1  2000/05/03 08:50:04  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "rvtypes.h"
#include "collapsedmesh.h"


CollapsedMesh::CollapsedMesh(rvulong _max_polys, rvulong _max_verts)
{
  max_polys=_max_polys;
  max_verts=_max_verts;

  data.num_polys=data.num_vecs=0;

  poly=new Polygon*[_max_polys]; MEM(poly);
  verts=new Vertex*[_max_verts]; MEM(verts);  
}




bool CollapsedMesh::add(Mesh *m)
{
  rvulong i;

  /* Add polys: */
  if((rvulong)data.num_polys + (rvulong)m->data.num_polys > max_polys)
    xpanic("CollapsedMesh busted (polys)");

  for(i=0;i<m->data.num_polys;i++)
    {
      poly[data.num_polys + i] = new Polygon(this, m->poly[i]); MEM(poly[data.num_polys + i]);
      poly[data.num_polys + i]->shiftVIndices(data.num_vecs);
    };
  data.num_polys += m->data.num_polys;


  /* Add vertices */
  if((rvulong)data.num_vecs + (rvulong)m->data.num_vecs > max_verts)
    xpanic("CollapsedMesh busted (verts)");

  for(i=0;i<m->data.num_vecs;i++)
    {
      verts[data.num_vecs + i] = new Vertex(m->verts[i]); MEM(verts[data.num_vecs + i]);
    };
  data.num_vecs += m->data.num_vecs;

  return true;
}

void CollapsedMesh::write_prm(FILE *f, bool is_IM)
{
  rvulong i;

  fwrite(&data.num_polys, sizeof(rvushort), 1, f);
  fwrite(&data.num_vecs,  sizeof(rvushort), 1, f);

  for(i=0;i<data.num_polys;i++)
    poly[i]->write(f, is_IM, true);

  for(i=0;i<data.num_vecs;i++)
    verts[i]->write(f);
}

void CollapsedMesh::set_grip(Vector *grip)
{
  rvulong i;
  Vector transvec;

  if(grip != NULL)
    {
      transvec= *grip * (-1.0);
      for(i=0;i<data.num_vecs;i++)
	verts[i]->translate(&transvec);
    }
  else
    {
      calc_bounds();

      Vector bball(data.ball_center);
      set_grip(&bball);
    };
}
