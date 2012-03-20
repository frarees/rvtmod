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
 * $Id: wwriter.cpp,v 1.2 2000/05/03 09:37:45 ali Exp ali $
 *
 * $Log: wwriter.cpp,v $
 * Revision 1.2  2000/05/03 09:37:45  ali
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
#include "wwriter.h"
#include "print.h"



//! Mini-Modifier for .w files
/*!
 * Writes env-mapping list to .w files
 *
 * Does not modify anything else
 */
class WFile_Env_Writer : public PolygonModifier
{
public:
  WFile_Env_Writer(FILE *_f) { f=_f; };

  virtual bool apply(Polygon *);

private:
  FILE *f;
};

bool WFile_Env_Writer::apply(Polygon *poly)
{
  if(poly->envmap == Enabled)
    fwrite(&poly->env_color, sizeof(rvulong),1,f);
  return true;
}









WFileWriter::WFileWriter(rvulong maxsz, bool imfile)
{
  cur_size=0;
  max_size=maxsz;
  is_intermediate=imfile;
  mlist=new Mesh*[maxsz]; MEM(mlist);
}

WFileWriter::~WFileWriter()
{
  rvulong i;

  for(i=0;i<cur_size;i++)
    delete mlist[i];

  delete mlist;
}


bool WFileWriter::addMesh(Mesh *m)
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
	};

      return true;
    }
  else
    return false;
}

void WFileWriter::putFunnyBall(FILE *f)
{
  /* THIS IS VERY ANNOYING: JUST ONE FUNNYBALL. */
  rvfloat fl;
  rvulong ul;

  // One ball
  ul=1; fwrite(&ul,sizeof(rvulong),1,f);

  // Center <0,0,0>
  fl=0.0; fwrite(&fl,sizeof(rvfloat),1,f);
  fl=0.0; fwrite(&fl,sizeof(rvfloat),1,f);
  fl=0.0; fwrite(&fl,sizeof(rvfloat),1,f);

  // Radius 100'000 should be enough
  fl=100000.0; fwrite(&fl,sizeof(rvfloat),1,f);
 

  // And everything inside it:
  fwrite(&cur_size,sizeof(rvulong),1,f);

  for(ul=0;ul<cur_size;ul++)
    fwrite(&ul,sizeof(rvulong),1,f);

}

int WFileWriter::write(const char *fname)
{
  FILE *f;
  rvulong i;
  rvulong zero16[4] = { 0,0,0,0 };
  int last;

  f=fopen(fname,"wb");
  if(f==NULL)
    return errno;

  if(is_intermediate)
    fwrite(zero16, 16, 1, f);

  fwrite(&cur_size, sizeof(rvulong),1,f);
  last=-1;
  for(i=0;i<cur_size;i++)
    {
      mlist[i]->write_w(f, is_intermediate);
      if(i%20==0)
	showPercent(2,i,cur_size,last);
    };
  doneShowPercent(2);

  putFunnyBall(f);

  fwrite(zero16, sizeof(rvulong), 1, f);

  WFile_Env_Writer envbabe(f);

  for(i=0;i<cur_size;i++)
    mlist[i]->apply(&envbabe);


  fclose(f);
  return 0;
}

