/*
 *  Part of ase2rv source code,
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rvgl/rvtypes.h"
#include "rvgl/print.h"
#include "aseparser.h"
#include "asekeys.h"
#include "asevlist.h"


ASEVectorList::ASEVectorList(const char *_desc_name, int _listsec_key,
			     int _entry_key, bool _flip_y, int _storage_order[3])
{
  size=-1;
  list=NULL;
  desc_name=strdup(_desc_name); MEM(desc_name);
  listsec_key = _listsec_key;
  entry_key = _entry_key;
  flip_y=_flip_y;

  if(_storage_order == NULL)
    {
      storage_order[0] = 0;
      storage_order[1] = 1;
      storage_order[2] = 2;
    }
  else
    {
      storage_order[0] = _storage_order[0];
      storage_order[1] = _storage_order[1];
      storage_order[2] = _storage_order[2];
    };
}

ASEVectorList::~ASEVectorList(void)
{
  int i;

  free(desc_name);
  if(list != NULL)
    {
      for(i=0;i<size;i++)
	if(list[i]!=NULL)
	  delete list[i];
      delete list;
    };
}

void ASEVectorList::readSize(ASEParser &p)
{
  int i;

  if(p.curtok != PT_INTEGER)
    sxpanic("%s list size is no int",desc_name);

  if(size != -1)
    sxpanic("Multiple %s list sizes",desc_name);

  size=p.tok_int;

  list=new Vector*[size]; MEM(list);
  for(i=0;i<size;i++)
    list[i]=NULL;

  lprintf(2,"  (%s list: %d entries)\n",desc_name,size);
}

void ASEVectorList::readList(ASEParser &p)
{
  int curidx, curcoord;

  if(p.curtok != PT_BBEGIN)
    sxpanic("%s section begins without '{'", id2keyname(listsec_key));
  if(size <0)
    sxpanic("%s before size definition", id2keyname(listsec_key));
  p.genKeyToken(true);

  while(p.curtok == PT_KEYWORD)
    {
      if(p.tok_key == entry_key)
	{
	  p.gentoken();
	  if(p.curtok != PT_INTEGER)
	    sxpanic("Strange %s",desc_name);
	  curidx=p.tok_int;
	  if((curidx < 0)||(curidx>=size))
	    sxpanic("%s outside range",desc_name);
	  if(list[curidx]!=NULL)
	    sxpanic("Multiple %s definitions",desc_name);
	  list[curidx]=new Vector; MEM(list[curidx]);
	  p.gentoken();

	  for(curcoord=0;curcoord<3;curcoord++)
	    {
	      if((p.curtok != PT_FLOAT)&&(p.curtok != PT_INTEGER))
		sxpanic("%s coord is no number",desc_name);
	      (*list[curidx])[storage_order[curcoord]] = p.getFloatValue();
	      p.gentoken();
	    };
	  if(flip_y)
	    (*list[curidx])[1] = -(*list[curidx])[1];
	}
      else
	p.genKeyToken(true);
    };

  if(p.curtok != PT_BEND)
    sxpanic("%s section ends strange",id2keyname(listsec_key));
  p.gentoken();
}

void ASEVectorList::checkSanity(void)
{
  int i;
  if(list == NULL)
    sxpanic("%s list is empty",desc_name);
  for(i=0;i<size;i++)
    if(list[i]==NULL)
      sxpanic("%s list is incomplete",desc_name);

  lprintf(2,"  (%s list: ",desc_name);
  for(i=0;i<size;i++)
    {
      if(i!=0) lprintf(2,", ");
      lprintf(2,"<%g,%g,%g>",list[i]->coord[0],list[i]->coord[1],list[i]->coord[2]);
    };
  lprintf(2,")\n");
}

int ASEVectorList::find(const Vector &v)  // vag 2001-05-08
{
  if(list == NULL)
    sxpanic("%s list is empty",desc_name);
  for(int i=0;i<size;i++)
    if(list[i])
    {
      if((*list[i])==v)
          return i;
    }
    else
      sxpanic("%s list is incomplete",desc_name);
    return -1;
}

