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
 * $Id: modlist.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: modlist.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "modlist.h"
#include "keys.h"
#include "simplemods.h"
#include "errdesc.h"
#include "colormod.h"
#include "optkeep.h"
#include "legomod.h"
#include "texwall.h"
#include "tazmod.h"
#include "texmap.h"


ModList empty_modlist;



ModList::ModList(void)
{
  first=last=NULL;
}


ModList::~ModList(void)
{
  ModListEntry *cur=first, *nxt;

  while(cur != NULL)
    {
      nxt=cur->next;
      if(cur->owner)
        delete cur->mod;
      delete cur;
      cur=nxt;
    };
}


void ModList::add(ParsedMod *mod, bool own)
{
  if(last == NULL)
    {
      first=last=new ModListEntry;
      first->next = NULL;
      first->owner=own;
      first->mod=mod;
    }
  else
    {
      last->next = new ModListEntry;
      last->next->next = NULL;
      last->next->mod = mod;
      last->next->owner=own;
      last=last->next;
    };
}



void ModList::dump(const char *pref)
{
  char bufcp[200];
  ModListEntry *cur=first;

  printf("%sModList\n",pref);

  while(cur != NULL)
    {
      sprintf(bufcp,"%s | %c ",pref, cur->owner ? '*' : ' ');
      cur->mod->dump(bufcp);
      cur=cur->next;
    };

  printf("%s \\----\n",pref);
}



bool ModList::apply(Polygon *p)
{
  ModListEntry *cur=first;

  while(cur != NULL)
    {
      if(!cur->mod->apply(p))
	return false;
      cur=cur->next;
    };

  return true;
}


bool ModList::bad(void)
{
  return false;
}


void ModList::dummyread(Parser &p)
{
  int level=0;

  if(p.curtok == PT_BBEGIN)
    {
      do
	{
	  if(p.curtok==PT_BBEGIN)
	    level++;
	  else if(p.curtok == PT_BEND)
	    level--;
	  p.gentoken();
	}
      while ( (p.curtok != PT_EOF) && (level > 0) );

    };
}




bool ModList::parseUnknown(Parser &p)
{
  ModList *ml;
  /* try to find it in the "define" section */
  ml=define_group.lookup(p.tok_text, false);
  
  if(ml == NULL)
    {
      p.error(ERROR_UNKNOWN_OPTION,"Unknown option $$");
      return false;
    }
  else
    {
      add(ml, false);
      p.gentoken();
      return true;
    };
}




bool ModList::parseItem(Parser &p)
{
  ParsedMod *newmod;

  dprintf("o","parseItem %s\n",p.tok_text);

  if(strcasecmp(p.tok_text,KEY_MOD_INVISIBLE)==0)
    newmod=new Mod_Invisible;
  else if(strcasecmp(p.tok_text,KEY_MOD_NOCOLL)==0)
    newmod=new Mod_NoColl;
  else if(strcasecmp(p.tok_text,KEY_MOD_ENVMAP)==0)
    newmod=new Mod_EnvMap;
  else if(strcasecmp(p.tok_text,KEY_MOD_OFF)==0)
    newmod=new Mod_Off;
  else if((strcasecmp(p.tok_text,KEY_SURFACE_LONG)==0)||(strcasecmp(p.tok_text,KEY_SURFACE_SHORT)==0))
    newmod=new Mod_Surface;
  else if((strcasecmp(p.tok_text,KEY_TRANS_LONG)==0)||(strcasecmp(p.tok_text,KEY_TRANS_SHORT)==0))
    newmod=new Mod_Transparency;
  else if(strcasecmp(p.tok_text,KEY_LEGOSPLIT)==0)
    newmod=new Mod_Lego;
  else if(strcasecmp(p.tok_text,KEY_SETCOLOR)==0)
    newmod=new Mod_SetColor;
  else if(strcasecmp(p.tok_text,KEY_SETSHADEDCOLOR)==0)
    newmod=new Mod_SetShadedColor;
  else if(strcasecmp(p.tok_text,KEY_SETSHADE)==0)
    newmod=new Mod_SetShade;
  else if(strcasecmp(p.tok_text,KEY_SHADE)==0)
    newmod=new Mod_Shade;
  else if(strcasecmp(p.tok_text,KEY_TAZSPLIT)==0)
    newmod=new Mod_TAZ;
  else if(strcasecmp(p.tok_text,KEY_MOD_TEXMAP)==0)
    newmod=new Mod_TexMap;
  else if(strcasecmp(p.tok_text, KEY_TEX_WALL_DOUBLE)==0)
    newmod=new Mod_TexWall(true);
  else if(strcasecmp(p.tok_text, KEY_TEX_WALL_SINGLE)==0)
    newmod=new Mod_TexWall(false);
  else if(strcasecmp(p.tok_text,KEY_DOUBLE)==0)
    newmod=new Mod_DoubleSided;
  else if(strcasecmp(p.tok_text,KEY_LOWER_COLL)==0)
    newmod=new Mod_LowerColl;
  else
    return parseUnknown(p);
  
  p.gentoken();
  if(newmod->parse(p))
    {
      add(newmod);
      return true;
    }
  else
    {
      delete newmod;
      return false;
    };
}



bool ModList::parse(Parser &p)
{
  dprintf("o", "Modlist::parse() start\n");
  if(p.curtok != PT_BBEGIN)
    return true; /* empty list */

  p.gentoken();

  while(p.curtok != PT_BEND)
    {
      if(p.curtok == PT_EOF)
	{
	  p.error(ERROR_EOF_IN_BLOCK,"no closing bracket in option list");
	  return false;
	}
      else if(p.curtok==PT_BBEGIN)
	{
	  p.error(ERROR_USELESS_BLOCK,"Ignoring useless block in option list");
	  dummyread(p);
	}
      else if(p.curtok == PT_TEXT)
	{
	  if(!parseItem(p))
	    return false;
	}
      else
	p.gentoken(); // this should never happen
    };
  p.gentoken();
  dprintf("o", "Modlist::parse() done\n");

  return true;
}
