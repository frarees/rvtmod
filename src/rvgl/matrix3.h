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
 * $Id: matrix3.h,v 1.1 2000/05/03 08:49:53 ali Exp ali $
 *
 * $Log: matrix3.h,v $
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 */
#ifndef __MATRIX3_H
#define __MATRIX3_H

#include "rvbasetypes.h"
#include "vector.h"

//! 3x3-Matrix operations
/*!
 * Some operations for matrices. Re-Volt uses a 3x3-float-array, where
 * the matrix is encoded as column vectors.
 *
 * This class is directly written to and read from files.
 *
 * It may only contain the three coordinates, and nothing more that
 * requires storage memory, like virtual functions.
 */
class Matrix_3x3
{
public:
  Matrix_3x3();
  Matrix_3x3(const Matrix_3x3 &);
  Matrix_3x3(Vector &, Vector &, Vector &);  // vectors to columns!

  rvfloat val(int col, int lin) const;
  void set(int col, int lin, rvfloat val);


  /*!
   * Calculates determinant
   */
  rvfloat det(void) const;

  Vector operator * (Vector &);
  Matrix_3x3 operator * (Matrix_3x3 &);

  bool is_invertable(void) const;

  /*!
   * Solves M*x = v and returns x
   */
  Vector solve(Vector &v) const;

  Matrix_3x3 invert(void) const;

  /*! prints matrix to screen */
  void dump(const char *pref);

  /*!cofactor of element (col,lin): determinant of sub-matrix (excl. col/lin)*/
  rvfloat cofactor(int col, int lin) const; // vag 2001-07-03

  rvfloat data[3][3]; /* [column][line] !!!! */
};


#define MATRIX_STORAGE_SIZE_OKAY (sizeof(Matrix_3x3) == 9*sizeof(rvfloat))


#endif
