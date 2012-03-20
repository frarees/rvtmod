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
 * $Id: collapsedmesh.h,v 1.1 2000/05/03 08:49:53 ali Exp ali $
 *
 * $Log: collapsedmesh.h,v $
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 *
 *
 */
#ifndef __COLLAPSED_MESH_H
#define __COLLAPSED_MESH_H

#include "mesh.h"

//! Mesh container
/*!
 * subclass of Mesh with the ability to include other Meshes.
 *
 * That way, a whole set of meshes can be collapsed to a single
 * one that can be written to .prm files
 */
class CollapsedMesh : public Mesh
{
public:
  /*!
   * ctor.
   *
   * \a _max_polys and \a _max_verts are the maximum sizes the instance
   * can hold. Feeding more into it yields an xpanic().
   *
   * Although both are #rvulong, the thing can only hold as much fits
   * into #rvshort: 32000 polys and vertices.
   */
  CollapsedMesh(rvulong _max_polys, rvulong _max_verts);

  /*!
   * Adds a mesh. No reference is kept, all data is copied.
   *
   * \return
   * \c false, if the instance limits are reached
   */
  bool add(Mesh *);


  /*!
   * Writes the mesh to a .prm or .i-p file
   */
  void write_prm(FILE *f, bool is_IM);


  /*!
   * Sets the grip point.
   *
   * Feeding additional meshes into the class after setting grip destroys
   * everything...
   *
   * If \a grip is \c NULL, grip point will be calculated.
   */
  void set_grip(Vector *grip=NULL);

private:
  rvulong max_polys, max_verts;
};

#endif
