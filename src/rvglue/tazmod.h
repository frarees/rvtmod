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
#ifndef __TAZ_MODIFIER_H
#define __TAZ_MODIFIER_H

#include "parsedmod.h"

struct Mod_TAZ_Entry;

class Mod_TAZ : public ParsedMod
{
public:
  Mod_TAZ();
  virtual ~Mod_TAZ();

  virtual bool apply(Polygon*);
  virtual bool bad(void);
  virtual bool parse(Parser &);

  void dump(const char *pref);

private:
  bool parsed;

  void enterList(ModList *, rvulong id);
  bool readOneList(Parser &);
  bool readTAZFile(const char *fname);
  bool isPointInZone(Vector &, RV_TAZ_Data &);

  rvulong size;
  Mod_TAZ_Entry *zones;
  ModList *defaultmod;
};

#endif
