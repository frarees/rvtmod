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
 * $Id: rvcolor.cpp,v 1.2 2000/05/03 09:26:07 ali Exp ali $
 *
 * $Log: rvcolor.cpp,v $
 * Revision 1.2  2000/05/03 09:26:07  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"
#include "rvcolor.h"


#define CH_SHIFT(c)  ( 24 - (8*(c)) )






rvulong RVColor::channel(int ch)
{
  if((ch < 0) || (ch>3))
    xpanic("Request for Channel #%d in RVColor",ch);

  return ( value >> CH_SHIFT(ch)) & 0xff;
}

void RVColor::setChannel(int ch, rvulong val)
{
  if((ch < 0) || (ch>3))
    xpanic("Request to write Channel #%d in RVColor",ch);

  if(val > 255)
    val=255;

  value =  ( value & (~(0xff << CH_SHIFT(ch))) ) | (val << CH_SHIFT(ch));
}




void RVColor::setRGB(rvulong r, rvulong g, rvulong b)
{
  if(r>255) r=255;
  if(g>255) g=255;
  if(b>255) b=255;

  value = (value & 0xff000000) | (r<<16) | (g<<8) | b;
}


void RVColor::shadeAdd(int r, int g, int b)
{
  r+=(int)red();   if(r<0) r=0;
  g+=(int)green(); if(g<0) g=0;
  b+=(int)blue();  if(b<0) b=0;
  
  setRGB((rvulong)r, (rvulong)g, (rvulong)b);
}

void RVColor::shade(float sr, float sg, float sb)
{
  setRGB( ((rvulong)( ((rvfloat)red())*sr )),
	  ((rvulong)( ((rvfloat)green())*sg )),
	  ((rvulong)( ((rvfloat)blue())*sb )) );
}
 
rvfloat RVColor::brightness(void)
{
  return sqrt(   SQR(((rvfloat)red()) / 255.0)
	      +  SQR(((rvfloat)green()) / 255.0)
	      +  SQR(((rvfloat)blue()) / 255.0) );
}
