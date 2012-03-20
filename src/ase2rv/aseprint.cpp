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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "aseprint.h"


char linbuf[500];

int(*aprintf)(const char *args, ...) = printf;

int ase_curlet=0;
char ase_loletts[4] = "ase";
char ase_hiletts[4] = "ASE";

#define PUTLOWASECHAR {putchar(ase_loletts[ase_curlet]); ase_curlet=(ase_curlet+1)%3;}
#define PUTHIGHASECHAR {putchar(ase_hiletts[ase_curlet]); ase_curlet=(ase_curlet+1)%3;}

int asemode_printf(const char *args, ...)
{
  va_list ap;
  char *cur;

  va_start(ap,args);
  vsprintf(linbuf,args,ap);
  va_end(ap);

  cur=linbuf;
  while(*cur!=0)
    {
      if((*cur>='a')&&(*cur<='z'))
	PUTLOWASECHAR
      else if((*cur>='A')&&(*cur<='Z'))
	PUTHIGHASECHAR
      else
	putchar(*cur);
      cur++;
    };
  
  return 0;
}
