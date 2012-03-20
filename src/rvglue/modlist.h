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
 * $Id: modlist.h,v 1.1 2000/05/03 08:48:16 ali Exp ali $
 *
 * $Log: modlist.h,v $
 * Revision 1.1  2000/05/03 08:48:16  ali
 * Initial revision
 *
 *
 */
#ifndef __MODIFIER_LIST_H
#define __MODIFIER_LIST_H

#include "rvgl/mesh.h"
#include "rvgl/polymod.h"
#include "parsedmod.h"


//! linked-list for ModList
struct ModListEntry
{
  ParsedMod     *mod;
  ModListEntry  *next;

  /*! defines whether \a mod is owned by the ModList */
  bool owner;
};


//! list of modifiers
class ModList : public ParsedMod
{
public:
  ModList();
  virtual ~ModList();

  /*! applies it's modifiers in order */
  virtual bool apply(Polygon*);
  virtual bool bad(void);

  /*! adds a modifier to the modifer list */
  void add(ParsedMod *mod, bool own=true);

  virtual bool parse(Parser &);

  /*! called from parse() for unknown options.
   *
   * looks it up in the "define" section.
   *
   * can be reimplemented by subclasses to implement additional keywords */
  virtual bool parseUnknown(Parser &) hook;

  virtual void dump(const char *pref);

private: // functions
  bool parseItem(Parser &);
  void dummyread(Parser &);

private: // members
  ModListEntry *first, *last;
};



/*! an empty modifier list. Is returned by OptionSetKeeper::lookup() if
 * no named entry is found */
extern ModList empty_modlist;

#endif
