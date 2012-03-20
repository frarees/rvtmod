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
 * $Id: wwriter.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: wwriter.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 *
 *
 */
#ifndef __W_FILE_WRITER_H
#define __W_FILE_WRITER_H

#include "mesh.h"

//! Writer for .w files
class WFileWriter
{
public:
  WFileWriter(rvulong maxsz, bool imfile);
  ~WFileWriter();

  bool addMesh(Mesh *);
  int write(const char *fname);

private:
  void putFunnyBall(FILE *);

  Mesh **mlist;
  rvulong cur_size, max_size;
  bool is_intermediate;
};

#endif
