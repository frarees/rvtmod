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
 * $Id: wmodlist.h,v 1.2 2000/05/03 09:58:38 ali Exp ali $
 *
 * $Log: wmodlist.h,v $
 * Revision 1.2  2000/05/03 09:58:38  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:48:43  ali
 * Initial revision
 */
#ifndef __WORLD_MODIFIER_LIST_H
#define __WORLD_MODIFIER_LIST_H

#include "rvgl/vector.h"
#include "rvgl/mesh.h"
#include "modlist.h"
#include "meshmod.h"

class W_ModList : public ModList
{
public:
  W_ModList();
  virtual ~W_ModList();

  virtual bool parseUnknown(Parser &);

  virtual bool apply_MeshOperators(Mesh *);

  virtual void dump(const char *pref);  // ces 2000-04-13

private:
  std::vector<MeshMod *> mods;  // ces 2000-04-13
};

#endif
