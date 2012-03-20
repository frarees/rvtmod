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
 * $Id: parsedmod.cpp,v 1.1 2000/05/03 08:49:37 ali Exp ali $
 *
 * $Log: parsedmod.cpp,v $
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/print.h"
#include "parsedmod.h"

bool ParsedMod::bad(void)
{
  return false;
}

bool ParsedMod::parse(Parser &p)
{
  return true;
}

void ParsedMod::dump(const char *pref)
{
  printf("%s[abstract]\n",pref);
}
