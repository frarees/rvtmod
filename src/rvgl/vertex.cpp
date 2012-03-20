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

/*! \file
 * $Id: vertex.cpp,v 1.2 2000/05/03 09:25:06 ali Exp ali $
 *
 * $Log: vertex.cpp,v $
 * Revision 1.2  2000/05/03 09:25:06  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "vertex.h"


Vertex::Vertex(const Vector &_pos, const Vector &_normal)
{
  pos    = _pos;
  normal = _normal;
}

Vertex::Vertex(Vertex *other)
{
  pos    = other->pos;
  normal = other->normal;
}

Vertex::Vertex(FILE *f)
{
  fread(&pos,VECTOR_SIZE,1,f);
  fread(&normal,VECTOR_SIZE,1,f);
}

void Vertex::translate(rvfloat dx, rvfloat dy, rvfloat dz)
{
  pos[0] += dx;
  pos[1] += dy;
  pos[2] += dz;
}

void Vertex::translate(Vector *vec)
{
  pos[0] += vec->x();
  pos[1] += vec->y();
  pos[2] += vec->z();
}



void smxmult(rvfloat coord[3], rvfloat mx[3][3])
{
  rvfloat x,y,z;

  /* small matrix multiplication. No for() loop to avoid numerical errors. */
  x= mx[0][0]*coord[0] + mx[1][0]*coord[1] + mx[2][0]*coord[2];
  y= mx[0][1]*coord[0] + mx[1][1]*coord[1] + mx[2][1]*coord[2];
  z= mx[0][2]*coord[0] + mx[1][2]*coord[1] + mx[2][2]*coord[2];
 
  coord[0]=x;
  coord[1]=y;
  coord[2]=z;
}

void Vertex::rotate(Matrix_3x3 &mx)
{
  /*  smxmult(pos.coord, mx);
      smxmult(normal.coord, mx);*/
  pos = mx*pos;
  normal=mx*normal;
}

void Vertex::write(FILE *f)
{
  fwrite(&pos, VECTOR_SIZE,1,f);
  fwrite(&normal, VECTOR_SIZE,1,f);
}

