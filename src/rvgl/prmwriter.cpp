/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2001 Gabor Varga (vag) (bootes@freemail.hu)
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
 * $Id: prmwriter.cpp,v 1.2 2000/05/03 09:35:55 ali Exp ali $
 *
 * $Log: prmwriter.cpp,v $
 * Revision 1.2  2000/05/03 09:35:55  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "mesh.h"
#include "prmwriter.h"
#include "print.h"



PRMFileWriter::PRMFileWriter(rvulong maxsz, bool imfile)
{
  cur_size=0;
  max_size=maxsz;
  is_intermediate=imfile;
  mlist=new Mesh*[maxsz]; MEM(mlist);
  poly_cnt=0;
  vertex_cnt=0;
}

PRMFileWriter::~PRMFileWriter()
{
  rvulong i;

  for(i=0;i<cur_size;i++)
    delete mlist[i];

  delete mlist;
}


bool PRMFileWriter::addMesh(Mesh *m)
{
  Mesh *meshcopy;

  if(cur_size < max_size)
    {
      meshcopy=new Mesh(m);

      meshcopy->deleteSomePolys(false, !is_intermediate);
      meshcopy->optimize();

      if(meshcopy->isDegenerated())
	delete meshcopy;
      else
	{
	  mlist[cur_size]=meshcopy;
	  cur_size++;
	  poly_cnt+=meshcopy->data.num_polys;
	  vertex_cnt+=meshcopy->data.num_vecs;
	};

      return true;
    }
  else
    return false;
}

int PRMFileWriter::write(const char *fname)
{
  FILE *f;
  rvulong i;
  rvulong zero16[4] = { 0,0,0,0 };
  CollapsedMesh cmesh(poly_cnt, vertex_cnt);

  f=fopen(fname,"wb");
  if(f==NULL)
    return errno;

  if(is_intermediate)
    fwrite(zero16, 16, 1, f);

  for(i=0;i<cur_size;i++)
    cmesh.add(mlist[i]);

  cmesh.optimize();
  // Commented out by vag 2001-06-28
  // Centering can be done in World now.
  //cmesh.set_grip();
  cmesh.write_prm(f, is_intermediate);

  fclose(f);
  return 0;
}

