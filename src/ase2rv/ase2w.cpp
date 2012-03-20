/*
 *  Part of ase2rv source code,
 *  Copyright (C) 2001 Gabor Varga (bootes@freemail.hu)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rvgl/print.h"
#include "rvgl/helpfunc.h"
#include "rvgl/world.h"
#include "asematerial.h"
#include "asemesh.h"
#include "appopts.h"
#include "namelist.h"



int main(int argc, char **argv)
{
  AppOptions appopt;
  appopt.cmd=CmdWorld;
  appopt.parse(argc, argv);
  msglevel=appopt.msglevel;

  appopt.welcome();

  if(appopt.sgdump_only)
  {
    dump_sg_tables();
  }
  else if(appopt.listobj_only)
  {
    ASEParser p(appopt.ase_name);
    p.parse(NULL, NULL, NULL);
  }
  else
  {
    char iname[200], ncpname[200];
    ASEMaterialList *mats=new ASEMaterialList(); MEM(mats);
    ASEMeshList *meshes=new ASEMeshList(); MEM(meshes);
    ASEParser p(appopt.ase_name);
    p.parse(meshes, mats, appopt.names);
    World *w=meshes->createWorld(mats, appopt.wmode);
    strcpy(iname, appopt.out_name);
    replaceExt(iname, SUFFIX_W_IM);
    strcpy(ncpname, appopt.out_name);
    replaceExt(ncpname, SUFFIX_NCP);
    w->write_w(appopt.out_name, iname, ncpname);
    delete w;
    appopt.warn_untouched();
    delete mats;
    delete meshes;
  }
  return 0;
}
