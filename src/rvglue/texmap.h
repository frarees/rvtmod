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
 * $Id: texmap.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: texmap.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __TEXMAP_MODIFIER_H
#define __TEXMAP_MODIFIER_H

#include "parsedmod.h"
#include "texdef.h"

//! implementation of \c texmap option
class Mod_TexMap : public ParsedMod
{
public:
  Mod_TexMap();

  virtual bool apply(Polygon*);
  virtual bool bad(void);
  virtual bool parse(Parser &);

  void dump(const char *pref);

private:
  bool parsed;
  bool rotate;  // vag 2001-05-08
  TextureDefinition srcdef, dstdef;
};


#endif
