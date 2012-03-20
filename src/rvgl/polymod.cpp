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
 * $Id: polymod.cpp,v 1.1 2000/05/03 08:50:05 ali Exp ali $
 *
 * $Log: polymod.cpp,v $
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include "polymod.h"
#include "print.h"

bool PolygonModifier::apply(Polygon *p)
{
  xpanic("POLYMOD::APPLY() for abstract base class\n");
  return false;
}
