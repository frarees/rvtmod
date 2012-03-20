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
 * $Id: wmodlist.cpp,v 1.2 2000/05/03 10:01:47 ali Exp ali $
 *
 * $Log: wmodlist.cpp,v $
 * Revision 1.2  2000/05/03 10:01:47  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"

#include "errdesc.h"
#include "keys.h"
#include "wmodlist.h"
#include "texfloor.h"
#include "fixpipe.h"
#include "fixbridge.h"

W_ModList::W_ModList(void)
{
}

W_ModList::~W_ModList(void)
{
  for (int i=0; i<mods.size(); i++)  // ces 2000-04-13
    delete mods[i];
}

bool W_ModList::parseUnknown(Parser &p)
{
  if((strcasecmp(p.tok_text,KEY_TEX_FLOOR_LONG)==0)||(strcasecmp(p.tok_text, KEY_TEX_FLOOR_SHORT)==0))
    {
      for (int i=0; i<mods.size(); i++)  // ces 2000-04-13
	if (dynamic_cast<MeshMod_TexFloor *>(mods[i]))
	  {
	    p.error(ERROR_MULTIPLE_W_MOD, "The '%s' keyword may only appear once", p.tok_text);
	    return false;
	  }

      p.gentoken();
      MeshMod_TexFloor *new_mod = new MeshMod_TexFloor;
      mods.push_back(new_mod);
      return new_mod->parse(p);
    }
  /*  else if((strcasecmp(p.tok_text,KEY_TEX_WALL_LONG)==0)||(strcasecmp(p.tok_text, KEY_TEX_WALL_SHORT)==0))
    {
      for (int i=0; i<mods.size(); i++)  // ces 2000-04-13
	if (dynamic_cast<MeshMod_TexWall *>(mods[i]))
	  {
	    p.error(ERROR_MULTIPLE_W_MOD, "The '%s' keyword may only appear once", p.tok_text);
	    return false;
	  };

      p.gentoken();
      MeshMod_TexFloor *new_mod = new MeshMod_TexWall;
      mods.push_back(new_mod);
      return new_mod->parse(p);
      }*/
  else if(strcasecmp(p.tok_text,KEY_FIX_PIPE)==0)  // ces 2000-04-13
    {
      for (int i=0; i<mods.size(); i++)
	if (dynamic_cast<MeshMod_FixPipe *>(mods[i]))
	  {
	    p.error(ERROR_MULTIPLE_W_MOD, "The '%s' keyword may only appear once", p.tok_text);
	    return false;
	  }

      p.gentoken();
      MeshMod_FixPipe *new_mod = new MeshMod_FixPipe;
      mods.push_back(new_mod);
      return new_mod->parse(p);
    }
  else if(strcasecmp(p.tok_text,KEY_FIX_BRIDGE)==0)  // ces 2000-04-13
    {
      for (int i=0; i<mods.size(); i++)
	if (dynamic_cast<MeshMod_FixBridge *>(mods[i]))
	  {
	    p.error(ERROR_MULTIPLE_W_MOD, "The '%s' keyword may only appear once", p.tok_text);
	    return false;
	  }

      p.gentoken();
      MeshMod_FixBridge *new_mod = new MeshMod_FixBridge;
      mods.push_back(new_mod);
      return new_mod->parse(p);
    }

  return ModList::parseUnknown(p);
}

bool W_ModList::apply_MeshOperators(Mesh *m)
{
  for (int i=0; i<mods.size(); i++)  // ces 2000-04-13
    if (!mods[i]->apply(m))
      return false;

  return true;
}

void W_ModList::dump(const char *pref)  // ces 2000-04-13
{
  char bufcp[200];

  printf("%sW_ModList\n",pref);

  for (int i=0; i<mods.size(); i++)
    {
      sprintf(bufcp,"%s | * ",pref);
      mods[i]->dump(bufcp);
    }

  printf("%s \\----\n",pref);

  ModList::dump(pref);
}
