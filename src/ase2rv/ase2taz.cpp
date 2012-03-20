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
#include "rvgl/box.h"

#include "asemesh.h"
#include "appopts.h"
#include "namelist.h"



int main(int argc, char **argv)
{
  AppOptions appopt;
  appopt.cmd=CmdTaz;
  appopt.parse(argc, argv);
  msglevel=appopt.msglevel;

  appopt.welcome();

  if(appopt.listsyntax_only)
  {
    dump_tazfile_syntax();
  }
  else if(appopt.listobj_only)
  {
    ASEParser p(appopt.ase_name);
    p.parse(NULL, NULL, NULL);
  }
  else
  {
    ASEMeshList *meshes=new ASEMeshList(); MEM(meshes);
    ASEParser p(appopt.ase_name);
    p.parse(meshes, NULL, appopt.names);
    BoxList *b=meshes->createBoxes(appopt.list_name, /*vis=*/false);
    b->write_taz(appopt.out_name, appopt.safety_dist);
    delete b;
    appopt.warn_untouched();
    delete meshes;
  }
  return 0;
}
