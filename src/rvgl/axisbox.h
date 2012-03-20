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
 * $Id$
 *
 * $Log$
 */
#ifndef __AXIS_BOX_H
#define __AXIS_BOX_H

#include "rvbasetypes.h"
#include "vector.h"

//! Axis-parallel box
/*!
 * This class is directly written to and read from files.
 *
 * It may only contain the three coordinates, and nothing more that
 * requires storage memory, like virtual functions.
 */
class AxisBox
{
public:
  /*!
   * ctor. Creates an invalid box ready to get stuff fed into it
   */
  AxisBox();

  /*!
   * Returns \t true, if the box is not empty.
   */
  bool is_valid(void);

  void fit_into(Vector &);

  void be_empty(void);

  /*!
   * enlarges the box a small bit in every direction.
   */
  void add_safety(void);

  rvfloat  xlo, xhi;
  rvfloat  ylo, yhi;
  rvfloat  zlo, zhi;
};

#define AXISBOX_SIZE sizeof(AxisBox)
#define AXISBOX_STORAGE_SIZE_OKAY (sizeof(AxisBox) == 6*sizeof(rvfloat))


#endif
