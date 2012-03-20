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
 * $Id: rvtypes.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: rvtypes.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 */
#ifndef __RVTYPES_H
#define __RVTYPES_H

#include <math.h>

#include "rvbasetypes.h"
#include "rvcolor.h"
#include "vector.h"
#include "matrix3.h"
#include "vertex.h"
#include "axisbox.h"

#define SUFFIX_W_STD    "w"
#define SUFFIX_W_IM     "i-w"
#define SUFFIX_PRM_STD  "prm"
#define SUFFIX_PRM_IM   "i-p"
#define SUFFIX_NCP      "ncp"
#define SUFFIX_TAZ      "taz"       /* vag 2001-05-08 */
#define SUFFIX_VIS      "vis"       /* vag 2001-05-08 */


#define SUFFIX_DOT_W_STD    ".w"
#define SUFFIX_DOT_W_IM     ".i-w"
#define SUFFIX_DOT_PRM_STD  ".prm"
#define SUFFIX_DOT_PRM_IM   ".i-p"
#define SUFFIX_DOT_NCP      ".ncp"
#define SUFFIX_DOT_TAZ      ".taz"  /* vag 2001-05-08 */
#define SUFFIX_DOT_VIS      ".vis"  /* vag 2001-05-08 */



//! Texture reference.
/*! u and v are between 0 and 1
 */
struct RV_UV
{
  rvfloat     u;
  rvfloat     v;
};
#define RV_UV_SIZE sizeof(RV_UV)








//! long mesh header
/*! Header for meshes in .w files
 */
struct RV_W_Mesh
{
  //! center of bounding ball
  Vector   ball_center;
  //! radius of bounding ball
  rvfloat  ball_radius;

  AxisBox  bbox;

  //! number of polygons in mesh
  rvushort num_polys;
  //! number of vertices for mesh
  rvushort num_vecs;
};
#define RV_W_MESH_SIZE sizeof(RV_W_Mesh)





//! short mesh header
/*! Header for meshes in .prm files
 */
struct RV_PRM_Mesh
{
  //! number of polygons in  mesh
  rvushort    num_polys;

  //! number of vertices for mesh
  rvushort    num_vecs;
};
#define RV_PRM_MESH_SIZE sizeof(RV_PRM_Mesh)









//! polygon definition
/*! A three- or four-sided, flat polygon
 * with a texture.
 *
 * class is 'Polygon' (polygon.h)
 */
struct RV_Poly
{
  /*! bitfield:
   *
   * POLY_IS_QUAD : four sided
   *
   * POLY_IS_DOUBLE : double-sided
   *
   * POLY_IS_TRANS : is transparent
   *
   * POLY_TRANS_1_1 : if set, additive transparency, else: uses alpha channel
   *
   * POLY_PRM_DISABLE_ENV : for a .prm polygon: disable env mapping
   *
   * POLY_W_ENABLE_ENV : for a .w polygon: enable env mapping
   */
  rvushort    type;

  /*! 0=a.bmp, 1=b.bmp,...,
   * -1=Solid colored,
   *
   * For intermediate files: also -2=invisible
   */
  rvshort     texture;

  /*! index of vertex in the mesh's list */ 
  rvushort    vertidx[4];

  /*! color definition for the points.
   * for a textured polygon: shading
   * for a not textured polygon: color
   */
  RVColor     color[4];

  /*! texture coordinates */
  RV_UV       texcoord[4];
};
#define RV_POLY_SIZE sizeof(RV_Poly)


#define POLY_IS_QUAD           0x001
#define POLY_IS_DOUBLE         0x002
#define POLY_IS_TRANS          0x004
#define POLY_TRANS_1_1         0x100

#define POLY_PRM_DISABLE_ENV   0x400
#define POLY_W_ENABLE_ENV      0x800












//! Structure for .w files
struct RV_FunnyBall
{
  Vector   center;
  rvfloat  radius;
  
  rvulong  groupcnt;
};
#define RV_FUNNYBALL_SIZE sizeof(RV_FunnyBall)





//! Two-dimensional subset of the 3d space
struct RV_Plane
{
  Vector  normal;
  rvfloat    distance;
};
#define RV_PLANE_SIZE sizeof(RV_Plane);



//! Convex hull
struct RV_Polyhedron
{
  rvlong   type;
  rvlong   surface;

  RV_Plane plane[5];

  AxisBox  bbox;
};
#define RV_POLYHEDRON_SIZE sizeof(RV_Polyhedron)



//! NCP optimization data
struct RV_LookupGrid
{
  rvfloat    x0;
  rvfloat    z0;

  rvfloat    x_size;
  rvfloat    z_size;

  rvfloat    raster_size;
};
#define RV_LOOKUPGRID_SIZE sizeof(RV_LookupGrid)





//! Entry struct for .fin files
struct RV_FIN_Data
{
  char fname[9];

  signed char model_r, model_g, model_b;
  unsigned char env_b, env_g, env_r;

  unsigned char _unknown_1[2];

  unsigned char enable_env_flag;

  unsigned char _unknown_2[2];

  rvlong lodbias;

  Vector position;
  Matrix_3x3 rotation;
};
#define RV_FIN_DATA_SIZE sizeof(RV_FIN_Data)




//! Entry struct for .taz files
struct RV_TAZ_Data
{
  rvulong   id;
  Vector center;
  Matrix_3x3   rotation;
  Vector   size;
};
#define RV_TAZ_DATA_SIZE sizeof(RV_TAZ_Data)




#define CAMERA_NODE 0x1
#define CUBE_NODE 0x2

//! Entry struct for .vis files
struct RV_VIS_Data  // vag 2001-05-11
{
  unsigned char flags;
  unsigned char id;
  unsigned char _unknown_[2];
  AxisBox abox;
};
#define RV_VIS_DATA_SIZE sizeof(RV_VIS_Data)


#endif
