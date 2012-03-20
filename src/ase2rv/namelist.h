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

#ifndef __NAME_LIST_H
#define __NAME_LIST_H

class NameList
{
public:
  NameList();
  ~NameList();

  //! If dupCheck is true and the name already exists, returns true.
  bool add(const char *name, bool dupCheck=false);
  const char *at(int i);
  const char *find(const char* name, bool touch=true);
  void reset();
  int count() {return cnt;}
  //! Create a new list from untouched names; ownership passed
  NameList *untouched();

private:
  void grow();

private:
  int cnt;
  int max;
  char **names;
  bool *touched;
};

#endif
