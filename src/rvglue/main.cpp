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
 * $Id: main.cpp,v 1.1 2000/05/03 08:49:37 ali Exp ali $
 *
 * $Log: main.cpp,v $
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"
#include "rvgl/world.h"
#include "rvgl/matrix3.h"
#include "input.h"
#include "appopts.h"
#include "errdesc.h"

#define APPNAME "rvglue"

void inform_comp_error(const char *type_name)
{
  printf("        *** COMPILATION ERROR ***\n\n");
  printf("This copy of rvglue isn't functional at all, since somebody\n");
  printf("messed it up while compiling it.\n\n");
  printf("Recompile program for a 32-bit environment, or update\n");
  printf("the basic types in lib\\rvbasetypes.h to fit your environment!\n\n");
  printf("At least the data type '%s' has a wrong size\n",type_name);
  exit(255);
}

void check_env(void)
{
  if(sizeof(rvfloat)!=4) inform_comp_error("rvfloat");
  if(sizeof(rvshort)!=2) inform_comp_error("rvshort");
  if(sizeof(rvlong)!=4) inform_comp_error("rvlong");
  if(!COLOR_STORAGE_SIZE_OKAY) inform_comp_error("class RVColor");
  if(!VECTOR_STORAGE_SIZE_OKAY) inform_comp_error("class Vector");
  if(!VERTEX_STORAGE_SIZE_OKAY) inform_comp_error("class Vertex");
  if(!MATRIX_STORAGE_SIZE_OKAY) inform_comp_error("class Matrix_3x3");
  if(!AXISBOX_STORAGE_SIZE_OKAY) inform_comp_error("class AxisBox");
}



void welcome(void)
{
  /* 80:     123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
  lprintf(1," ------------------------------------------------------------------------------\n");
  lprintf(1,"| %s version %s,\n",APPNAME, APPVERSION);
  lprintf(1,"| Copyright (C) 2000-2001 Alexander Kroeller, Christer Sundin and Gabor Varga.\n");
  lprintf(1,"| %s is free software, you can redistribute it under certain conditions.\n", APPNAME);
  lprintf(1,"| %s comes with ABSOLUTELY NO WARRANTY; for details see 'doc\\GPL.html'.\n", APPNAME);
  lprintf(1," ------------------------------------------------------------------------------\n");
}


int main(int argc, char **argv)
{
  int res;
  World *world;


  check_env();

  if(!appopt.parseArgs(argc,argv))
    return 100;

  if(appopt.inputfname==NULL)
    {
      perror(0,ERROR_NO_IFILE,"No input file specified.");
      return 100;
    };

  welcome();

  world=new World;

  res=processInput(appopt.inputfname, world);

  if(res == 0)
    {
      if(appopt.create_instance)
        {
          world->set_grip();  // vag 2001-06-28
	  world->write_prm(appopt.woutname,appopt.ioutname,appopt.ncpoutname);
        }
      else
	world->write_w(appopt.woutname,appopt.ioutname,appopt.ncpoutname);
    };

  delete world;

  return res;
}




void dvec(Vector &v) { printf("(%g,%g,%g)",v[0],v[1],v[2]);}
int notmain(void)
{
  int i,j,k;
  Matrix_3x3 id;
  Matrix_3x3 invid;
  Vector vec(1,2,3);
  Vector res, res2;

  for(i=0;i<3;i++) for(j=0;j<3;j++) id.set(i,j,(float)((i*4)+j));

  for(k=0;k<100;k++)
    {
      for(i=0;i<3;i++) //col
	for(j=0;j<3;j++) //lin
	  {
	    /*	    if(i<j)
	      id.set(i,j,0.0);
	      else*/
	      id.set(i,j, (float)(  (((i*j*k)%7 + ((k+1)%3) + (i+j))+1) * (((k+i+j)%2)==0 ? -1 : 1) ));
	  };
      printf("\n\nTEST #%d\n",k);
      if(id.is_invertable())
	{
	  printf("Det==%g\n",id.det());
	  res=Vector(1.0,1.0,1.0);
	  res=id.solve(res);
	  printf("solve erg = %g,%g,%g\n",res[0],res[1],res[2]);
	  res2=id*res;
	  printf("solve test = %g,%g,%g\n",res2[0],res2[1],res2[2]);

	  id.dump("   M");
	  invid=id.invert();
	  invid.dump("M^-1");
	  
	  (id*invid).dump("mul");
	};
    };


  id.dump("id");

  printf("vec = ");
  dvec(vec);
  printf("\n");

  res=id*vec;

  printf("res = ");
  dvec(res);
  printf("\n");

  return 0;
}
