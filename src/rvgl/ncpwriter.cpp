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
 * $Id: ncpwriter.cpp,v 1.2 2000/05/03 09:35:02 ali Exp $
 *
 * $Log: ncpwriter.cpp,v $
 * Revision 1.2  2000/05/03 09:35:02  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <memory.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "mesh.h"
#include "ncpwriter.h"
#include "lookup.h"
#include "print.h"


#define LIMIT_MAX_SURFACES 16000


NCPFileWriter::NCPFileWriter(rvulong maxsz, bool _is_instance)
{
  is_instance=_is_instance;
  cur_size=0;
  max_size=maxsz;
  slist=new Surface*[maxsz]; MEM(slist);
  grid=NULL;
}

NCPFileWriter::~NCPFileWriter()
{
  rvulong i;
  for(i=0;i<cur_size;i++)
    delete slist[i];
  delete slist;
  if(grid != NULL)
    delete grid;
}

bool NCPFileWriter::addSurface(Surface *s)
{
  if(cur_size >= max_size)  // ces 2000-04-13
    {
      rvulong   new_size  = max_size * 2;
      Surface **new_slist = new Surface*[new_size]; MEM(new_slist);

      memcpy(new_slist, slist, cur_size * sizeof(Surface*));

      delete slist;
	  slist    = new_slist;
	  max_size = new_size;
    }

  slist[cur_size]=s;
  cur_size++;
  return true;
}

bool NCPFileWriter::addMesh(Mesh *m)
{
  int i;

  for(i=0;i<m->numPolys();i++)
    if(!m->poly[i]->isNoColl())
      if(!addSurface(m->createPolySurface(i)))
	return false;

  return true;
}


int NCPFileWriter::write(const char *fname)
{
  FILE *f;
  rvulong i;
  rvushort sh_sz=cur_size;
  int last;

  if(cur_size > LIMIT_MAX_SURFACES)
    {
      lprintf(0,"\n*** Warning: ***\n");
      lprintf(0,"The .ncp contains %d solid surfaces, although Re-Volt supports only %d.\n",
	      cur_size, LIMIT_MAX_SURFACES);
      lprintf(0,"Thus, %d polygons will be non-solid. The 'extra' surfaces will be written\n",
	      cur_size-LIMIT_MAX_SURFACES);
      lprintf(0,"to the file, but been ignored by the game engine.\n");
    };
  

  f=fopen(fname,"wb");
  if(f==NULL)
    return errno;

  fwrite(&sh_sz, sizeof(rvushort), 1, f);

  last=-1;
  for(i=0;i<cur_size;i++)
    {
      slist[i]->write(f);
      if((i%20)==0)
	showPercent(2,i,cur_size,last);
    };
  doneShowPercent(2);

  if(!is_instance)
    grid->write(f);

  fclose(f);
  return 0;
}


void NCPFileWriter::createGrid(void)
{
  rvulong i;

  if(is_instance)
    xpanic("createGrid() in instance creator?");

  grid=new LookupGrid(cur_size); MEM(grid);
  for(i=0;i<cur_size;i++)
    slist[i]->expandGrid(grid);
  
  grid->doneSizing();

  for(i=0;i<cur_size;i++)
    slist[i]->enterIntoGrid(grid,i);
}
