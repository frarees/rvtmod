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
 * $Id: wf.cpp,v 1.2 2000/05/03 09:56:46 ali Exp ali $
 *
 * $Log: wf.cpp,v $
 * Revision 1.2  2000/05/03 09:56:46  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"
#include "rvgl/helpfunc.h"

#include "wf.h"
#include "errdesc.h"
#include "wmodlist.h"
#include "appopts.h"




//! Mini-Modifier for .w files
/*!
 * Reads env-mapping list from .w files
 *
 * Does not modify anything else
 */
class WFile_Env_Reader : public PolygonModifier
{
public:
  WFile_Env_Reader(FILE *_f) { f=_f; };

  virtual bool apply(Polygon *);

private:
  FILE *f;
};

bool WFile_Env_Reader::apply(Polygon *poly)
{
  if((poly->envmap == Enabled) && (!poly->isInvisible()))
    fread(&poly->env_color, sizeof(rvulong),1,f);

  return true;
}









int process_w_file(const char *bfname, W_ModList &mlist, World *world)
{
  FILE *f;
  rvlong meshcnt, count, i;
  RV_W_Mesh wmesh;
  RV_FunnyBall fball;
  Mesh **meshl;
  WFile_Env_Reader *envreader;
  int lastp=-1;
  bool doreadenv;
  char fname[100], fsname[100];
  bool reading_im;
  char i_head[16];

  strcpy(fname,bfname);
  breakfname(fname);

  if((strcasecmp(fname, appopt.baseoutname)==0) && (!appopt.create_instance))
    {
      perror(0, ERROR_W_IN_AND_OUT,
	     "Attempt to read and write same file (%s.w)",
	     fname);
      return 100;
    };


  strcpy(fsname, fname); strcat(fsname, SUFFIX_DOT_W_IM);
  f=fopen(fsname,"rb");
  if(f!=NULL)
    reading_im=true;
  else
    {
      strcpy(fsname, fname); strcat(fsname, SUFFIX_DOT_W_STD);
      f=fopen(fsname,"rb");
      if(f!=NULL)
	reading_im=false;
      else
	{
	  perror(0, ERROR_FILE_ROPEN, "Cannot open `%s.%s' or '%s.%s'",
		 fname, SUFFIX_W_IM, fname, SUFFIX_W_STD);
	  return 100;
	};
    };


  if(reading_im)
    {
      fread(i_head,16,1,f);
      for(i=0;i<16;i++)
	if(i_head[i] != 0)
	  {
	    perror(0,ERROR_IM_HEADER, "Intermediate file '%s' has malformatted header - may be corrupt\n",
		   fsname);
	    break;
	  };
    };

  fread(&meshcnt,sizeof(rvlong),1,f);

  lprintf(1,"Reading `%s': %d meshes - step 1 of 2\n",fsname,meshcnt);
  meshl = new Mesh*[meshcnt]; MEM(meshl);

  for(i=0;i<meshcnt;i++)
    {
      showPercent(2,i,meshcnt,lastp);

      fread(&wmesh, RV_W_MESH_SIZE,1,f);
      meshl[i]=new Mesh(&wmesh,f,reading_im);
    };
  doneShowPercent(2);




  fread(&count,sizeof(rvlong),1,f);
  lprintf(3,"Skipping %d funnyballs\n",count);
  for(i=0;i<count;i++)
    {
      fread(&fball, RV_FUNNYBALL_SIZE, 1, f);
      fseek(f,sizeof(rvlong)*fball.groupcnt,SEEK_CUR);
    };




  if(fread(&count,sizeof(rvlong),1,f) < 1)
    {
      lprintf(2,"File ended before env mapping list - using defaults\n");
      doreadenv=false;
    }
  else if(count != 0)
    {
      doreadenv=false;
      perror(1, ERROR_W_ENV_NOT_FOUND, "Can't find env mapping values - using defaults\n");
      lprintf(3,"Strange count is %d (0x%x)\n",count,count);
    }
  else
    doreadenv=true;





  lprintf(1,"Reading `%s': step 2 of 2\n",fname);
  lastp=-1;
  envreader=new WFile_Env_Reader(f);
  for(i=0;i<meshcnt;i++)
    {
      showPercent(2,i,meshcnt,lastp);

      if(doreadenv)
	meshl[i]->apply(envreader);


      if(!mlist.apply_MeshOperators(meshl[i]))
	 {
	  delete envreader;
	  fclose(f);
	  return 1;
	};

      if(!meshl[i]->apply(&mlist))
	{
	  delete envreader;
	  fclose(f);
	  return 1;
	};
      world->addMesh(meshl[i]);
    };
  doneShowPercent(2);
  delete envreader;

  delete meshl;

  fclose(f);
  return 0;
}
