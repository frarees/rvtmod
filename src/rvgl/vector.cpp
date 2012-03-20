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
 * $Id: vector.cpp,v 1.2 2000/05/03 09:30:34 ali Exp ali $
 *
 * $Log: vector.cpp,v $
 * Revision 1.2  2000/05/03 09:30:34  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "vector.h"
#include "rvbasetypes.h"

Vector::Vector(Vector &v1, Vector &v2)
{
  *this = v1 * v2;
  normalize();
}

bool   Vector::operator == (const Vector &p) const
{
  return FEQUAL(x(),p.x()) && FEQUAL(y(),p.y()) && FEQUAL(z(),p.z());
}

bool   Vector::operator != (const Vector &p) const
{
  return (!FEQUAL(x(),p.x())) || (!FEQUAL(y(),p.y())) || (!FEQUAL(z(),p.z()));
}

void Vector::rotate_right_xz(void)
{
  rvfloat h=x();

  x(z());
  z(-h);
}

rvfloat Vector::scalarprod(const Vector &v) const
{
  return x()*v.x() + y()*v.y() + z()*v.z();
}

rvfloat Vector::scalarprod_xz(const Vector &v) const
{
  return x()*v.x() + z()*v.z();
}

Vector Vector::operator - (const Vector &p)
{
  return Vector( x()-p.x(), y()-p.y(), z()-p.z() );
}

Vector Vector::operator + (const Vector &p)
{
  return Vector( x()+p.x(), y()+p.y(), z()+p.z() );
}

rvfloat &Vector::x(void)
  { return coord[0]; }

rvfloat &Vector::y(void)
  { return coord[1]; }

rvfloat &Vector::z(void)
  { return coord[2]; }

rvfloat Vector::x(void) const
  { return coord[0]; }

rvfloat Vector::y(void) const
  { return coord[1]; }

rvfloat Vector::z(void) const
  { return coord[2]; }

rvfloat Vector::x(rvfloat f)
  { return x()=f; }

rvfloat Vector::y(rvfloat f)
  { return y()=f; }

rvfloat Vector::z(rvfloat f)
  { return z()=f; }




Vector::Vector()
{
  coord[0]=coord[1]=coord[2]=0.0;
}
  
Vector::Vector(rvfloat _x, rvfloat _y, rvfloat _z)
{
  x(_x); y(_y); z(_z); 
}

bool Vector::operator <= (const Vector &p) const
{
  return (fabs(x())<=p.x()) && (fabs(y())<=p.y()) && (fabs(z())<=p.z());
}

Vector::Vector(const Vector &p)
{
  x(p.coord[0]);
  y(p.coord[1]);
  z(p.coord[2]);
}

rvfloat Vector::length(void) const
{
  return sqrt( SQR(x()) + SQR(y()) + SQR(z()) );
}

rvfloat Vector::lengthXZ(void) const  // ces 2000-04-13
{
  return sqrt( SQR(x()) + SQR(z()) );
}

bool Vector::normalize(void)
{
  rvfloat lgt=length();
  
  if(lgt < EPSILON)
    return false;
  x() /= lgt;
  y() /= lgt;
  z() /= lgt;
  return true;
}

bool Vector::normalizeXZ(void)
{
  rvfloat lgt=lengthXZ();
  
  if(lgt < EPSILON)
    return false;
  x() /= lgt;
  z() /= lgt;
  return true;
}

Vector Vector::operator * (rvfloat scalefac)
{
  return Vector(x()*scalefac, y()*scalefac, z()*scalefac );
}

Vector Vector::operator / (rvfloat scalediv)  // ces 2000-04-13
{
  return Vector(x()/scalediv, y()/scalediv, z()/scalediv );
}

Vector Vector::operator * (Vector &p)
{
  return Vector(
		y()*p.z() - z()*p.y(),
		z()*p.x() - x()*p.z(),
		x()*p.y() - y()*p.x() );
}


bool Vector::unidir(const Vector &v) const  // vag 2001-05-08
{
  Vector vv(v);
  Vector vt(*this);
  if(vv.normalize() && vt.normalize() && vv==vt)
    return true;
  return false;
}



bool Vector::right_perp_triplet(Vector &v1, Vector &v2, Vector &v3) // vag 2001-05-08
{
  return
    !v1.isnull() && !v2.isnull() && !v3.isnull() &&
    v1.perp(v2) && v1.perp(v3) && v2.perp(v3) &&
    v1.unidir(v2*v3);
}

