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
 * $Id: surface.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: surface.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 *
 *
 */
#ifndef __SURFACE_H
#define __SURFACE_H

#include "rvtypes.h"
#include "lookup.h"


//! class for RV_Polyhedron
/*!
 * NCP Grid entry method is controlled by external define CHEAP_NCP_GRID
 */
class Surface
{
public:
  Surface(rvulong stype, rvulong type,
	  Vector *v0, Vector *v1, Vector *v2, Vector *v3=NULL);

  void createPolyhedron(rvulong);
  Vector planarNormal(void);
  void createPlane(RV_Plane *, Vector &normal, Vector &pos);

  void write(FILE *);

  void expandGrid(LookupGrid*);

  /*! requires that createPolyhedron() has been called before */
  void enterIntoGrid(LookupGrid*, rvulong myidx);

  bool isInGrid(LookupGrid*, int, int);

private:
  RV_Polyhedron ph;
  Vector vecs[4];
  Vector orient_vecs[4];
  bool   orient_is_waste[4];
  rvfloat orient_distance[4];
  int nvecs;
};

#endif
