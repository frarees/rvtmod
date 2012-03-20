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
 * $Id: legomod.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: legomod.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/print.h"

#include "legomod.h"



Mod_Lego::Mod_Lego()
{
  parsed=false;
  keeper=new OptionSetKeeper;
}

bool Mod_Lego::bad(void)
{
  return !parsed;
}

Mod_Lego::~Mod_Lego()
{
  delete keeper;
}

bool Mod_Lego::apply(Polygon *p)
{
  ModList *mlist;

  switch(p->detTEPart())
    {
    case TEP_PWALL:
      mlist=keeper->lookup("pwall");
      break;
    case TEP_RWALL:
      mlist=keeper->lookup("rwall");
      break;
    case TEP_CARIMG:
      mlist=keeper->lookup("carimg");
      break;
    case TEP_FLOOR:
      mlist=keeper->lookup("floor");
      break;
    case TEP_LAMP:
      mlist=keeper->lookup("lamp");
      break;
    case TEP_BOX:
      mlist=keeper->lookup("box");
      break;
    default:
      mlist=keeper->lookup("track");
    };

  if(mlist != NULL)
    return mlist->apply(p);
  else
    return true;
}


bool Mod_Lego::parse(Parser &p)
{
  if(keeper->read(p) != 0)
    return false;
  parsed=true;
  return true;
}

void Mod_Lego::dump(const char *pref)
{
  char bufcp[200];

  printf("%sMod_Lego\n",pref);
  sprintf(bufcp,"%s | ",pref);
  keeper->dump(bufcp);
  printf("%s \\----\n",pref);
}
