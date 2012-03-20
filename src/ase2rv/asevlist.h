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
#ifndef __ASE_VECTORLIST_H
#define __ASE_VECTORLIST_H

#include "rvgl/rvtypes.h"
#include "aseparser.h"

class ASEVectorList
{
public:
  ASEVectorList(const char *_desc_name, int _listsec_key, int _entry_key, bool _flip_y=false, int _storage_order[3]=NULL);
  ~ASEVectorList();
  

  void readSize(ASEParser &);
  void readList(ASEParser &);

  void checkSanity(void);

  int find(const Vector &v);  // index of vector, or -1.  // vag 2001-05-08

  int size;
  Vector **list;

  char * desc_name;
  int listsec_key;
  int entry_key;
  int storage_order[3];
  bool flip_y;
};

#endif
