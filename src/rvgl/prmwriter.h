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
 * $Id: prmwriter.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: prmwriter.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 *
 *
 */
#ifndef __PRM_FILE_WRITER_H
#define __PRM_FILE_WRITER_H

#include "mesh.h"
#include "collapsedmesh.h"

//! Writer for .prm files
class PRMFileWriter
{
public:
  PRMFileWriter(rvulong maxsz, bool imfile);
  ~PRMFileWriter();

  bool addMesh(Mesh *);
  int write(const char *fname);

private:
  Mesh **mlist;
  rvulong cur_size, max_size;
  rvulong poly_cnt, vertex_cnt;
  bool is_intermediate;
};

#endif
