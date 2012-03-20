/*
 *  Part of rvglue source code,
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

#ifndef __BOX_H
#define __BOX_H

#include "rvtypes.h"

//! A box (center, edge half sizes and rotation matrix) plus main/sub ids
struct Box
{
  /*!
   * enlarges the box a small bit in every direction.
   */
  void add_safety(rvfloat dist);
  /*!
   * true if the box is parellel to main coord axes.
   */
  bool axis_para(void);

  rvulong id;
  unsigned char flags;
  Vector center;
  Matrix_3x3 rotation;
  Vector size;
};

//! List of boxes
class BoxList
{
public:
  BoxList();
  ~BoxList();

  void add(Box *box);   // ownership taken
  int write_taz(const char *fname, rvfloat grow=0.0);
  int write_vis(const char *fname, rvfloat cameraGrow=0.0, rvfloat cubeGrow=0.0);

private:

  void checkSanity(bool vis);
  void reset();
  void grow();

private:

  int cnt;
  int max;
  Box **list;   // owned
};

#endif
