/*
 *  Part of ase2rv source code,
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
#ifndef __ASE_FACE_V_LIST_H
#define __ASE_FACE_V_LIST_H

#include "rvgl/rvtypes.h"
#include "aseparser.h"

struct ASEFaceVListEntry
{
  int id;
  int val[3];
};

class ASEFaceVList
{
public:
  ASEFaceVList(const char *_desc_name, int _listsec_key, int _entry_key);
  ~ASEFaceVList();
  
  inline bool isEmpty(void) { return (size<1); }

  void readSize(ASEParser &);
  void readList(ASEParser &);

  void checkSanity(bool empty_allowed);

  int size;
  ASEFaceVListEntry **list;

  char *desc_name;
  int   listsec_key;
  int   entry_key;
};

#endif
