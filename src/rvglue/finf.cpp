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
 * $Id: finf.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: finf.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"
#include "rvgl/print.h"
#include "rvgl/helpfunc.h"

#include "finf.h"
#include "errdesc.h"
#include "readprm.h"
#include "findfile.h"
#include "appopts.h"



//! Mini-Modifier for instances
/*!
 * Applies .fin data to polygons.
 *
 * Does not modify position and rotation
 */
class FIN_Poly_Mod : public PolygonModifier
{
public:
  FIN_Poly_Mod(RV_FIN_Data *_fd) { fd=_fd; };

  virtual bool apply(Polygon *);

private:
  RV_FIN_Data *fd;
};


bool FIN_Poly_Mod::apply(Polygon *poly)
{
  int i;

  for(i=0;i<poly->numverts();i++)
    poly->data.color[i].shadeAdd( (int)(fd->model_r), (int)(fd->model_g), (int)(fd->model_b) );

  if(fd->enable_env_flag & 1)
    {
      poly->envmap=Enabled;
      poly->env_color.setRed  ( (rvulong)(fd->env_r) );
      poly->env_color.setGreen( (rvulong)(fd->env_g) );
      poly->env_color.setBlue ( (rvulong)(fd->env_b) );
    };
  return true;
}




void modifyMesh(Mesh *mesh, RV_FIN_Data *fdata)
{
  mesh->rotate(fdata->rotation);
  mesh->translate(fdata->position.coord[0], fdata->position.coord[1],fdata->position.coord[2]);
}





bool insertInstance(RV_FIN_Data *fd, OptionSetKeeper &keeper, World *world)
{
  Mesh *mesh;
  ModList *mlist;
  FIN_Poly_Mod pmod(fd);
  char *usename;
  int nfound;


  nfound=find_file(fd->fname, true, &usename, SUFFIX_PRM_IM, SUFFIX_PRM_STD, NULL);
  if(nfound == 0)
    {
      perror(0, ERROR_FILE_ROPEN, "Cannot open '%s*.%s' or '%s*.%s'",
	     fd->fname, SUFFIX_PRM_IM, fd->fname, SUFFIX_PRM_STD);
      return false;
    };

  if(nfound > 1)
    {
      perror(0, ERROR_AMBIGUOUS_NAME, "Too much possibilities for '%s'",
	     usename);
      free(usename);
      return false;
    };
  
  
  if(filehastype(usename,SUFFIX_PRM_IM))
    mesh=create_ip_mesh(usename);
  else
    mesh=create_prm_mesh(usename);

  if(mesh != NULL)
    lprintf(2,"  insert '%s'\n",usename);
  else
    {
      perror(0, ERROR_FILE_ROPEN, "Cannot open '%s'", usename);
      free(usename);
      return false;
    };

  breakfname(usename);

  modifyMesh(mesh,fd);
  mesh->apply(&pmod);

  mlist=keeper.lookup(usename);
  //  printf("MESH %s\n",usename);
  //  mlist->dump("  ");

  mesh->apply(mlist);

  world->addMesh(mesh);

  return true;
}



int process_fin_file(const char *fname, OptionSetKeeper &keeper, World *world)
{
  FILE *f;
  rvlong count, i;
  RV_FIN_Data fdata;
  char *fn_base;
  
  fn_base=strdup(fname); MEM(fn_base);
  breakfname(fn_base);
  if((strcasecmp(fn_base,appopt.baseoutname)==0)&&(!appopt.create_instance))
    perror(1, ERROR_FIN_MATCHES_W,
	   "%s may cause unexpected results if you don't know exactly what you're doing",
	   fname);
  free(fn_base);

  f=fopen(fname,"rb");
  if(f==NULL)
    {
      perror(0, ERROR_FILE_ROPEN, "Cannot open `%s': %s",fname,strerror(errno));
      return 100;
    };

  lprintf(1,"Reading `%s'\n",fname);
  fread(&count,sizeof(rvlong),1,f);

  for(i=0;i<count;i++)
    {
      fread(&fdata, RV_FIN_DATA_SIZE,1,f);
      if(!insertInstance(&fdata,keeper,world))
	{
	  fclose(f);
	  return 100;
	};
    };
  fclose(f);
  return 0;
}
