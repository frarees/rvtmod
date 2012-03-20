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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rvgl/rvtypes.h"
#include "rvgl/print.h"
#include "rvgl/helpfunc.h"

#include "aseparser.h"
#include "asekeys.h"
#include "asematerial.h"



ASEMaterial::ASEMaterial(void)
{
  id                 = -1;
  texture            = -1;
  specular_3ds.value = 0;
  have_specular_3ds  = false;
  transp_3ds         = 0.0;
  have_transp_3ds    = false;
  has_subs           = false;
  submats            = NULL;
  num_submats        = 0;
  strcpy(name,"(unknown)");
}



ASEMaterial::~ASEMaterial(void)
{
  int i;

  if(has_subs)
    {
      for(i=0;i<num_submats; i++)
	if(submats[i] != NULL)
	  delete submats[i];
      delete submats;
    };
}



void ASEMaterial::parse(ASEParser &p)
{
  rvfloat curval;
  ASEMaterial *newmat;
  int i;

  if(p.curtok != PT_INTEGER)
    sxpanic("Material ID is no integer");
  if(p.tok_int < 0)
    sxpanic("Assumption 'Material ID's are positive' is wrong");
  id=p.tok_int;
  p.gentoken();

  if(p.curtok != PT_BBEGIN)
    sxpanic("Material definition doesn't begin with '{'");
  p.gentoken();
  
  while(p.genKeyToken(false))
    {
      if(p.tok_key == AK_SUB_MATERIAL)
	{
	  p.gentoken();
	  if(!has_subs)
	    sxpanic("Material '%s' has sub-materials, but no %s",name,id2keyname(AK_NUM_SUB_MATERIALS));
	  newmat=new ASEMaterial; MEM(newmat);
	  newmat->parse(p);
	  if((newmat->id<0)||(newmat->id>=num_submats))
	    sxpanic("Submaterial is out of range");
	  if(submats[newmat->id] != NULL)
	    sxpanic("Submaterial specified multiple times");
	  submats[newmat->id] = newmat;
	}
      else if(p.tok_key == AK_NUM_SUB_MATERIALS)
	{
	  p.gentoken();
	  if(has_subs)
	    sxpanic("Multiple %s in material!",id2keyname(AK_NUM_SUB_MATERIALS));
	  if(p.curtok != PT_INTEGER)
	    sxpanic("%s is no integer",id2keyname(AK_NUM_SUB_MATERIALS));

	  num_submats=p.tok_int;
	  submats = new ASEMaterial*[num_submats]; MEM(submats);
	  for(i=0;i<num_submats;i++)
	    submats[i]=NULL;
	  has_subs=true;
	  p.gentoken();
	}
      else if(p.tok_key == AK_MATERIAL_NAME)
	{
	  p.gentoken();
	  if(p.curtok != PT_STRING)
	    sxpanic("Material name is no string?");
	  strncpy(name,p.tok_text,98);
	  name[99]=0;
	  p.gentoken();
	}
      else if(p.tok_key==AK_TRANSPARENCY)
	{
	  p.gentoken();
	  if((p.curtok!=PT_INTEGER)&&(p.curtok!=PT_FLOAT))
	    sxpanic("%s value is no number", id2keyname(AK_TRANSPARENCY));
	  transp_3ds=1.0 - p.getFloatValue();
	  if((transp_3ds < 0.0)||(transp_3ds > 1.0))
	    sxpanic("Assumption 'transparency is 0..1' wrong");
	  have_transp_3ds = (transp_3ds < 0.95);
	  p.gentoken();
	}
      else if(p.tok_key==AK_SPECULAR)
	{
	  p.gentoken();
	  have_specular_3ds = false;
	  for(i=0;i<3;i++)
	    {
	      if((p.curtok!=PT_INTEGER)&&(p.curtok!=PT_FLOAT))
		sxpanic("%s value is no number", id2keyname(AK_SPECULAR));
	      curval=p.getFloatValue();
	      if((curval < 0.0)||(curval > 1.0))
		sxpanic("Assumption 'specular rgb is [0..1]^3' wrong");
	      if(curval>0.02)
		have_specular_3ds=true;
	      specular_3ds.setChannel(i+1, (rvulong)(curval*255.0) );
	      p.gentoken();
	    };
	}
      else if(p.tok_key==AK_DIFFUSE_MAP)
	{
	  p.gentoken();
	  parseDiffMap(p);
	}
      else
	p.gentoken();
    };

  if(p.curtok != PT_BEND)
    sxpanic("Material definition ends strange");
  p.gentoken();
  
  if(!has_subs)
    {
      lprintf(2,"  (material '%s': id%d, tex%d", name, id, texture);
      if(have_specular_3ds)
	lprintf(2,", spec#%06x", specular_3ds.value);
      if(transp_3ds)
	lprintf(2,", transp%g", transp_3ds);
      lprintf(2,")\n");
    };
}



static void bitmaprefstupid(const char *bitmapname, const char *mat_name, char letter)
{
  printf("\nError:\n");
  printf("Material '%s' uses the texture\n",mat_name);
  printf("  %s\n",bitmapname);
  printf("This tool tries to find the correct Re-Volt texture sheet by looking\n");
  printf("at the last character in the file name.\n");
  printf("In this case it is '%c'\n",letter);
  printf("But only letters A to J are legal.\n\n");
  printf("So, rename the texture and update the material\n");
  exit(1);
}



ASEMaterial *ASEMaterial::find(int submatid)
{
  if(!has_subs)
    return this;

  if(submats==NULL)
    sxpanic("I lost my sub-material list somewhere");
  if((submatid < 0) || (submatid >= num_submats))
    sxpanic("Submaterial not found");
  if(submats[submatid] == NULL)
    sxpanic("Submaterial doesn't exist");

  return submats[submatid];
}



void ASEMaterial::parseDiffMap(ASEParser &p)
{
  char *dot;

  if(p.curtok != PT_BBEGIN)
    sxpanic("%s doesn't begin with '{'", id2keyname(AK_DIFFUSE_MAP));

  p.gentoken();
  
  while(p.genKeyToken(false))
    {
      if(p.tok_key == AK_BITMAP)
	{
	  p.gentoken();
	  if(p.curtok != PT_STRING)
	    sxpanic("%s is no string", id2keyname(AK_BITMAP));

	  dot=strrchr(p.tok_text,'.');

	  if(dot==NULL)
	    sxpanic("Weird bitmap");
	  if(dot<=p.tok_text)
	    sxpanic("Bitmap without extension???");

	  dot--;
	  if((tolower(*dot)<'a') || (tolower(*dot)>'j'))
	    bitmaprefstupid(p.tok_text, name, *dot);
	  texture=(tolower(*dot) - 'a');
	  p.gentoken();
	}
      else
	p.gentoken();
    };

  if(p.curtok != PT_BEND)
    sxpanic("%s ends strange", id2keyname(AK_DIFFUSE_MAP));
  p.gentoken();
}


// ASEMAterialList by vag 2001-05-08

ASEMaterialList::ASEMaterialList()
{
  materials=NULL;
  cnt=-1;
}



ASEMaterialList::~ASEMaterialList()
{
  while(--cnt>=0)
    delete materials[cnt];
  delete materials;
}



void ASEMaterialList::parse(ASEParser &p)
{
  int i;
  ASEMaterial *newmat;

  if(p.curtok != PT_BBEGIN)
    sxpanic("%s doesn't begin with '{'", id2keyname(AK_MATERIAL_LIST));

  p.gentoken();
  while(p.genKeyToken(false))
    {
      if(p.tok_key == AK_NUM_MATERIALS)
	{
	  p.gentoken();
	  if(p.curtok != PT_INTEGER)
	    sxpanic("%s is no integer???", id2keyname(AK_NUM_MATERIALS));
	  if(materials != NULL)
	    sxpanic("%s given multiple times", id2keyname(AK_NUM_MATERIALS));
	  cnt=p.tok_int;
	  materials=new ASEMaterial*[cnt]; MEM(materials);
	  for(i=0;i<cnt;i++)
	    materials[i]=NULL;
	  p.gentoken();
	}
      else if(p.tok_key == AK_MATERIAL)
	{
	  p.gentoken();
	  newmat=new ASEMaterial; MEM(newmat);
	  newmat->parse(p);
	  if((newmat->id<0)||(newmat->id>=cnt))
	    sxpanic("Material ID is out of range");
	  if(materials[newmat->id] != NULL)
	    sxpanic("Material specified multiple times");
	  materials[newmat->id] = newmat;
	}
      else
	p.gentoken();
    };

  if(p.curtok != PT_BEND)
    sxpanic("%s ends strange", id2keyname(AK_MATERIAL_LIST));

  p.gentoken();
}



ASEMaterial *ASEMaterialList::find(int master, int sub)
{
  if(materials==NULL)
    sxpanic("I lost my material list somewhere");
  if((master < 0) || (master >= cnt))
    sxpanic("Master material not found");
  if(materials[master] == NULL)
    sxpanic("Master material doesn't exist");

  return materials[master]->find(sub);
}
