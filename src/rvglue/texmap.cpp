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
 * $Id$
 *
 * $Log$
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/polygon.h"
#include "rvgl/print.h"

#include "parser.h"
#include "errdesc.h"
#include "texmap.h"
#include "keys.h"



Mod_TexMap::Mod_TexMap(void)
{
  parsed=false;
  rotate=false;    // vag 2001-05-08
}

bool Mod_TexMap::apply(Polygon *p)
{
  int npoints= p->numverts();
  int i;
  rvfloat tx, ty;
  rvfloat tmp;

  for(i=0;i<npoints;i++)
    if(!srcdef.uv_in_def(p->data.texture, &p->data.texcoord[i]))
      return true;

  /* is inside, now modify it: */
  if(dstdef.tex != TEX_UNKNOWN)
    p->data.texture = dstdef.tex;
  
  for(i=0;i<npoints;i++)
    {
      tx = (dstdef.x1 - dstdef.x0) / (srcdef.x1 - srcdef.x0);
      ty = (dstdef.y1 - dstdef.y0) / (srcdef.y1 - srcdef.y0);

      p->data.texcoord[i].u =  dstdef.x0 + (p->data.texcoord[i].u - srcdef.x0)*tx;
      p->data.texcoord[i].v =  dstdef.y0 + (p->data.texcoord[i].v - srcdef.y0)*ty;

      if(p->data.texcoord[i].u < 0.0) p->data.texcoord[i].u = 0.0;
      if(p->data.texcoord[i].u > 1.0) p->data.texcoord[i].u = 1.0;
      if(p->data.texcoord[i].v < 0.0) p->data.texcoord[i].v = 0.0;
      if(p->data.texcoord[i].v > 1.0) p->data.texcoord[i].v = 1.0;

      if(rotate)   // vag 2001-05-08
        {
          // dstdef was rotated right; now rotate the translated uv coords left.
          tmp = p->data.texcoord[i].u;
          p->data.texcoord[i].u = p->data.texcoord[i].v;
          p->data.texcoord[i].v = 1 - tmp;
        }
    };
  return true;
}

void Mod_TexMap::dump(const char *pref)
{
  printf("%stexmap: ",pref);
  srcdef.printtex(stdout);
  printf(" -> ");
  dstdef.printtex(stdout);
  if(rotate)    // vag 2001-05-08
    printf(" (dst rotated right)");
  if(bad())
    printf(", BAD");
  printf("\n");
}

bool Mod_TexMap::bad(void)
{
  return !parsed;
}



bool Mod_TexMap::parse(Parser &p)
{
  bool haveopen;
  char *eqsign;

  haveopen=p.eatOpen(false);

  if(p.curtok != PT_TEXT)
    {
      perror(0,ERROR_EXPECTED_KEY,"Expected texture map definition, not $$");
      return false;
    };

  eqsign=strchr(p.tok_text,'=');
  if(eqsign == NULL)
    {
      perror(0, ERROR_TEXDEF_SYNTAX, "Texture map definition is malformatted, since it contains no '='");
      return false;
    };

  *eqsign=0; eqsign++;

  if(!srcdef.parseTexSpec(p.tok_text, &default_src_texdef, p, true))
    return false;

  if(!dstdef.parseTexSpec(/*p.tok_text*/eqsign, &srcdef, p, true))  // fix: vag 2001-10-09
    return false;

  p.gentoken();

  if(p.curtok == PT_TEXT && strcasecmp(p.tok_text,KEY_ROT)==0)  // vag 2001-05-08
    {
      // Rotate dstdef right 90 degrees now. When translating, the translated
      // uv-s will be rotated left -> result is "rotating the image under
      // uv-s" (in addition to translation).
      // The rotation of dstdef is done here rather than during translation;
      // this way this rotation is done only once, and not repeated for all
      // polys that this modifier applies to.
      rotate=true;
      dstdef.rotateRight();
      p.gentoken();
    }

  if(haveopen)
    if(!p.eatClose(true))
      return false;

  parsed=true;
  return true;
}

