/*
 *  mkmirror source code,
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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

#include "rvgl/rvtypes.h"



struct POR_Entry
{
  rvulong type;
  rvulong id[2];
  Vector  center;
  Matrix_3x3 rotation;
  Vector  size;
  rvfloat zeroes[4];
};

struct RIM_Entry
{
  rvulong flags; // actually 0 or 1

  Vector  plane_normal;
  rvfloat plane_distance;

  AxisBox bbox;

  Vector  vertices[4];
};


std::vector<RIM_Entry *> riments;


void convert_single(POR_Entry &ent)
{
  Vector    verts[4], side1, side2, tmpvec;
  RIM_Entry *rim;
  int i;

  rim=new RIM_Entry;

  rim->flags=1;
  rim->bbox.be_empty();

  verts[0] = Vector(-ent.size[0], -ent.size[1], 0.0);
  verts[1] = Vector(ent.size[0], -ent.size[1], 0.0);
  verts[2] = Vector(ent.size[0], ent.size[1], 0.0);
  verts[3] = Vector(-ent.size[0], ent.size[1], 0.0);
 
  for(i=0;i<4;i++)
    {
      tmpvec = ent.rotation * verts[i];
      rim->vertices[i] = ent.center + tmpvec;
      rim->bbox.fit_into(rim->vertices[i]);
    };
  
  side1=rim->vertices[0] - rim->vertices[1];
  side2=rim->vertices[2] - rim->vertices[1];

  rim->plane_normal = side1*side2;
  rim->plane_normal.normalize();

  rim->plane_distance= - rim->plane_normal.scalarprod(rim->vertices[0]);

  riments.push_back(rim);
}

void convert(const char *porname, const char *rimname)
{
  FILE *por, *rim;
  rvulong   por_size, il;
  rvushort  rim_size, is;
  POR_Entry por_entry;

  por=fopen(porname,"rb");
  if(por==NULL)
    {
      printf("Can't open file %s: %s\n",
	     porname, strerror(errno));
      return;
    };

  riments.clear();

  fread(&por_size, sizeof(rvulong), 1, por);

  for(il=0;il<por_size;il++)
    {
      fread(&por_entry, sizeof(POR_Entry), 1, por);

      if(por_entry.type == 0)
	{
	  printf("  (converting portal %d/%d)\n",por_entry.id[0],por_entry.id[1]);
	  convert_single(por_entry);
	}
      else
	printf("  (skipping region %d)\n",por_entry.id[0]);
    };

  fclose(por);

  rim_size=riments.size();


  rim=fopen(rimname,"wb");

  if(rim==NULL)
    {
      printf("Can't write file %s: %s\n",
	     rimname, strerror(errno));
      return;
    };


  fwrite(&rim_size, sizeof(rvushort), 1, rim);
  for(is=0;is<rim_size;is++)
    fwrite(riments[is], sizeof(RIM_Entry), 1, rim);
  fclose(rim);
}



int main(int argc, char **argv)
{
  char porn[200], rimn[200];
  if(argc != 2)
    {
      printf("Usage: mkmirror <filebase>\n");
      printf("  converts <filebase>.por to <filebase>.rim\n");
      exit(1);
    };

  printf("mkmirror is Copyright (C) 2000 Alexander Kroeller\n");
  printf("It is distributed under the terms of the GPL - see 'doc\\gpl.html' for details\n\n");

  sprintf(porn,"%s.por",argv[1]);
  sprintf(rimn,"%s.rim",argv[1]);
  convert(porn,rimn);

  return 0;
}
