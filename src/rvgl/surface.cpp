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
 * $Id: surface.cpp,v 1.2 2000/05/03 09:26:42 ali Exp ali $
 *
 * $Log: surface.cpp,v $
 * Revision 1.2  2000/05/03 09:26:42  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "vector.h"
#include "surface.h"

#define MAXIMUM_CAR_LENGTH 200.0


Surface::Surface(rvulong stype, rvulong type, 
		 Vector *v0, Vector *v1, Vector *v2, Vector *v3)
{
  vecs[0]=*v0;
  vecs[1]=*v1;
  vecs[2]=*v2;

  if(v3==NULL)
    nvecs=3;
  else
    {
      nvecs=4;
      vecs[3]=*v3;
    };

  createPolyhedron(type);
  ph.surface = stype;
}

Vector Surface::planarNormal(void)
{
  Vector v0, v2;

  v0 = vecs[0] - vecs[1];
  v2 = vecs[2] - vecs[0];
  return Vector(v0,v2);
}


void Surface::createPlane(RV_Plane *pl, Vector &normal, Vector &pos)
{
  pl->normal = normal;
  pl->distance = -pos.scalarprod(normal);
}


void Surface::write(FILE *f)
{
  fwrite(&ph, RV_POLYHEDRON_SIZE, 1, f);
}

void Surface::expandGrid(LookupGrid *grid)
{
  int i;

  for(i=0;i<nvecs;i++)
    grid->expandGrid(vecs[i]);
}

void Surface::createPolyhedron(rvulong type)
{
  Vector normal, snormal, tmpvec;
  int i;

  ph.type   =  ( (nvecs == 4) ? 1 : 0 )
    | ( type & (~1));

  ph.surface= 0;


  /******* BOUNDING BOX *********/
  ph.bbox.be_empty();
  
  for(i=0;i<nvecs;i++)
    ph.bbox.fit_into(vecs[i]);

  ph.bbox.add_safety();


  /********* MASTER PLANE *******/
  normal=planarNormal(); /* normal on surface */
  createPlane(&ph.plane[0], normal, vecs[1]);


  /********* BOUNDING PLANES ******/
  for(i=0;i<nvecs;i++)
    {
      tmpvec = vecs[(i+1) % nvecs] - vecs[i];
      snormal=Vector(normal, tmpvec);
      createPlane(&ph.plane[i+1], snormal, vecs[i]);
    };
}











bool Surface::isInGrid(LookupGrid *grid, int grid_x, int grid_z)
{
  Vector edges[4];
  int cur_side, num_outside, i;

  edges[0] = grid->gridcoord(grid_x, grid_z);
  edges[1] = grid->gridcoord(grid_x+1, grid_z);
  edges[2] = grid->gridcoord(grid_x, grid_z+1);
  edges[3] = grid->gridcoord(grid_x+1, grid_z+1);

  for(cur_side=0;cur_side<nvecs;cur_side++)
    {
      num_outside=0;
      
      for(i=0;i<4;i++)
	if(edges[i].scalarprod_xz(orient_vecs[cur_side]) < orient_distance[cur_side])
	  num_outside++;

      if(num_outside==4)
	return false;
    };
  return true;
}


void Surface::enterIntoGrid(LookupGrid *grid, rvulong myidx)
{
  int xlo, zlo, xhi, zhi;
  int x,z;
  int i;

  xlo=grid->xgridcoord(ph.bbox.xlo - MAXIMUM_CAR_LENGTH);
  zlo=grid->zgridcoord(ph.bbox.zlo - MAXIMUM_CAR_LENGTH);
  xhi=grid->xgridcoord(ph.bbox.xhi + MAXIMUM_CAR_LENGTH);
  zhi=grid->zgridcoord(ph.bbox.zhi + MAXIMUM_CAR_LENGTH);
  
  if(xlo<0) xlo=0;
  if(zlo<0) zlo=0;


#ifdef CHEAP_NCP_GRID
  for(x=xlo;x<=xhi;x++)
    for(z=zlo;z<=zhi;z++)
      grid->lichain(x,z)->push_back(myidx);
#else

  /* calculate orientations: */
  if(ph.plane[0].normal.y() < 0)
    {
      // face up
      for(i=0;i<nvecs;i++)
	orient_vecs[i] = vecs[(i+1)%nvecs] - vecs[i];
    }
  else
    {
      // face down
      for(i=0;i<nvecs;i++)
	orient_vecs[i] = vecs[(nvecs+i-1)%nvecs] - vecs[i];
    };
  for(i=0;i<nvecs;i++)
    {
      orient_vecs[i].rotate_right_xz();
      orient_is_waste[i]= (orient_vecs[i].lengthXZ() < 1.0);
      orient_vecs[i].normalizeXZ();
      orient_distance[i]= vecs[i].scalarprod_xz(orient_vecs[i]) - MAXIMUM_CAR_LENGTH;
    };

  // and enter:
  for(x=xlo;x<=xhi;x++)
    for(z=zlo;z<=zhi;z++)
      if(isInGrid(grid,x,z))
	grid->lichain(x,z)->push_back(myidx);
#endif

}
