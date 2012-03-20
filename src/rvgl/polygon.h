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
 * $Id: polygon.h,v 1.2 2000/05/03 09:19:57 ali Exp ali $
 *
 * $Log: polygon.h,v $
 * Revision 1.2  2000/05/03 09:19:57  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 */
#ifndef __POLYGON_H
#define __POLYGON_H

#include "rvtypes.h"


#define TEP_PWALL      1
#define TEP_RWALL      2
#define TEP_CARIMG     4
#define TEP_FLOOR      8
#define TEP_FLOOR2    16  // ces 2000-04-13
#define TEP_LAMP      32
#define TEP_BOX       64
#define TEP_TRACK    128
#define TEP_PIPE     256  // ces 2000-04-13
#define TEP_BCEIL    512  // ces 2000-04-13
#define TEP_BRAIL   1024  // ces 2000-04-13


#define SURF_NOCOLL   -2
#define TEX_INVISIBLE -2

class Mesh;

/*! \enum EnvMapType 
 * gives the different representations of env mapping in .w and .prm files
 * a common access.
 */
enum EnvMapType
{

  Enabled,  /*!< \arg For .w: Set enable-env bit. \arg For .prm: don't set disable-env bit */
  Default,  /*!< \arg For .w: Don't set enable-env bit. \arg For .prm: don't set disable-env bit */
  Disabled  /*!< \arg For .w: Don't set enable-env bit. \arg For .prm: Set disable-env bit */
};


//! Simply a polygon-class
class Polygon
{
public:
  friend class PolygonModifier;

public:
  Polygon(Mesh *p, RV_Poly *d);  // ces 2000-04-13

  Polygon(Mesh *, FILE *, bool is_IM, bool is_Instance);

  /*!
   * ctor.
   *
   * creates an exact copy. Only difference is the parent pointer.
   */
  Polygon(Mesh *, Polygon *);


  /*! sets the transparency for the whole polygon, and sets the bits for
   * alpha-channel based transparency
   *
   * If argument is 255, disables transparency and clears the transparency-bits
   */
  void setTransparency(rvulong);
  /*! sets the surface type for the polygon */
  void setSurface(rvlong);

  void turnInvisible(void);
  bool isInvisible(void);
  void setNoColl(void);
  bool isNoColl(void);

  /*! number of vertices for this polygon
   *
   * \return
   * \arg 3 if poly is a triangle
   * \arg 4 if poly is a quad
   */
  int numverts(void);

  void setTEColor(rvulong);

  void write(FILE *, bool is_IM, bool is_Instance);

  /*! obsolete */
  int detTEPart(void);

  void shiftVIndices(rvushort);

  bool isInTexture(rvshort page, rvfloat ulo, rvfloat uhi, rvfloat vlo, rvfloat vhi);

  /*! returns the vertex index indicated by the argument.
   *
   * advantage is that it "wraps over": For a triangle, vertidx(0), vertidx(3) and
   * vertidx(-3) return all the same
   */
  int vertidx(int);

  Mesh *parent;




private:
  /*! hack: heuristic to detect whether the poly belongs to the lego part "blue wall". Simply calls isInTexture(). \internal */
  bool teheur_BlueWall(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "ceiling lamps".  Simply calls isInTexture().\internal */
  bool teheur_Lamp(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "green floor".  Simply calls isInTexture().\internal */
  bool teheur_Floor(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "outside floor".  Simply calls isInTexture().\internal */
  bool teheur_Floor2(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "room wall".  Simply calls isInTexture().\internal */
  bool teheur_RWall(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "box pile".  Simply calls isInTexture().\internal */
  bool teheur_Box(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "car images".  Simply calls isInTexture().\internal */
  bool teheur_CarImage(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "pipe end".  Simply calls isInTexture().\internal */
  bool teheur_Pipe(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "bridge ceiling".  Simply calls isInTexture().\internal */
  bool teheur_BCeil(void);
  /*! hack: heuristic to detect whether the poly belongs to the lego part "bridge railing".  Simply calls isInTexture().\internal */
  bool teheur_BRail(void);

public:
  /*! surface type as written to the .ncp file, or \c SURF_NOCOLL for non-solid polys */
  rvlong surface;
  /*! extra bits to include in the .ncp file */
  rvlong ncp_type;

  EnvMapType envmap;
  RVColor env_color;
  RV_Poly data;
  /*! lower collision data, if this is a vertical axis-aligned quad poly */
  rvfloat lower_coll;   // vag 2001-06-28
};


#endif
