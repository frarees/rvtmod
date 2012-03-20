/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
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
 * $Id: mesh.h,v 1.2 2000/05/03 09:09:58 ali Exp ali $
 *
 * $Log: mesh.h,v $
 * Revision 1.2  2000/05/03 09:09:58  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 */
#ifndef __MESH_H
#define __MESH_H

#include "rvtypes.h"
#include "polygon.h"
#include "vertex.h"
#include "surface.h"
#include "polymod.h"


//! Polygon mesh
/*!
 * class for RV_W_Mesh and RV_PRM_Mesh
 */
class Mesh
{
  friend class CollapsedMesh;

public:
  Mesh(RV_PRM_Mesh*, FILE *, bool intermediate);
  Mesh(RV_W_Mesh*, FILE *, bool intermediate);
  Mesh();

  /*!
   * ctor. Copies \e all data!
   */
  Mesh(Mesh *);
  ~Mesh();

  void translate(rvfloat dx, rvfloat dy, rvfloat dz);
  void translate(Vector &v) {translate(v.x(),v.y(),v.z());};  // vag 2001-06-28
  void rotate(Matrix_3x3 &);

  /*!
   * writes the mesh to a .w or .i-w file.
   *
   * writing to .prm's is done by CollapsedMesh
   */
  void write_w(FILE *, bool is_im_file);

  /*!
   * \return
   * \c true, if the mesh consists of 0 polygons
   */
  bool isDegenerated(void);

  /*!
   * removes deleted polygons from the poly list
   */
  void cleanupPolyList(void);

  /*!
   * removes unused vertices from the vertex list
   */
  void cleanupVertexList(void);

  /*!
   * optimizes mesh by calling cleanupPolyList() and cleanupVertexList()
   */
  void optimize(void);

  int numPolys(void);
  int numSolidPolys(void);

  Surface *createPolySurface(int pidx);

  /*!
   * adds a polygon to the mesh
   */
  void add(Polygon *p);  // ces 2000-04-13
  
  /*!
   * adds a vertex to the mesh
   */
  void add(Vertex *v);  // ces 2000-04-13

  /*!
   * deletes unnecessary polygons.
   *
   * Polys that are "off" are always deleted, deletion of others is
   * controlled by the arguments.
   */
  void deleteSomePolys(bool delUnsolids, bool delInvisibles);

  /*
   * Applies the modifier to every polygon
   */
  bool apply(PolygonModifier *);

  /*
   * Adjust abox (if needed) so this Mesh is inside.
   * Invisible polys are excluded.
   */
  void calc_visible_bounds(AxisBox &abox);  // vag 2001-06-28

protected:
  void load(FILE *, bool intermediate, bool is_Instance);

  void calc_bounds(void);

public:
  Polygon **poly;
  RV_W_Mesh data;
  Vertex **verts;
  int max_polys;  // ces 2000-04-13
  int max_verts;  // ces 2000-04-13
};



#endif
