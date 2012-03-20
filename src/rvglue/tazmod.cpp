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
 * $Id$
 *
 * $Log$
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/polygon.h"
#include "rvgl/helpfunc.h"
#include "rvgl/print.h"

#include "parser.h"
#include "modlist.h"
#include "errdesc.h"
#include "keys.h"
#include "tazmod.h"



struct Mod_TAZ_Entry
{
  RV_TAZ_Data data;

  ModList *mods;
  bool owns_mods;
};


Mod_TAZ::Mod_TAZ(void)
{
  size=0;
  zones=NULL;
  defaultmod=NULL;
}

Mod_TAZ::~Mod_TAZ(void)
{
  rvulong i;

  if(defaultmod != NULL)
    delete defaultmod;

  if(zones != NULL)
    {
      for(i=0;i<size;i++)
	if((zones[i].owns_mods) && (zones[i].mods != NULL))
	  delete zones[i].mods;
      delete zones;
    };
}




bool Mod_TAZ::bad(void)
{
  return !parsed;
}

bool Mod_TAZ::readTAZFile(const char *fname)
{
  FILE *f;
  rvulong i;

  if(zones != NULL)
    xpanic("Multiple read attempts in Mod_TAZ\n");

  f=fopen(fname,"rb");
  if(f==NULL)
    {
      perror(0, ERROR_FILE_ROPEN, "Cannot open `%s': %s",fname,strerror(errno));
      return false;
    };

  lprintf(1,"Reading `%s'\n",fname);
  fread(&size,sizeof(rvulong),1,f);

  zones=new Mod_TAZ_Entry[size]; MEM(zones);

  for(i=0;i<size;i++)
    {
      fread( &(zones[i].data), RV_TAZ_DATA_SIZE, 1,f);
      /* we are not interested in the rotation of the box, but rather in whether
       * a point is inside it. A point X is inside the box, if
       *
       *  Inv(RotMatrix)*(X - center) < SizeVec
       *
       * So, we can also invert the matrices now:
       */
      zones[i].data.rotation = zones[i].data.rotation.invert();

      zones[i].mods=NULL;
      zones[i].owns_mods=false;

      if(i>0)
	if(zones[i].data.id < zones[i-1].data.id)
	  xpanic("Assumption \"TAZ is sorted by ID's\" is wrong\n");
    };

  fclose(f);
  return true;
}


void Mod_TAZ::enterList(ModList *list, rvulong id)
{
  rvulong cur=0;

  while((cur<size) && (id > zones[cur].data.id))
    cur++;

  if(cur>=size) /* zone not in .taz file */
    {
      delete list;
      return;
    };

  if(id != zones[cur].data.id)/* zone not in .taz file */
    {
      delete list;
      return;
    };

  zones[cur].mods=list;
  zones[cur].owns_mods=true; /* the first one owns his list */
  
  cur++;
  while((cur<size) && (id == zones[cur].data.id))
    {
      zones[cur].mods=list;
      zones[cur].owns_mods=false; /* additional ones don't */
      cur++;
    };
}

bool Mod_TAZ::readOneList(Parser &p)
{
  bool add_to_default;
  int cur_zone;
  ModList *newlist;

  if(strcasecmp(p.tok_text, KEY_DEFAULT)==0)
    {
      p.gentoken();
      add_to_default=true;
    }
  else
    {
      if(  (cur_zone=p.eatNumber()) <0)
	return false;
      add_to_default=false;
    };

  if(p.curtok != PT_BBEGIN)
    {
      p.error(ERROR_EMPTY_FILEOPT,"ignoring empty option set for taz zone %d", cur_zone);
      return true;
    };

  newlist=new ModList;
  if(!newlist->parse(p))
    return false;

  if(!add_to_default)
    enterList(newlist, (rvulong)cur_zone);
  else
    {
      if(defaultmod==NULL)
	defaultmod=newlist;
      else
	{
	  p.error(ERROR_DUPLICATE_FILEOPT,"Ignoring extra \"default\" group in TAZ modifier");
	  delete newlist;
	};
    };
  return true;
}





bool Mod_TAZ::parse(Parser &p)
{
  if(!p.eatOpen(true))
    return false;

  if(p.curtok != PT_TEXT)
    {
      p.error(ERROR_EXPECTED_FNAME, "Expected .taz file name, not $$");
      return false;
    };

  if(!filehastype(p.tok_text, "taz"))
    {
      p.error(ERROR_EXPECTED_FNAME, "Expected .taz file name, not $$");
      return false;
    };

  if(!readTAZFile(p.tok_text))
    return false;
  p.gentoken();

  while(p.curtok != PT_BEND)
    {
      if(p.curtok != PT_TEXT)
	{
	  p.error(ERROR_EXPECTED_KEY,"Expected zone number or keyword in taz list, not $$");
	  return false;
	};

      if(!readOneList(p))
	return false;
    };

  if(!p.eatClose(true))
    return false;

  return true;
}

bool Mod_TAZ::isPointInZone(Vector &p, RV_TAZ_Data &zone)
{
  Vector vec;

  vec=p - zone.center;
  vec=zone.rotation * vec;
  return vec <= zone.size;
}

void Mod_TAZ::dump(const char *pref)
{
  char bufcp[200];
  rvulong cur;

  printf("%sMod_TAZ\n",pref);


  printf("%s |  default\n",pref);
  if(defaultmod == NULL)
    printf("%s |   (empty)\n",pref);
  else
    {
      sprintf(bufcp,"%s | * ",pref);
      defaultmod->dump(bufcp);
    };
 
  for(cur=0;cur<size;cur++)
    {
      printf("%s |  %d\n",pref,zones[cur].data.id);
      if(zones[cur].mods == NULL)
	printf("%s |   (empty)\n",pref);
      else
	{
	  sprintf(bufcp,"%s | %c ",pref, zones[cur].owns_mods ? '*' : ' ');
	  zones[cur].mods->dump(bufcp);
	};
    };

  printf("%s \\----\n",pref);
}






bool Mod_TAZ::apply(Polygon *p)
{
  bool is_in[4];
  rvulong curentry;
  int i, nverts=p->numverts();
  Vector *vecs[4];
  rvulong look_id;
  ModList *mods;
  bool has_been_applied=false;
  
  /* cache the vector pointers */
  for(i=0;i<nverts;i++)
    vecs[i]= &( p->parent->verts[p->vertidx(i)]->pos );

  curentry=0;
  while(curentry < size)
    {
      /* at start of a zone sequence.
       * consider every point being outside
       */
      is_in[0]=is_in[1]=is_in[2]=is_in[3]=false;

      // only look at the current id
      look_id=zones[curentry].data.id;
      mods=zones[curentry].mods;

      // run through all entries with this id:
      while((curentry<size) && (zones[curentry].data.id==look_id))
	{
	  for(i=0;i<nverts;i++)
	    if(!is_in[i])
	      {
		// if point isn't already in the zones, we look now
		if(isPointInZone(*vecs[i], zones[curentry].data))
		  is_in[i]=true;
	      };
	  curentry++;
	};

      // now we are through the zones. Collect is_in's in is_in[0]:
      if(nverts==4)
	is_in[0] = is_in[0] && is_in[1] && is_in[2] && is_in[3];
      else
	is_in[0] = is_in[0] && is_in[1] && is_in[2];

      // if its inside, apply the mod
      if(is_in[0] && mods!=NULL)
	{
	  if(! mods->apply(p) )
	    return false;
	  has_been_applied=true;
	};
    };

  if(!has_been_applied && (defaultmod != NULL))
    return defaultmod->apply(p);

  return true;
}



