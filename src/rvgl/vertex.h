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
 * $Id: vertex.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: vertex.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 */
#ifndef __VERTEX_H
#define __VERTEX_H

#include "rvbasetypes.h"
#include "vector.h"
#include "matrix3.h"

//! Vector with normal
/*!
 * This class is directly written to and read from files.
 *
 * It may only contain the three coordinates, and nothing more that
 * requires storage memory, like virtual functions.
 */
class Vertex
{
public:
  Vertex();  // empty, x=y=z=0
  Vertex(FILE *);        // read from binary file
  Vertex(Vertex *);
  Vertex(const Vector &_pos, const Vector &_normal);

  void translate(rvfloat dx, rvfloat dy, rvfloat dz);
  void translate(Vector *vec);
  void rotate(Matrix_3x3 &);

  void write(FILE *);

  Vector pos;
  Vector normal;
};

#define VERTEX_SIZE sizeof(Vertex)
#define VERTEX_STORAGE_SIZE_OKAY (sizeof(Vertex) == 2*VECTOR_SIZE)

#endif
