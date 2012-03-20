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
 * $Id: vector.h,v 1.2 2000/05/03 09:28:36 ali Exp ali $
 *
 * $Log: vector.h,v $
 * Revision 1.2  2000/05/03 09:28:36  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 */
#ifndef __VECTOR_H
#define __VECTOR_H

#include "rvbasetypes.h"


//! mathematical 3d-vector
/*!
 * This class is directly written to and read from files.
 *
 * It may only contain the three coordinates, and nothing more that
 * requires storage memory, like virtual functions.
 */
class Vector
{
public:
  Vector();
  
  /*! ctor: copy from other vector */
  Vector(const Vector &);

  /*! ctor: be the normal vector to those */
  Vector(Vector &, Vector &);

  /*! ctor:  spec. x,y,z */
  Vector(rvfloat x, rvfloat y, rvfloat z);

  Vector operator - (const Vector &);
  Vector operator + (const Vector &);

  /*! Is a x1<x2 && y1<y2 && z1<z2 */
  bool operator <= (const Vector &) const;

  /*! scales a vector by \a scalefac */
  Vector operator * (rvfloat scalefac);

  /*! scales a vector by \a scalediv */
  Vector operator / (rvfloat scalediv);  // ces 2000-04-13

  /*! cross product (result is \e a (not \e the) normal vector to
    this and the argument vector) */
  Vector operator * (Vector &);

  /*! equal op. Does return \c true for close vectors, too */
  bool   operator == (const Vector &) const;
  bool   operator != (const Vector &) const;

  rvfloat length(void) const;
  rvfloat lengthXZ(void) const;  // ces 2000-04-13

  /*! tries to rescale a vector to have length 1.0
   *
   * \return
   * \c true if successfull
   * \c false if vector is too small for rescaling
   */
  bool  normalize(void);

  bool  normalizeXZ(void);

  /*! scalar product */
  rvfloat scalarprod(const Vector &) const;

  /*! scalar product ignoring Y */
  rvfloat scalarprod_xz(const Vector &) const;

  /*! rotates the vector by 90 degrees in the X-Z-plane */
  void rotate_right_xz(void);

  bool isnull() const {return length() < EPSILON;}    // vag 2001-05-08

  /*! true if perpendicular to other vector (nullvector is always!) */
  bool perp(const Vector &v) const {return scalarprod(v) < EPSILON;}

  /*! true if parallel and unidirectional to other vector (never to nullvector)*/
  bool unidir(const Vector &v) const;   // vag 2001-05-08

  /*! true if v1,v2 and v3 (in this order) form a right-handed perpendicular triplet */
  static bool right_perp_triplet(Vector &v1, Vector &v2, Vector &v3);   // vag 2001-05-08

  rvfloat &x(void); 
  rvfloat &y(void);
  rvfloat &z(void);
  rvfloat x(void) const;    // vag 2001-05-08
  rvfloat y(void) const;    // vag 2001-05-08
  rvfloat z(void) const;    // vag 2001-05-08
  rvfloat x(rvfloat);
  rvfloat y(rvfloat);
  rvfloat z(rvfloat);

  inline rvfloat &operator [] (int i) { return coord[i]; }

  rvfloat coord[3];
};

#define VECTOR_SIZE sizeof(Vector)
#define VECTOR_STORAGE_SIZE_OKAY (sizeof(Vector) == 3*sizeof(rvfloat))

#endif
