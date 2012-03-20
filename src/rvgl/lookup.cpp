/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
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
 * $Id: lookup.cpp,v 1.2 2000/05/03 08:59:04 ali Exp ali $
 *
 * $Log: lookup.cpp,v $
 * Revision 1.2  2000/05/03 08:59:04  ali
 * sync ces
 *
 * Revision 1.1  2000/05/03 08:50:04  ali
 * Initial revision
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "lookup.h"


#ifdef WIN32  // ces 2000-04-13
#define rint(_x_) floor((_x_)+0.5)
#endif


LookupGrid::LookupGrid(rvulong _size)
{
  xlo=zlo=1e40;
  xhi=zhi=-1e40;
  size=_size;
  ichains=NULL;
}

LookupGrid::~LookupGrid(void)
{
  int i;

  if(ichains != NULL)
    {
      for(i=0;i<zsz*xsz;i++)
	delete ichains[i];
      delete ichains;
    };
}

void LookupGrid::expandGrid(Vector &v)
{
  if(v.x() > xhi) xhi=v.x();
  if(v.x() < xlo) xlo=v.x();
  if(v.z() > zhi) zhi=v.z();
  if(v.z() < zlo) zlo=v.z();
}


void LookupGrid::write(FILE *f)
{
  IndexChain::iterator it;
  rvulong cur;
  int i;

  fwrite(&data, RV_LOOKUPGRID_SIZE,1,f);
  for(i=0;i<zsz*xsz;i++)
    {
      cur=ichains[i]->size(); fwrite(&cur, sizeof(rvulong), 1, f);
      for(it=ichains[i]->begin(); it!=ichains[i]->end(); ++it)
	{
	  cur=*it;
	  fwrite(&cur, sizeof(rvulong), 1, f);
	};
    };
}

IndexChain *LookupGrid::lichain(int x, int z)
{
  if((x<0)||(z<0)||(x>=xsz)||(z>=zsz))
    xpanic("LookupGrid::lichain(%d,%d) in %dx%d-grid",x,z,xsz,zsz);

  return ichains[ x + (z*xsz) ];
}

void LookupGrid::doneSizing(void)
{
  rvfloat size_x1, size_x2, size_z1, size_z2;
  int i;

  size_x1 = (xhi-xlo)/60.0; size_z1= (zhi-zlo)/60.0;
  size_x2 = size_z2 = 500.0;

  data.raster_size=rint((size_x1+ size_x2+ size_z1+ size_z2) / 4.0);
  data.x0 = xlo;
  data.z0 = zlo;
  
  data.x_size = ceil((xhi-xlo)/data.raster_size);
  data.z_size = ceil((zhi-zlo)/data.raster_size);

  xsz=(int)data.x_size; zsz=(int)data.z_size;

  ichains=new IndexChain*[zsz*xsz]; MEM(ichains);

  for(i=0;i<zsz*xsz;i++)
    {
      ichains[i]=new IndexChain; MEM(ichains[i]);
    };

  lprintf(3,"LookupGrid: bounds are X(%g..%g), Z(%g..%g)\n",xlo,xhi,zlo,zhi);
  lprintf(3,"LookupGrid: created %dx%d grid\n",xsz,zsz);
  lprintf(3,"LookupGrid: origin (%g,%g), raster_size %g\n",data.x0,data.z0,data.raster_size);
}


int LookupGrid::xgridcoord(rvfloat x)
{
  int res=(int)floor((x-xlo) / data.raster_size);
  
  if(res==xsz)
    res--;

  return res;
}


int LookupGrid::zgridcoord(rvfloat z)
{
  int res=(int)floor((z-zlo) / data.raster_size);

  if(res==zsz)
    res--;

  return res;
}

Vector LookupGrid::gridcoord(int x, int z)
{
  return Vector(data.x0 + ((rvfloat)x)*data.raster_size,
		0.0,
		data.z0 + ((rvfloat)z)*data.raster_size);
}
