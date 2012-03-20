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
 * $Id: readprm.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: readprm.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __READPRM_H
#define __READPRM_H

#include "rvgl/mesh.h"

Mesh *create_prm_mesh(const char *filename); // fname w/o suffix!!!
Mesh *create_ip_mesh(const char *filename);  // fname w/o suffix!!!

#endif
