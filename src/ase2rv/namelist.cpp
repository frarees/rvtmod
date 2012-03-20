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

#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include "rvgl/print.h"
#include "namelist.h"

#define GRANULARITY 8

NameList::NameList()
{
  cnt=0;
  max=0;
  names=NULL;
  touched=NULL;
}



NameList::~NameList()
{
  reset();
}



bool NameList::add(const char *name, bool dupCheck/*=false*/)
{
  if(!name)
    xpanic("adding NULL name");
  if(cnt==max)
    grow();
  names[cnt]=strdup(name); MEM(names[cnt]);
  touched[cnt]=false;
  cnt++;
  if(dupCheck)
  {
    for(int i=0; i<cnt-1; i++)
      if(!strcmp(name, names[i]))
        return true;
  }
  return false;
}



const char *NameList::at(int i)
{
  if(i<0 || i>cnt)
    xpanic("index out of range");
  return names[i];
}



const char *NameList::find(const char* name, bool touch/*=true*/)
{
  for(int i=0; i<cnt; i++)
    if(!strcasecmp(name, names[i]))
    {
      if(touch)
        touched[i]=true;
      return names[i];
    }
  return NULL;
}



void NameList::reset()
{
  if(names)
  {
    while(--cnt>0)
      free(names[cnt]);
    free(names);
    free(touched);
  }
  cnt=0;
  max=0;
  names=NULL;
  touched=NULL;
}



NameList *NameList::untouched()
{
  NameList *list=new NameList;
  for(int i=0; i<cnt; i++)
    if(!touched[i])
      list->add(names[i]);
  return list;
}

  
  
void NameList::grow()
{
  int newmax=max+GRANULARITY;
  names=(char**)realloc(names, newmax*sizeof(char*)); MEM(names);
  touched=(bool*)realloc(touched, newmax*sizeof(bool)); MEM(touched);
  max=newmax;
}

