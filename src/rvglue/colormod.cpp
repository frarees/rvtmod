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
 * $Id: colormod.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: colormod.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/print.h"

#include "colormod.h"

Mod_ColorModifier::Mod_ColorModifier(void)
{
  lowest=0; highest=255;
  strcpy(name,"?");
  parsed=false;
  color.val[0]=color.val[1]=color.val[2]=0;
}

bool Mod_ColorModifier::bad(void)
{
  return !parsed;
}

bool Mod_ColorModifier::parse(Parser &p)
{
  return parsed = color.parse(p, lowest, highest);
}


void Mod_ColorModifier::dump(const char *pref)
{
  printf("%s%s: ",pref,name);
  color.print(stdout);
  if(bad())
    printf(", BAD");
  printf("\n");
}







// ///////////////////////////// ENV_MAP

Mod_EnvMap::Mod_EnvMap(void)
{
  strcpy(name,"env_map");
}

bool Mod_EnvMap::apply(Polygon *p)
{
  p->envmap=Enabled;
  p->env_color.setRGB(color.val[0], color.val[1], color.val[2]);
  return true;
}













// ///////////////////////////// SET_COLOR ("color")

Mod_SetColor::Mod_SetColor(void)
{
  strcpy(name,"SetColor");
}

bool Mod_SetColor::apply(Polygon *p)
{
  int i;

  if(p->data.texture == -1)
    {
      for(i=0;i<p->numverts();i++)
	p->data.color[i].setRGB(color.val[0], color.val[1], color.val[2]);
    };
  return true;
}















// ///////////////////////////// SetShadedColor ("colorize")

Mod_SetShadedColor::Mod_SetShadedColor(void)
{
  strcpy(name,"SetShadedColor");
  highest=10000; // arbitrary but large
}

bool Mod_SetShadedColor::apply(Polygon *p)
{
  int i, chan;
  rvfloat bright;

  if(p->data.texture == -1)
    for(i=0;i<p->numverts();i++)
      {
	bright=p->data.color[i].brightness();
	p->data.color[i].setRGB(color.val[0], color.val[1], color.val[2]);
	p->data.color[i].shade(bright);
      };

  return true;
}











// ///////////////////////////// Shade ("shade")

Mod_Shade::Mod_Shade(void)
{
  strcpy(name,"Shade");
  highest=255;
  lowest=-255;
}

bool Mod_Shade::apply(Polygon *p)
{
  int i;
  ParsedColor newcol;

  if(p->data.texture == -1)
    for(i=0;i<p->numverts();i++)
      p->data.color[i].shadeAdd(color.val[0], color.val[1], color.val[2]);

  return true;
}








// ///////////////////////////// SetShade ("setshade")

Mod_SetShade::Mod_SetShade(void)
{
  strcpy(name,"Shade");
  highest=255;
  lowest=0;
}

bool Mod_SetShade::apply(Polygon *p)
{
 int i;

  if(p->data.texture >= 0)
    {
      for(i=0;i<p->numverts();i++)
	p->data.color[i].setRGB(color.val[0], color.val[1], color.val[2]);
    };
  return true;
}
