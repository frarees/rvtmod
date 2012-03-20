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
 * $Id: colormod.h,v 1.1 2000/05/03 08:48:41 ali Exp ali $
 *
 * $Log: colormod.h,v $
 * Revision 1.1  2000/05/03 08:48:41  ali
 * Initial revision
 *
 *
 */
#ifndef __COLOR_MODIFIER_H
#define __COLOR_MODIFIER_H

#include "parsedmod.h"
#include "parsedcolor.h"




//! Abstract thing for color-modifying mods
class Mod_ColorModifier : public ParsedMod
{
public:
  Mod_ColorModifier();

  virtual bool bad(void);
  virtual bool parse(Parser &);

  virtual void dump(const char *pref);

protected:
  bool parsed;
  ParsedColor color;

  int lowest, highest;
  char name[20];
};


//! implementation of \c envmap option
class Mod_EnvMap : public Mod_ColorModifier
{
public:
  Mod_EnvMap();

  virtual bool apply(Polygon*);
};


//! implementation of \c color option
class Mod_SetColor : public Mod_ColorModifier
{
public:
  Mod_SetColor();

  virtual bool apply(Polygon*);
};



//! implementation of \c colorize option
class Mod_SetShadedColor : public Mod_ColorModifier
{
public:
  Mod_SetShadedColor();

  virtual bool apply(Polygon*);
};




//! implementation of \c shade option
class Mod_Shade : public Mod_ColorModifier
{
public:
  Mod_Shade();

  virtual bool apply(Polygon*);
};




//! implementation of \c setshade option
class Mod_SetShade : public Mod_ColorModifier
{
public:
  Mod_SetShade();

  virtual bool apply(Polygon*);
};





#endif
