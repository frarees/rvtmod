/*
 *  Part of ase2rv source code,
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
#ifndef __ASE_MATERIAL_H
#define __ASE_MATERIAL_H

#include "rvgl/rvtypes.h"
#include "aseparser.h"

// only the interesting things are kept...
class ASEMaterial
{
public:
  ASEMaterial();
  ~ASEMaterial();

  void parse(ASEParser &);
  void parseDiffMap(ASEParser &);

  ASEMaterial *find(int submatid);

  int id;
  rvshort texture;
  RVColor specular_3ds;
  bool    have_specular_3ds;
  rvfloat transp_3ds;
  bool    have_transp_3ds;
  char    name[100];

  bool has_subs;
  ASEMaterial **submats;
  int num_submats;
};

class ASEMaterialList   // vag 2001-05-08
{
public:
  ASEMaterialList();
  ~ASEMaterialList();

  void parse(ASEParser &p);

  ASEMaterial *find(int master, int sub);

private:
  ASEMaterial **materials;
  int cnt;
};

#endif
