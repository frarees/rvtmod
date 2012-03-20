/*
 *  Part of ase2rv source code,
 *  Copyright (C) 2001 Gabor Varga (bootes@freemail.hu)
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
#include <errno.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "rvtypes.h"
#include "box.h"

#define GRANULARITY 8
#define MAX_TAZ_ID 255
#define MAX_VIS_ID 63

void Box::add_safety(rvfloat dist)
{
  size.x() += dist;
  size.z() += dist;
  size.z() += dist;
}

bool Box::axis_para(void)
{
  // If all three column vectors in matrix are parallel to a main axis then
  // the rotation matrix is also.
  int zeroes;
  for(int col = 0; col < 3; col++)
  {
    zeroes = 0;
    for(int lin = 0; lin < 3; lin++)
        zeroes += fabs(rotation.val(col,lin)) < EPSILON ? 1 : 0;
    if(zeroes != 2)
      return false;
  }
  return true;
}

BoxList::BoxList()
{
  cnt=0;
  max=0;
  list=NULL;
}



BoxList::~BoxList()
{
  reset();
}



void BoxList::add(Box *box)
{
  if(!box)
    xpanic("adding NULL box");
  if(cnt==max)
    grow();
  list[cnt++]=box;
}



int BoxList::write_taz(const char *fname, rvfloat grow/*=0.0*/)
{
  RV_TAZ_Data data;
  Box* box;
  checkSanity(/*vis=*/false);
  FILE *f=fopen(fname, "wb");
  if(!f)
    sxpanic("Error opening file '%s': %s\n", fname, strerror(errno));

  lprintf(1,"Writing TAZ File '%s'\n", fname);
  rvulong size=cnt;
  fwrite( &size, sizeof(rvulong), 1, f );
  for(int i=0; i<cnt; i++)
    {
    box=list[i];
    data.id=box->id;
    data.center=box->center;
    data.rotation=box->rotation;
    data.size=box->size;
    data.size.x() += grow;
    data.size.y() += grow;
    data.size.z() += grow;
    fwrite(&data, sizeof(RV_TAZ_Data), 1, f);
    }
  fclose(f);
  return 0;
}


int BoxList::write_vis(const char *fname, rvfloat cameraGrow/*=0.0*/, rvfloat cubeGrow/*=0.0*/)
{
  rvfloat grow;
  RV_VIS_Data data;
  Box* box;
  Vector rotSize;
  checkSanity(/*vis=*/true);
  FILE *f=fopen(fname, "wb");
  if(!f)
    sxpanic("Error opening file '%s': %s\n", fname, strerror(errno));

  lprintf(1,"Writing VIS File '%s'\n", fname);
  rvulong size=cnt;
  fwrite( &size, sizeof(rvulong), 1, f );
  for(int i=0; i<cnt; i++)
  {
    box=list[i];
    // fix by vag 2001-10-09. Using rotated size and absolute values.
    rotSize = box->rotation*box->size;
    rotSize.x() = fabs( rotSize.x() );
    rotSize.y() = fabs( rotSize.y() );
    rotSize.z() = fabs( rotSize.z() );
    grow = (box->flags & CAMERA_NODE) ? cameraGrow : cubeGrow;
    data.id=box->id;
    data.flags=box->flags;
    data.abox.xlo=box->center.x()-rotSize.x()-grow;
    data.abox.xhi=box->center.x()+rotSize.x()+grow;
    data.abox.ylo=box->center.y()-rotSize.y()-grow;
    data.abox.yhi=box->center.y()+rotSize.y()+grow;
    data.abox.zlo=box->center.z()-rotSize.z()-grow;
    data.abox.zhi=box->center.z()+rotSize.z()+grow;
    fwrite(&data, sizeof(RV_VIS_Data), 1, f);
  }
  fclose(f);
  return 0;
}



void BoxList::checkSanity(bool vis)
{
  if(list)
  {
    rvulong maxid=vis ? MAX_VIS_ID : MAX_TAZ_ID;
    int step;
    if(list[0]->id!=0)
      xpanic("Misordered box list: first box is %d", list[0]->id);
    if(vis)
      if(list[0]->flags!=CAMERA_NODE && list[0]->flags!=CUBE_NODE)
        xpanic("rubbish flags in box (#0, id0)");
    for(int i=1; i<cnt; i++)
    {
      step=list[i]->id-list[i-1]->id;
      if(step!=0 && step!=1)
        xpanic("misordered box list: %d comes after %d",
          list[i]->id, list[i-1]->id);
      if(vis)
        if(list[i]->flags!=CAMERA_NODE && list[i]->flags!=CUBE_NODE)
          xpanic("rubbish flags in box (#%d, id%d)", i, list[i]->id);
    }
    if(list[cnt-1]->id>maxid)
      sxpanic("Maximum id value (%d) exceeded, largest is %d\n", maxid, list[cnt-1]->id);
  }
}



void BoxList::reset()
{
  if(list)
  {
    while(--cnt>0)
      delete(list[cnt]);
    free(list);
    list=0;
  }
  max=0;
}



void BoxList::grow()
{
  int newmax=max+GRANULARITY;
  list=(Box**)realloc(list, newmax*sizeof(Box*)); MEM(list);
  max=newmax;
}



