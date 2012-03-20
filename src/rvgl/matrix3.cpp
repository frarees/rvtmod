/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2001 Gabor Varga (bootes@freemail.hu)
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

/*! \file
 * $Id: matrix3.cpp,v 1.1 2000/05/03 08:50:05 ali Exp ali $
 *
 * $Log: matrix3.cpp,v $
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "matrix3.h"


rvfloat Matrix_3x3::val(int col, int lin) const
{
  return data[col][lin];
}

void Matrix_3x3::set(int col, int lin, rvfloat val)
{
  data[col][lin]=val;
}

Matrix_3x3::Matrix_3x3(const Matrix_3x3 &mx)
{
  int i,j;
  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      data[i][j] = mx.data[i][j];
}




Matrix_3x3::Matrix_3x3(void)
{
  int i,j;

  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      set(i,j, i==j ? 1.0 : 0.0);
}

Matrix_3x3::Matrix_3x3(Vector &v1, Vector &v2, Vector &v3)
{
  int lin;

  for(lin=0;lin<3;lin++)
    {
      set(0,lin, v1[lin]);
      set(1,lin, v2[lin]);
      set(2,lin, v3[lin]);
    };
}

rvfloat Matrix_3x3::det(void) const
{
  /* Sarrus' Rule */
  return 
      val(0,0)*val(1,1)*val(2,2)
    + val(1,0)*val(2,1)*val(0,2)
    + val(2,0)*val(0,1)*val(1,2)
    - val(0,2)*val(1,1)*val(2,0)
    - val(1,2)*val(2,1)*val(0,0)
    - val(2,2)*val(0,1)*val(1,0);
}

Matrix_3x3 Matrix_3x3::operator *(Matrix_3x3 & mx)
{
  Matrix_3x3 res;
  int col, lin;

  for(col=0;col<3;col++)
    for(lin=0;lin<3;lin++)
      res.set(col,lin,
	      val(0,lin)*mx.val(col,0) + val(1,lin)*mx.val(col,1) + val(2,lin)*mx.val(col,2)
	      );

  return res;
}

Matrix_3x3 Matrix_3x3::invert(void) const
{
// Whole method has changed by vag 2001-07-03
  rvfloat d=det();
  if(fabs(d)<EPSILON)
    xpanic("Attempt to invert non-invertable matrix");

  Matrix_3x3 res;
  int i, j;

  // Adjoint method of inverting the matrix:
  // res is the matrix of cofactors transposed (lin/col exchanged), divided
  // by determinant.
  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
      res.set(i,j,cofactor(j,i)/d);

  return res;
}

bool Matrix_3x3::is_invertable(void) const
{
  return fabs(det()) > 0.00001;
}

Vector Matrix_3x3::solve(Vector &v) const
{
  Vector res;
  rvfloat mydet = det();
  int i;

  /* Cramer's rule */
  if(is_invertable())
    {

      for(i=0;i<3;i++)
	{
	  Matrix_3x3 modmx(*this);

	  // replace column i by vector v
	  modmx.set(i,0, v.x());
	  modmx.set(i,1, v.y());
	  modmx.set(i,2, v.z());

	  res[i] = modmx.det() / mydet;
	};

    };

  return res;
}

void Matrix_3x3::dump(const char *pref)
{
  int plen = strlen(pref);
  int lin, i, col;

  for(lin=0;lin<3;lin++)
    {
      if(lin==1)
	printf("%s = | ",pref);
      else
	{
	  for(i=0;i<plen;i++)
	    putchar(' ');
	  printf("   %c ", lin==0 ? '/' : '\\');
	};

      for(col=0;col<3;col++)
	{
	  printf("%7.3f",val(col,lin));
	  
	  if(col<2)
	    printf(",");
	};

      if(lin==0) printf(" \\\n");
      else if(lin==1) printf(" |\n");
      else printf(" /\n");
    };
}

Vector Matrix_3x3::operator * (Vector &v)
{
  Vector res;

  res[0]= val(0,0)*v[0] + val(1,0)*v[1] + val(2,0)*v[2];
  res[1]= val(0,1)*v[0] + val(1,1)*v[1] + val(2,1)*v[2];
  res[2]= val(0,2)*v[0] + val(1,2)*v[1] + val(2,2)*v[2];
 
  return res;
}

rvfloat Matrix_3x3::cofactor(int col, int lin) const    // vag 2001-07-03
{
  // indexes to this matrix (which give the sub-matrix excluding col/lin)
  int c0, c1, l0, l1;
  c0= col==0 ? 1 : 0;
  c1= col==2 ? 1 : 2;
  l0= lin==0 ? 1 : 0;
  l1= lin==2 ? 1 : 2;
  // determinant of sub-matrix with appropriate sign
  return (val(c0, l0) * val(c1, l1) - val(c1, l0) * val(c0, l1)) *
         ((col+lin)%2 ? -1 : 1);
}
