/*
 *  Part of rvglue source code,
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

/*
 * $Id: texdef.cpp,v 1.1 2000/05/03 08:49:37 ali Exp ali $
 *
 * $Log: texdef.cpp,v $
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "errdesc.h"
#include "parser.h"
#include "texdef.h"


#define TEX_SAFETY_DIST (0.5 / 256.0)


TextureDefinition default_src_texdef(TEX_UNKNOWN, 0.0, 1.0, 0.0, 1.0);





TextureDefinition::TextureDefinition(rvshort _tex,
                                     rvfloat _x0, rvfloat _x1,
                                     rvfloat _y0, rvfloat _y1)
{
  x0=_x0;
  y0=_y0;
  x1=_x1;
  y1=_y1;
  tex=_tex;
}

TextureDefinition::TextureDefinition()
{
  x0=y0=0.0;
  x1=y1=1.0;
  tex=TEX_UNKNOWN;
}

bool TextureDefinition::uv_in_def(rvshort sheet, RV_UV *uv)
{
  if((tex != TEX_UNKNOWN) && (sheet != TEX_UNKNOWN) && (sheet != tex))
    return false;

  return (uv->u >= x0-TEX_SAFETY_DIST)
    &&   (uv->u <= x1+TEX_SAFETY_DIST)
    &&   (uv->v >= y0-TEX_SAFETY_DIST)
    &&   (uv->v <= y1+TEX_SAFETY_DIST);
};

void TextureDefinition::rotateRight()   // vag 2001-05-08
{
  rvfloat tx0=x0;
  rvfloat tx1=x1;
  x0=1-y1;
  x1=1-y0;
  y0=tx0;
  y1=tx1;
}


void TextureDefinition::printtex(FILE *f)
{
  fprintf(f,"%c.bmp ",
         tex == TEX_UNKNOWN ? '*' : tex + 'a');

  if(x0 < -1.0)
    fprintf(f,"(*,*) -> ");
  else
    fprintf(f,"(%g,%g) -> ",x0*256.0,y0*256.0);

  if(x1 < -1.0)
    fprintf(f,"(*,*)");
  else
    fprintf(f,"(%g,%g)",x1*256.0,y1*256.0);
}


bool TextureDefinition::parseTexLetter(char *buf, Parser &p, bool allow_asterisk)
{
  if((tolower(buf[0])>='a') && (tolower(buf[0])<='j'))
    tex=tolower(buf[0])-'a';
  else if(buf[0]=='*')
    {
      if(allow_asterisk)
	tex=TEX_UNKNOWN;
      else
	{
	  p.error(ERROR_TEXDEF_WILDCARD, "Sheet wildcards ('*') not allowed for this texture definition");
	  return false;
	};
    }
  else
    {
      p.error(ERROR_TEXDEF_SYNTAX, "Sheet letter must be between 'a' and 'j'");
      return false;
    };

  if(buf[1] != 0)
    p.error(ERROR_TEXDEF_TRAIL, "Ignoring trailing garbage in texture sheet part");

  return true;
}


bool TextureDefinition::parseSingleNum(char *buf, int *num, Parser &p)
{
  char *c;

  c=buf;
  while((*c >= '0') && (*c <= '9'))
    c++;

  if(*c != 0)
    {
      p.error(ERROR_TEXDEF_SYNTAX, "Number '%s' contains non-digit(s)",buf);
      return false;
    };

  if(buf[0]==0)
    {
      p.error(ERROR_TEXDEF_SYNTAX, "Number in texture definition is empty");
      return false;
    };

  sscanf(buf,"%d",num);

  return true;
}


bool TextureDefinition::parseNumPair(char *obuf, char sepch, 
				     int *i1, int *i2,
				     const char *pairname, Parser &p)
{
  char buf[100];
  char *sep;

  strcpy(buf,obuf);

  if(buf[0]=='*')
    {
      *i1 = *i2 = -1;
      if(buf[1] != 0)
        {
          p.error(ERROR_TEXDEF_TRAIL, "Ignoring trailing garbage for '%s'\n",pairname);
        };
    }
  else
    {
      sep=strchr(buf,sepch);
      if(sep == NULL)
        {
          p.error(ERROR_TEXDEF_SYNTAX, "Group '%s' has no separator ('%c')!",pairname,sepch);
          return false;
        };
      *sep=0; sep++;
      if(!parseSingleNum(buf,i1,p))
	return false;
      if(!parseSingleNum(sep,i2,p))
	return false;
    };

  return true;
}


rvfloat px2uv(int i)
{
  return ((rvfloat)i) / 256.0;
}


bool TextureDefinition::parseTexSpec(const char *str, TextureDefinition *def, Parser &parser, bool allow_asterisk)
{
  char buf[100];
  char *col1, *col2;
  int sx0, sy0, swdt, shgt;

  strcpy(buf,str);
  col1=strchr(buf,':');
  if(col1 == NULL)
    {
      parser.error(ERROR_TEXDEF_SYNTAX,"Texture definition contains no ':'");
      return false;
    };
  *col1=0; col1++;

  col2=strchr(col1,':');
  if(col2 == NULL)
    {
      parser.error(ERROR_TEXDEF_SYNTAX,"Texture definition contains only one ':'");
      return false;
    };
  *col2=0; col2++;

  if(!parseTexLetter(buf,parser,allow_asterisk))
    return false;
  
  if(!parseNumPair(col1, ',', &sx0, &sy0, "top-left corner",parser))
    return false;

  if(!parseNumPair(col2, 'x', &swdt, &shgt, "texture size",parser))
    return false;


  /* now convert */

  if(sx0 == -1)
    {
      x0=def->x0;
      x1=def->x1;
    }
  else
    {
      x0 = px2uv(sx0);
      y0 = px2uv(sy0);
    };

  if(swdt == -1)
    {
      x1= (def->x1 - def->x0) + x0;
      y1= (def->y1 - def->y0) + y0; 
    }
  else
    {
      x1 = x0 + px2uv(swdt);
      y1 = y0 + px2uv(shgt);
    };

  /* test & fix all values */
  if(x0 > 1.0)
    x0=1.0;
  if(y0 > 1.0)
    y0=1.0;

  if(x1 > 1.0)
    x1=1.0;

  if(y1 > 1.0)
    y1=1.0;


  if((x1 - x0)<0.0001)
    {
      parser.error(ERROR_TEXDEF_SMALL,"Texture definition '%s' has a too small size",str);
      return false;
    };
  if((y1 - y0)<0.0001)
    {
      parser.error(ERROR_TEXDEF_SMALL,"Texture definition '%s' has a too small size",str);
      return false;
    };
  return true;
}
