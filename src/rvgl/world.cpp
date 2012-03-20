/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2001 Gabor Varga (vag) (bootes@freemail.hu)
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
 * $Id: world.cpp,v 1.2 2000/05/03 09:36:41 ali Exp ali $
 *
 * $Log: world.cpp,v $
 * Revision 1.2  2000/05/03 09:36:41  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "rvtypes.h"
#include "world.h"
#include "wwriter.h"
#include "ncpwriter.h"
#include "prmwriter.h"

#define MAX_MESHES 10000

World::World()
{
  mlist=new Mesh*[MAX_MESHES]; MEM(mlist);
  
  cur_size=0;
} 

World::~World()
{
  int i;
  
  for(i=0;i<cur_size;i++)
    delete mlist[i];

  delete mlist;
}



bool World::addMesh(Mesh *m)
{
  if(cur_size < MAX_MESHES)
    {
      mlist[cur_size]=m;
      cur_size++;
      return true;
    }
  else
    return false;
}


int World::write_w(const char *wname, const char *iname, const char *ncpname)
{
  int i;
  int wcnt=0;
  int polycnt=0;
  int last;

  if(wname != NULL)
    {
      WFileWriter wf(cur_size, false);
      
      lprintf(2,"Creating W File\n");
      for(i=0;i<cur_size;i++)
	{
	  wf.addMesh(mlist[i]);
	  wcnt++;
	};
      lprintf(1,"Writing W File '%s'\n",wname);

      wf.write(wname);
    };

  if(iname != NULL)
    {
      WFileWriter wf(cur_size, true);
      
      lprintf(2,"Creating I-W File\n");
      for(i=0;i<cur_size;i++)
	{
	  wf.addMesh(mlist[i]);
	  wcnt++;
	};
      lprintf(1,"Writing I-W File '%s'\n",iname);

      wf.write(iname);
    };



  if(ncpname != NULL)
    {
      for(i=0;i<cur_size;i++)
	polycnt += mlist[i]->numSolidPolys();

      NCPFileWriter ncpf(polycnt, false);
      last=-1;

      lprintf(2,"Creating NCP Data\n");

      for(i=0;i<cur_size;i++)
	{
	  ncpf.addMesh(mlist[i]);
	  showPercent(2,i,cur_size,last);
	};

      doneShowPercent(2);
      lprintf(2,"Creating NCP Grid\n");
      ncpf.createGrid();
      
      lprintf(1,"Writing NCP File '%s'\n",ncpname);
	
      ncpf.write(ncpname);
    };
  return wcnt;
}






int World::write_prm(const char *prmname, const char *iname, const char *ncpname)
{
  int i;
  int wcnt=0;
  int polycnt=0;
  int last;

  if(prmname != NULL)
    {
      PRMFileWriter prmf(cur_size, false);
      
      lprintf(2,"Creating PRM File\n");
      for(i=0;i<cur_size;i++)
	{
	  prmf.addMesh(mlist[i]);
	  wcnt++;
	};
      lprintf(1,"Writing PRM File '%s'\n",prmname);

      prmf.write(prmname);
    };

  if(iname != NULL)
    {
      PRMFileWriter ipf(cur_size, true);
      
      lprintf(2,"Creating I-P File\n");
      for(i=0;i<cur_size;i++)
	{
	  ipf.addMesh(mlist[i]);
	  wcnt++;
	};
      lprintf(1,"Writing I-P File '%s'\n",iname);

      ipf.write(iname);
    };

  if(ncpname != NULL)
    {
      for(i=0;i<cur_size;i++)
	polycnt += mlist[i]->numSolidPolys();

      NCPFileWriter ncpf(polycnt, true);
      last=-1;

      lprintf(2,"Creating NCP Data\n");

      for(i=0;i<cur_size;i++)
	{
	  ncpf.addMesh(mlist[i]);
	  showPercent(2,i,cur_size,last);
	};

      doneShowPercent(2);
      
      lprintf(1,"Writing NCP File '%s'\n",ncpname);
	
      ncpf.write(ncpname);
    };
  return wcnt;
}


void World::set_grip(Vector *grip /*=NULL*/)  // vag 2001-06-28
{
  int i;
  Vector transvec;

  if(grip != NULL)
    {
      transvec= *grip * (-1.0);
      for(i=0;i<cur_size;i++)
        mlist[i]->translate(transvec);
    }
  else
    {
      transvec= center_visible();
      set_grip(&transvec);
    }
}


Vector World::center_visible()  // vag 2001-06-28
{
  int i;
  AxisBox abox;

  abox.be_empty();
  for(i=0;i<cur_size;i++)
    mlist[i]->calc_visible_bounds(abox);
  return Vector
  (
    (abox.xlo+abox.xhi) / 2.0,
    (abox.ylo+abox.yhi) / 2.0,
    (abox.zlo+abox.zhi) / 2.0
  );
}

