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
 * $Id: simplemods.cpp,v 1.1 2000/05/03 08:49:37 ali Exp ali $
 *
 * $Log: simplemods.cpp,v $
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/polygon.h"
#include "rvgl/print.h"

#include "parser.h"
#include "errdesc.h"
#include "simplemods.h"



//! List of predefined surface types
const struct SurfaceKeyword
      {
        char key[20];
        int  value;
      } surfkeys [] =
  {
    {"standard",0},
    {"notracks",8},
    {"ice",10},
    {"grass",12},
    {"verybump",14},
    {"bump",15},
    {"mud",18},
    {"slippery",21},
    {"", -1}
  };








// ///////////////////////////// SURFACE

Mod_Surface::Mod_Surface(void)
{
  parsed=false;
  surface=0;
}

bool Mod_Surface::apply(Polygon *p)
{
  p->setSurface(surface);
  return true;
}

void Mod_Surface::dump(const char *pref)
{
  printf("%ssurface: %d",pref,surface);
  if(bad())
    printf(", BAD");
  printf("\n");
}

bool Mod_Surface::bad(void)
{
  return !parsed;
}



bool Mod_Surface::parse(Parser &p)
{
  bool haveopen;
  int val;
  const SurfaceKeyword *curkey;

  haveopen=p.eatOpen(false);

  val=p.eatNumber();
  if(val == -1)
    {
      /* surface keywords */
      curkey=surfkeys;

      do
	{
	  if(curkey->value == -1)
	    {
	      p.error(ERROR_EXPECTED_KEY, "Expected surface keyword, not $$");
	      return false;
	    }
	  else if(strcasecmp(p.tok_text,curkey->key)==0)
	    {
	      val=curkey->value;
	      p.gentoken();
	    }
	  else
	    curkey++;
	}
      while(val==-1);
    };

  if(haveopen)
    if(!p.eatClose(true))
      return false;

  surface=val;
  parsed=true;
  return true;
}









// //////////////////////////// TRANSPARENCY

Mod_Transparency::Mod_Transparency(void)
{
  value=0;
  parsed=false;
}

void Mod_Transparency::dump(const char *pref)
{
  printf("%stransparency: %d",pref,value);
  if(bad())
    printf(", BAD");
  printf("\n");
}

bool Mod_Transparency::apply(Polygon *p)
{
  p->setTransparency(value);
  return true;
}

bool Mod_Transparency::bad(void)
{
  return !parsed;
}

bool Mod_Transparency::parse(Parser &p)
{
  bool haveopen;

  haveopen=p.eatOpen(false);

  value=p.eatNumber();
  if(value == -1)
    {
      p.error(ERROR_EXPECTED_KEY, "Expected transparency value, not $$");
      return false;
    };

  if(value > 255)
    {
      p.error(ERROR_NUMBER_RANGE, "Transparency value must be between 0 and 255");
      return false;
    };

  if(haveopen)
    if(!p.eatClose(true))
      return false;

  parsed=true;
  return true;
}







// //////////////////////////// INVISIBLE

bool Mod_Invisible::apply(Polygon *p)
{
  p->turnInvisible();
  return true;
}

void Mod_Invisible::dump(const char *pref)
{
  printf("%sinvisible\n",pref);
}












// ///////////////////////////// NOCOLL

bool Mod_NoColl::apply(Polygon *p)
{
  p->setNoColl();
  return true;
}

void Mod_NoColl::dump(const char *pref)
{
  printf("%snocoll\n",pref);
}







// //////////////////// OFF

bool Mod_Off::apply(Polygon *p)
{
  p->turnInvisible();
  p->setNoColl();
  return true;
}

void Mod_Off::dump(const char *pref)
{
  printf("%soff\n",pref);
}








// //////////////////////////// INVISIBLE

bool Mod_DoubleSided::apply(Polygon *p)
{
  p->data.type |= POLY_IS_DOUBLE;
  return true;
}

void Mod_DoubleSided::dump(const char *pref)
{
  printf("%sinvisible\n",pref);
}








// //////////////////////////// LOWER_COLL

#define LOWER_COLL_VALUE (0.1)

Mod_LowerColl::Mod_LowerColl(void)  // whole class by vag 2001-06-28
{
  value=LOWER_COLL_VALUE;
  parsed=false;
}

bool Mod_LowerColl::apply(Polygon *p)
{
  // This modifier is somehow special because it does not really modify
  // the poly NOW. We don't want to lower the whole poly, only collision data.
  // So this value is checked and applied only when creating the Surface,
  // in Mesh::createPolySurface().
  p->lower_coll=value;
  return true;
}

void Mod_LowerColl::dump(const char *pref)
{
  printf("%slower_coll %f\n",pref, value);
}

