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
 * $Id: world.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: world.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 *
 *
 */
#ifndef __WORLD_H
#define __WORLD_H

#include "mesh.h"


//! Keeper for the whole track
class World
{
public:
  World();
  ~World();

  bool addMesh(Mesh *);

  int write_w  (const char *wname,   const char *iname, const char *ncpname);
  int write_prm(const char *prmname, const char *iname, const char *ncpname);

  /*!
   * Sets grip point.
   * Feeding additional meshes into the class after setting grip destroys
   * everything...
   * If \a grip is \c NULL, grip point will be calculated (centered on
   * visible stuff).
   */
  void set_grip(Vector *grip=NULL);  // vag 2001-06-28
  /*!
   * Calculate center of visible part.
   */
  Vector center_visible();  // vag 2001-06-28

private:
  int cur_size;
  Mesh **mlist;
};


#endif
