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
#include <math.h>

#include "rvgl/rvtypes.h"
#include "rvgl/print.h"

#include "aseparser.h"
#include "asekeys.h"
#include "asefacevlist.h"


ASEFaceVList::ASEFaceVList(const char *_desc_name,  int _listsec_key, int _entry_key)
{
  size=-1;
  list=NULL;
  desc_name=strdup(_desc_name); MEM(desc_name);
  listsec_key=_listsec_key;
  entry_key=_entry_key;
}

ASEFaceVList::~ASEFaceVList(void)
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

void ASEFaceVList::readSize(ASEParser &p)
{
  int i;

  if(p.curtok != PT_INTEGER)
    sxpanic("%s list size is no int",desc_name);

  if(size != -1)
    sxpanic("Multiple %s list sizes",desc_name);

  size=p.tok_int;

  list=new ASEFaceVListEntry*[size]; MEM(list);
  for(i=0;i<size;i++)
    list[i]=NULL;

  lprintf(2,"  (%s list: %d entries)\n",desc_name,size);
}

void ASEFaceVList::readList(ASEParser &p)
{
  int curidx, curs, curid;

  if(p.curtok != PT_BBEGIN)
    sxpanic("%s section begins without '{'",id2keyname(listsec_key));
  if(size <0)
    sxpanic("%s before size definition",id2keyname(listsec_key));
  p.genKeyToken(true);

  curidx=0;

  while(p.curtok == PT_KEYWORD)
    {
      if(p.tok_key == entry_key)
	{
	  p.gentoken();

	  if(p.curtok != PT_INTEGER)
	    sxpanic("Strange %s",desc_name);

	  curid=p.tok_int;

	  if((curidx < 0)||(curidx>=size))
	    sxpanic("%s larger than specified",desc_name);

	  list[curidx]=new ASEFaceVListEntry; MEM(list[curidx]);
	  p.gentoken();

	  list[curidx]->id = curid;
	  for(curs=0;curs<3;curs++)
	    {
	      if(p.curtok != PT_INTEGER)
		sxpanic("%s sub entry is no number",desc_name);
	      list[curidx]->val[curs] = p.tok_int;
	      p.gentoken();
	    };
	  curidx++;
	}
      else
	p.genKeyToken(true);
    };

  if(p.curtok != PT_BEND)
    sxpanic("%s section ends strange",id2keyname(listsec_key));
  p.gentoken();
}

void ASEFaceVList::checkSanity(bool empty_allowed)
{
  int i;

  if(isEmpty()&&empty_allowed)
    return;

  if(list == NULL)
    sxpanic("%s list is empty",desc_name);
  for(i=0;i<size;i++)
    if(list[i]==NULL)
      sxpanic("%s list is incomplete",desc_name);

  lprintf(2,"  (%s list: ",desc_name);
  for(i=0;i<size;i++)
    {
      if(i!=0) lprintf(2,", ");
      lprintf(2,"%d=<%d,%d,%d>",list[i]->id, list[i]->val[0],list[i]->val[1],list[i]->val[2]);
    };

  lprintf(2,")\n");
}
