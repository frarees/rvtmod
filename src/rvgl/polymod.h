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
 * $Id: polymod.h,v 1.1 2000/05/03 08:49:53 ali Exp ali $
 *
 * $Log: polymod.h,v $
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 *
 *
 */
#ifndef __POLYGON_MODIFIER_H
#define __POLYGON_MODIFIER_H

#include "polygon.h"


//! abstract superclass
class PolygonModifier
{
public:

  /*! polygon modifying function
   *
   * abstract prototype
   */
  virtual bool apply(Polygon*) hook;

};

#endif
