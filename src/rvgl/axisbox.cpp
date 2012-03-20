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
#include <stdlib.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvbasetypes.h"
#include "vector.h"
#include "axisbox.h"

AxisBox::AxisBox(void)
{
  be_empty();
}
 
void AxisBox::be_empty(void)
{
  xlo=ylo=zlo= +1e40;
  xhi=yhi=zhi= -1e40;
}

bool AxisBox::is_valid(void)
{
  return (xlo <= xhi) && (ylo <= yhi) && (zlo <= zhi);
}

void AxisBox::fit_into(Vector &v)
{
  if(v.x() < xlo) xlo=v.x();
  if(v.y() < ylo) ylo=v.y();
  if(v.z() < zlo) zlo=v.z();
  if(v.x() > xhi) xhi=v.x();
  if(v.y() > yhi) yhi=v.y();
  if(v.z() > zhi) zhi=v.z();
}

void AxisBox::add_safety(void)
{
  xlo -= 1.0;
  ylo -= 1.0;
  zlo -= 1.0;
  xhi += 1.0;
  yhi += 1.0;
  zhi += 1.0;
}
