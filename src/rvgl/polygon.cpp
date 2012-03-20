/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
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
 * $Id: polygon.cpp,v 1.2 2000/05/03 09:23:11 ali Exp ali $
 *
 * $Log: polygon.cpp,v $
 * Revision 1.2  2000/05/03 09:23:11  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvtypes.h"
#include "polygon.h"


Polygon::Polygon(Mesh *p, RV_Poly *d)  // ces 2000-04-13
{
  parent=p;
  data=*d;
  ncp_type=0;
  surface=0;
  envmap=Default;
  lower_coll=0; // vag 2001-06-28

  env_color.setRGB(128,128,128);
}

Polygon::Polygon(Mesh *p, FILE *f, bool is_IM, bool is_Instance)
{
  parent=p;
  fread(&data,RV_POLY_SIZE,1,f);

  if(is_IM)
    {
      fread(&ncp_type, sizeof(rvlong), 1, f);
      fread(&surface,  sizeof(rvlong), 1, f);
    }
  else
    {
      ncp_type=0;
      surface=0;
    };

  if(is_Instance)
    envmap = (data.type & POLY_PRM_DISABLE_ENV) ? Disabled : Default;
  else
    envmap = (data.type & POLY_W_ENABLE_ENV) ? Enabled : Default;

  lower_coll=0; // vag 2001-06-28
  env_color.setRGB(128,128,128);

  data.type &= ~(POLY_PRM_DISABLE_ENV | POLY_W_ENABLE_ENV);
}


void Polygon::setTransparency(rvulong t)
{
  int i;

  if(t == 255)
    {
      data.type &= ~0x104; // transp. off
    }
  else
    {
      for(i=0;i<numverts();i++)
	data.color[i].setAlpha(t);
      data.type = (data.type & (~0x104)) | 0x004;
    };
}

Polygon::Polygon(Mesh *m, Polygon *p)
{
  parent=m;

  surface   = p->surface;
  ncp_type  = p->ncp_type;
  envmap    = p->envmap;
  env_color = p->env_color;

  memcpy(&data, &p->data, RV_POLY_SIZE);
}

void Polygon::setSurface(rvlong s)
{
  if(surface != SURF_NOCOLL)
    surface=s;
}

void Polygon::turnInvisible(void)
{
  data.texture = TEX_INVISIBLE;
}

bool Polygon::isInvisible(void)
{
  return data.texture == TEX_INVISIBLE;
}

void Polygon::setNoColl(void)
{
  surface=SURF_NOCOLL;
}

bool Polygon::isNoColl(void)
{
  return surface==SURF_NOCOLL;
}

int Polygon::numverts(void)
{
  return (data.type & 1) ? 4 : 3;
}




void Polygon::write(FILE *f, bool is_IM, bool is_Instance)
{
  data.type &= ~(POLY_PRM_DISABLE_ENV | POLY_W_ENABLE_ENV);
  if(is_Instance)
    {
      if(envmap == Disabled)
	data.type |= POLY_PRM_DISABLE_ENV;
    }
  else
    {
      if(envmap == Enabled)
	data.type |= POLY_W_ENABLE_ENV;
    };

  fwrite(&data,RV_POLY_SIZE,1,f);

  if(is_IM)
    {
      fwrite(&ncp_type, sizeof(rvulong), 1, f);
      fwrite(&surface,  sizeof(rvulong), 1, f);
    };
}







bool Polygon::teheur_BlueWall(void)
{
  int i;
  rvulong r,g,b;
  bool isTE=false;

  if(data.texture != -1)
    return false;

  for(i=0;i<numverts();i++)
    {
      r=(data.color[i].red());
      g=(data.color[i].green());
      b=(data.color[i].blue());

      if((r!=g)||(g!=b))
	isTE=true;
    };
  return isTE;
}

int Polygon::vertidx(int idx)
{
  return data.vertidx[idx];
}

void Polygon::setTEColor(rvulong color)
{
  rvulong r,g,b;
  float scale;
  int i;

  if(!teheur_BlueWall())
    return;

  for(i=0;i<numverts();i++)
    {
      r=data.color[i].red();
      g=data.color[i].green();
      b=data.color[i].blue();

      scale = ((float)r + (float)g + (float)b) / 765.0;

      r=(rvulong)(   (float)( (color&0xff0000) >> 16 )    * scale );
      g=(rvulong)(   (float)( (color&0xff00)   >> 8  )    * scale );
      b=(rvulong)(   (float)( (color&0xff)           )    * scale );

      data.color[i].setRGB(r,g,b);

    };
}


bool Polygon::teheur_Lamp(void)
{
  int i;
  rvulong r,g,b;
  bool isTE=true;

  if(data.texture != -1)
    return false;

  for(i=0;i<numverts();i++)
    {
      r=data.color[i].red();
      g=data.color[i].green();
      b=data.color[i].blue();

      if((r!=g)||(g!=b))
	isTE=false;
    };
  return isTE;
}

bool Polygon::isInTexture(rvshort page,
			  rvfloat ulo, rvfloat uhi,
			  rvfloat vlo, rvfloat vhi)
{
  int i;

  if(data.texture != page)
    return false;

  for(i=0;i<numverts();i++)
    {
      if( data.texcoord[i].u < ulo )
	return false;
      if( data.texcoord[i].v < vlo )
	return false;
      if( data.texcoord[i].u > uhi )
	return false;
      if( data.texcoord[i].v > vhi )
	return false;
    };
  return true;
}


bool Polygon::teheur_Floor(void)
{
  return isInTexture(0,  0.0,0.1  ,  0.9,1.0);
}

bool Polygon::teheur_Floor2(void)  // ces 2000-04-13
{
  return isInTexture(7,  0.0,0.25  ,  0.75,1.0);
}

bool Polygon::teheur_Box(void)
{
  return isInTexture(7,  0.0,0.75  ,  0.0,0.25);
}

bool Polygon::teheur_CarImage(void)
{
  return isInTexture(7,  0.0,1.0  ,  0.25,0.75);
}


bool Polygon::teheur_RWall(void)
{ 
  if(isInTexture(7, 0.0,1.0, 0.75,1.0))
    return true;
  return isInTexture(7, 0.75,1.0, 0.0,0.25);
}

bool Polygon::teheur_Pipe(void)  // ces 2000-04-13
{
  return isInTexture(0,  0.50,0.75  ,  0.0,0.375);
}

bool Polygon::teheur_BCeil(void)  // ces 2000-04-13
{
  return isInTexture(1,  0.25,0.5  ,  0.0,0.25);
}

bool Polygon::teheur_BRail(void)  // ces 2000-04-13
{
  return isInTexture(1,  0.0,0.25  ,  0.0,0.25);
}


int Polygon::detTEPart(void)
{
  /* This runs some heuristics to guess what part of a track editor track
     this polygon could be */

  if(teheur_BlueWall())
    return TEP_PWALL;

  if(teheur_Lamp())
    return TEP_LAMP;

  if(teheur_Floor())
    return TEP_FLOOR;

  if(teheur_Floor2())  // ces 2000-04-13
    return TEP_FLOOR2;

  if(teheur_RWall())
    return TEP_RWALL;

  if(teheur_Box())
    return TEP_BOX;

  if(teheur_CarImage())
    return TEP_CARIMG;

  if(teheur_Pipe())   // ces 2000-04-13
    return TEP_PIPE;

  if(teheur_BCeil())  // ces 2000-04-13
    return TEP_BCEIL;

  if(teheur_BRail())  // ces 2000-04-13
    return TEP_BRAIL;

  return TEP_TRACK;
}


void Polygon::shiftVIndices(rvushort plus)
{
  int i;

  for(i=0;i<numverts();i++)
    data.vertidx[i] += plus;
}
