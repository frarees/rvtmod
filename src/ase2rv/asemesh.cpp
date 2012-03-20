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
#include <errno.h>

#include "rvgl/print.h"
#include "rvgl/rvtypes.h"
#include "rvgl/world.h"
#include "rvgl/box.h"
#include "rvgl/helpfunc.h"

#include "aseprint.h"
#include "asekeys.h"
#include "aseparser.h"
#include "asematerial.h"
#include "asemesh.h"


enum ActionType
{
  trans, surf, env, dbl, nocoll, invis, addtrans, nop
};

struct SG_Action
{
  ActionType action;
  int arg;
};

// vag 2002-07-25: changed morph mode SG bits 23-28 to make bit 28 addtrans.

SG_Action sg_morph[32] =
{ {surf, 16}, {surf, 17}, {surf, 24}, {surf, 25}, {surf, 7},  //1..5
  {surf, 6}, {surf, 8}, {surf, 2}, {surf, 3}, {surf, 11},     //6..10
  {surf, 9}, {surf, 5}, {surf, 21}, {surf, 10}, {surf, 13},   //11..15
  {surf, 12}, {surf, 4}, {surf, 15}, {surf, 14}, {surf, 19},  //16..20
  {surf, 18}, {surf, 20},                                     //21..22
  {trans, 42}, {trans, 84}, {trans, 127}, {trans, 170},       //23..26
  {trans, 212}, {addtrans, 1},                                //27..28
  {invis, 1}, {env, 1}, {dbl, 1}, {nocoll, 1}                 //29..32
};

SG_Action sg_ali[32] =
{
  { surf,  1 },{ surf,  2 },{ surf,  3 },{ surf,  4 },{ surf,  5 }, // 1..5
  { surf,  6 },{ surf,  7 },{ surf,  8 },{ surf,  9 },{ surf, 10 }, // 6..10
  { surf, 11 },{ surf, 12 },{ surf, 13 },{ surf, 14 },{ surf, 15 }, // 11..15
  { surf, 16 },{ surf, 17 },{ surf, 18 },{ surf, 19 },{ surf, 20 }, // 16..20
  { surf, 21 },{ surf, 22 },{ surf, 23 },{ surf, 24 },{ surf, 25 }, // 21..25
  { nop, 0}, { nop, 0}, { addtrans, 1},                             // 26..28
  { invis, 1}, { nop, 0}, { dbl, 1}, {nocoll, 1}                    // 29..32
};


void dump_action(SG_Action &act)
{
  switch(act.action)
    {
    case trans:
      aprintf("transp.%3d%%     ", ((act.arg+1)*100)/256);
      break;
    case addtrans:
      aprintf("add. transp.   ");
      break;
    case surf:
      aprintf("surface %2d     ", act.arg);
      break;
    case env:
      aprintf("enable env     ");
      break;
    case dbl:
      aprintf("double sided   ");
      break;
    case nocoll:
      aprintf("no collision   ");
      break;
    case invis:
      aprintf("invisible      ");
      break;
    case nop:
      aprintf("nothing        ");
      break;
    };
}

void dump_sg_tables(void)
{
  int i;

  aprintf("   SG#  -ali effect    -morph effect  |   SG#  -ali effect    -morph effect \n");
  for(i=0;i<16;i++)
    {
      aprintf("   %2d   ", i+1);
      dump_action(sg_ali[i]);
      dump_action(sg_morph[i]);
      aprintf("|   %2d   ", i+17);
      dump_action(sg_ali[i+16]);
      dump_action(sg_morph[i+16]);
      aprintf("\n");
    };
}





ASEMesh::ASEMesh(void)
{
  int order_XZY[3]={0,2,1};

  num_faces_3ds=-1;
  faces_3ds=NULL;

  first_normal=last_normal=NULL;

  verts_3ds=new ASEVectorList("vertex",AK_VERT_LIST,AK_VERTEX, true, order_XZY); MEM(verts_3ds);
  tvert_3ds=new ASEVectorList("vertex texture",AK_T_VERT_LIST, AK_T_VERT); MEM(tvert_3ds);
  cvert_3ds=new ASEVectorList("vertex color", AK_C_VERT_LIST, AK_C_VERT); MEM(cvert_3ds);

  tface_3ds=new ASEFaceVList("face texture", AK_T_FACE_LIST, AK_T_FACE); MEM(tface_3ds);
  cface_3ds=new ASEFaceVList("face color", AK_C_FACE_LIST, AK_C_FACE); MEM(tface_3ds);

  nname=NULL;
  master_mat=-1;
}

ASEMesh::~ASEMesh(void)
{
  int i;
  ASEFaceNormalSection *cur, *nxt;

  cur=first_normal;
  while(cur != NULL)
    {
      nxt=cur->next;
      delete cur;
      cur=nxt;
    };

  if(faces_3ds != NULL)
    {
      for(i=0;i<num_faces_3ds;i++)
	if(faces_3ds[i]!=NULL)
	  delete faces_3ds[i];
      delete faces_3ds;
    };

  delete verts_3ds;
  delete tvert_3ds;
  delete cvert_3ds;
  delete tface_3ds;
  delete cface_3ds;

  if(nname)
    free(nname);
}

rvulong smgroup2bitmask(int i)
{
  if((i<1)||(i>32))
    sxpanic("Assumption 'Smoothing Groups are 1..32' wrong!");

  return ((rvulong)1) << (i-1);
}

void addsmgroupbits(rvulong &bits, char *buffer)
{
  char *cur, *next;
  int curint;

  cur=buffer;

  while((cur != NULL) && (*cur != 0))
    {
      next=strchr(cur,',');
      if(next != NULL)
	{
	  *next=0; next++;
	};
      sscanf(cur,"%d",&curint);
      bits |= smgroup2bitmask(curint);
      cur=next;
    };
}

void ASEMesh::setNumFaces3ds(int num)
{
  int i;

  if(num_faces_3ds != -1)
    sxpanic("Multiple %s", id2keyname(AK_NUM_FACES));

  num_faces_3ds=num;

  faces_3ds=new ASEFace*[num]; MEM(faces_3ds);
  for(i=0;i<num;i++)
    faces_3ds[i]=NULL;

  lprintf(2,"  (mesh: %d faces)\n",num);
}

void ASEMesh::readFaceVertices(ASEParser &p, ASEFace *f)
{
  int c;

  f->vert_idx_3ds[0]=f->vert_idx_3ds[1]=f->vert_idx_3ds[2]=-1;

  for(c=0;c<3;c++)
    {
      if(p.curtok != PT_STRING)
	sxpanic("MESH_FACE is strange [1]");
      if(p.tok_text[0] != 'A'+c)
	sxpanic("MESH_FACE is disordered");
      p.gentoken();
      if(p.curtok != PT_INTEGER)
	sxpanic("MESH_FACE is strange [2]");
      if((p.tok_int < 0)||(p.tok_int >= verts_3ds->size))
	sxpanic("MESH_FACE contains vertices out of range");
      f->vert_idx_3ds[c]=p.tok_int;
      p.gentoken();
    };

  /* the vertex id's are read. Now look for following stuff: */
  while(p.genKeyToken(false))
    {
      if(p.tok_key == AK_SMOOTH_GROUP)
	{
	  p.gentoken();
	  if(p.curtok == PT_INTEGER)
	    {
	      // just one group:
	      f->sm_bits |= smgroup2bitmask(p.tok_int);
	      p.gentoken();
	    }
	  else if(p.curtok == PT_STRING)
	    {
	      addsmgroupbits(f->sm_bits, p.tok_text);
	      p.gentoken();
	    };
	}
      else if(p.tok_key == AK_MAT_ID)
	{
	  p.gentoken();
	  if(p.curtok != PT_INTEGER)
	    sxpanic("Material ID is no integer???");
	  if(p.tok_int < 0)
	    sxpanic("Assumption 'Material ID's are never negative' is wrong!");
	  f->mat_id=p.tok_int;
	  p.gentoken();
	}
      else
	return; // something else, most likely next face
    };
}

void ASEMesh::readFaceList(ASEParser &p)
{
  char *colon;
  int i,curfidx;

  if(p.curtok != PT_BBEGIN)
    sxpanic("MESH_FACE_LIST section begins without '{'");
  if(verts_3ds->size <0)
    sxpanic("MESH_FACE_LIST before MESH_NUM_VERTEX");
  if(num_faces_3ds <0)
    sxpanic("MESH_FACE_LIST before MESH_NUM_FACES");
  p.genKeyToken(true);

  while(p.curtok == PT_KEYWORD)
    {
      if(p.tok_key == AK_FACE)
	{
	  p.gentoken();
	  if(p.curtok != PT_STRING)
	    sxpanic("Strange face number");
	  colon=strchr(p.tok_text,':');
	  if(colon == NULL)
	    sxpanic("Face number contains no ':'");
	  *colon=0;
	  sscanf(p.tok_text,"%d",&curfidx);
	  p.gentoken();

	  if((curfidx < 0)||(curfidx>=num_faces_3ds))
	    sxpanic("Face number outside range");
	  if(faces_3ds[curfidx]!=NULL)
	    sxpanic("Multiple faces definition");
	  faces_3ds[curfidx]=new ASEFace; MEM(faces_3ds[curfidx]);

	  faces_3ds[curfidx]->sm_bits=0;
	  faces_3ds[curfidx]->mat_id=-1;

	  readFaceVertices(p,faces_3ds[curfidx]);
	  p.genKeyToken(false);
	}
      else
	p.genKeyToken(true);
    };

  lprintf(2,"  (faces: ");
  for(i=0;i<num_faces_3ds;i++)
    if(faces_3ds[i] != NULL)
      {
	if(i!=0) lprintf(2,", ");
	lprintf(2,"%d:%d->%d->%d/M%d/SG%x",
		i,
		faces_3ds[i]->vert_idx_3ds[0],faces_3ds[i]->vert_idx_3ds[1],faces_3ds[i]->vert_idx_3ds[2],
		faces_3ds[i]->mat_id, faces_3ds[i]->sm_bits);
      };
  lprintf(2,")\n");

  if(p.curtok != PT_BEND)
    sxpanic("MESH_FACES_LIST section ends strange");
  p.gentoken();
}

void ASEMesh::readNormalList(ASEParser &p)
{
  int order_XZY[3]={0,2,1};

  int vidx, curcoord;
  ASEFaceNormalSection *newentry;

  if(p.curtok != PT_BBEGIN)
    sxpanic("%s section begins without '{'", id2keyname(AK_NORMAL_LIST));

  p.genKeyToken(true);

  while(p.curtok == PT_KEYWORD)
    {
      if(p.tok_key == AK_FACE_NORMAL)
	{
	  p.gentoken();
	  
	  newentry=new ASEFaceNormalSection;

	  if(p.curtok != PT_INTEGER)
	    sxpanic("Strange face number for %s", id2keyname(AK_FACE_NORMAL));

	  newentry->face_id=p.tok_int;
	  p.genKeyToken(true);

	  for(vidx=0;vidx<3;vidx++)
	    {
	      if(p.curtok != PT_KEYWORD)
		sxpanic("Missing data in %s!",id2keyname(AK_FACE_NORMAL));
	      if(p.tok_key != AK_VERT_NORMAL)
		sxpanic("Missing %s in %s!",id2keyname(AK_VERT_NORMAL), id2keyname(AK_FACE_NORMAL));
	      p.gentoken();
	      if(p.curtok != PT_INTEGER)
		sxpanic("Strange vertex number for %s", id2keyname(AK_VERT_NORMAL));
	      newentry->vert_id[vidx]=p.tok_int;
	      p.gentoken();

	      for(curcoord=0;curcoord<3;curcoord++)
		{
		  if((p.curtok != PT_INTEGER)&&(p.curtok != PT_FLOAT))
		    sxpanic("Strange vertex normal coordinate for %s", id2keyname(AK_VERT_NORMAL));
		  newentry->vert_normal[vidx].coord[order_XZY[curcoord]]=p.getFloatValue();
		  p.gentoken();
		};
	      newentry->vert_normal[vidx].coord[1]=-newentry->vert_normal[vidx].coord[1];
	    };

	  newentry->next=NULL;
	  if(first_normal==NULL)
	    first_normal=last_normal=newentry;
	  else
	    {
	      last_normal->next=newentry;
	      last_normal=newentry;
	    };
	  p.genKeyToken(false);
	}
      else
	sxpanic("Strange extra keyword in %s", id2keyname(AK_NORMAL_LIST));
    };

  if(p.curtok != PT_BEND)
    sxpanic("%s section ends strange", id2keyname(AK_NORMAL_LIST));
  p.gentoken();  
}

void ASEMesh::parse(ASEParser &p)
{
  if(p.curtok != PT_BBEGIN)
    sxpanic("MESH section begins without '{'");
  if(!p.genKeyToken(true))
    sxpanic("MESH section is empty");

  do
    {
      if(p.tok_key == AK_NUM_VERTS)//strcasecmp(p.tok_text, "*MESH_NUMVERTEX")==0)
	{
	  p.gentoken();
	  verts_3ds->readSize(p);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_NUM_T_VERTS)//strcasecmp(p.tok_text, "*MESH_NUMTVERTEX")==0)
	{
	  p.gentoken();
	  tvert_3ds->readSize(p);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_NUM_C_VERTS)//strcasecmp(p.tok_text, "*MESH_NUMCVERTEX")==0)
	{
	  p.gentoken();
	  cvert_3ds->readSize(p);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_NUM_T_FACES)//strcasecmp(p.tok_text, "*MESH_NUMTVFACES")==0)
	{
	  p.gentoken();
	  tface_3ds->readSize(p);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_NUM_C_FACES)//strcasecmp(p.tok_text, "*MESH_NUMCVFACES")==0)
	{
	  p.gentoken();
	  cface_3ds->readSize(p);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_NUM_FACES)//strcasecmp(p.tok_text, "*MESH_NUMFACES")==0)
	{
	  p.gentoken();
	  if(p.curtok != PT_INTEGER)
	    sxpanic("*MESH_NUMFACES is no int");
	  setNumFaces3ds(p.tok_int);
	  p.genKeyToken(true);
	}
      else if(p.tok_key == AK_VERT_LIST)//strcasecmp(p.tok_text, "*MESH_VERTEX_LIST")==0)
	{
	  p.gentoken();
	  verts_3ds->readList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_T_VERT_LIST)//strcasecmp(p.tok_text, "*MESH_TVERTLIST")==0)
	{
	  p.gentoken();
	  tvert_3ds->readList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_C_VERT_LIST)//strcasecmp(p.tok_text, "*MESH_CVERTLIST")==0)
	{
	  p.gentoken();
	  cvert_3ds->readList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_T_FACE_LIST)//strcasecmp(p.tok_text, "*MESH_TFACELIST")==0)
	{
	  p.gentoken();
	  tface_3ds->readList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_C_FACE_LIST)//strcasecmp(p.tok_text, "*MESH_CFACELIST")==0)
	{
	  p.gentoken();
	  cface_3ds->readList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_FACE_LIST)//strcasecmp(p.tok_text, "*MESH_FACE_LIST")==0)
	{
	  p.gentoken();
	  readFaceList(p);
	  p.genKeyToken(false);
	}
      else if(p.tok_key == AK_NORMAL_LIST)
	{
	  p.gentoken();
	  readNormalList(p);
	  p.genKeyToken(false);
	}
      else
	p.genKeyToken(true);
    }
  while(p.curtok == PT_KEYWORD);
  p.gentoken();
}

void ASEMesh::checkSanity(void)
{
  int i,j;

  verts_3ds->checkSanity();
  //  tvert_3ds->checkSanity();
  //  cvert_3ds->checkSanity();

  //  tface_3ds->checkSanity(false);

  //  cface_3ds->checkSanity(true);

  if(first_normal==NULL)
    sxpanic("Normal list empty!");

  if(faces_3ds==NULL)
    sxpanic("Faces list is empty");

  for(i=0;i<num_faces_3ds;i++)
    {
      if(faces_3ds[i]==NULL)
	sxpanic("Faces list is incomplete");
      for(j=0;j<3;j++)
	if(faces_3ds[i]->vert_idx_3ds[j]<0)
	  sxpanic("Faces list contains incomplete faces");
    };
}


void ASEMesh::createPoly(int face, Mesh *mesh, int &first_v_idx)
{
  int i;
  RV_Poly *poly=new RV_Poly;
  Polygon *mpoly;
  Vector *curcolor;

  MEM(poly);

  for(i=0;i<3;i++)
    {
      mesh->verts[first_v_idx] =
	new Vertex( *(verts_3ds->list[ faces_3ds[face]->vert_idx_3ds[i] ]),
		    Vector(1.0,1.0,1.0) );
      MEM(mesh->verts[first_v_idx]);
      poly->vertidx[i]=(rvushort)first_v_idx;
      first_v_idx++;
    };

  poly->vertidx[3]=0;
  poly->type=0;
  poly->texture=0;
  
  for(i=0;i<3;i++)
    {
      poly->texcoord[i].u=tvert_3ds->list[ tface_3ds->list[face]->val[i] ]->x();
      poly->texcoord[i].v=tvert_3ds->list[ tface_3ds->list[face]->val[i] ]->y();

      curcolor=cvert_3ds->list[ cface_3ds->list[face]->val[i] ];
      poly->color[i].setRGBf(curcolor->x(), curcolor->y(), curcolor->z());
    };

  mpoly=new Polygon(mesh, poly); MEM(mpoly);
  mesh->poly[face]=mpoly;
}




void ASEMesh::createPolyGeom(int face, Mesh *mesh, int &first_v_idx, 
			     ASEMaterialList *mats, WorkMode wmode)
{
  int i;
  RV_Poly *poly=new RV_Poly;
  Polygon *mpoly;
  ASEMaterial *mymat=NULL;
  SG_Action *actions;

  MEM(poly);

  for(i=0;i<3;i++)
    {
      mesh->verts[first_v_idx] =
	new Vertex( *(verts_3ds->list[ faces_3ds[face]->vert_idx_3ds[i] ]),
		    Vector(0.0,0.0,0.0) );
      MEM(mesh->verts[first_v_idx]);
      poly->vertidx[i]=(rvushort)first_v_idx;
      first_v_idx++;
    };

  poly->vertidx[3]=0;
  poly->type=0; /* triangles only !?! */
  
  /* fill in dummy data - every poly is white and boring: */
  poly->texture=-1;
  for(i=0;i<4;i++)
    {
      poly->color[i].value=0x00ffffff;
      poly->texcoord[i].u=poly->texcoord[i].v=0.0;
    };

  /* fill in material's data */
  if( master_mat>=0)
    {
      mymat=mats->find(master_mat, faces_3ds[face]->mat_id);
      poly->texture=mymat->texture;
    };

  mpoly=new Polygon(mesh, poly); MEM(mpoly);
  mesh->poly[face]=mpoly;
  delete poly;

  /* If material has no specular color, envmap is always disabled. */
  if(master_mat>=0)
    {
      if(mymat->have_specular_3ds)
        {
          mpoly->envmap=Default;
          mpoly->env_color.value=mymat->specular_3ds.value;
        }
      else
        mpoly->envmap=Disabled;
    }
  else
    mpoly->envmap=Disabled;


  if(wmode == WmMorph)
    actions=sg_morph;
  else if(wmode == WmAli)
    actions=sg_ali;
  else
    actions=NULL;

  if(actions != NULL)
    {
      rvulong bit;

      for(bit=0;bit<32;bit++)
	if(faces_3ds[face]->sm_bits & (1L << bit))
	  switch(actions[bit].action)
	    {
	    case trans:
	      mpoly->setTransparency((rvulong)actions[bit].arg);
	      break;
	    case addtrans:
	      mpoly->data.type |= POLY_IS_TRANS | POLY_TRANS_1_1;
	    case surf:
	      mpoly->setSurface(actions[bit].arg);
	      break;
	    case env:
              /* Env on by SG (except if material has no specular color) */
	      if(mpoly->envmap==Default)
		mpoly->envmap=Enabled;
	      break;
	    case dbl:
	      mpoly->data.type |= POLY_IS_DOUBLE;
	      break;
	    case nocoll:
	      mpoly->setNoColl();
	      break;
	    case invis:
	      mpoly->turnInvisible();
	      break;
	    case nop:
	      break;
	    };
    };

  if(wmode == WmAli)
    {
      /* ali-mode transparency from material */
      if(master_mat>=0)
        if(mymat->have_transp_3ds)
          mpoly->setTransparency((rvulong)(mymat->transp_3ds*255.0));

      /* Env on by default (except if material has no specular color. */
      if(mpoly->envmap==Default)
        mpoly->envmap=Enabled;
    }
  else if(wmode == WmMorph)
    {
      /* Env off by default (except if explicitly turned on by SG). */
      if(mpoly->envmap==Default)
        mpoly->envmap=Disabled;
    }

// vag 2002-07-25: allowing Disabled envmap pass through (Env can be disabled
// by setting specular color black).
}


void ASEMesh::applyPolyVColors(Mesh *mesh)
{
  int flist_idx, vidx;
  RV_Poly *pdata;
  Vector *curcol;

  for(flist_idx=0; flist_idx<cface_3ds->size; flist_idx++)
    {
      pdata= &(mesh->poly[ cface_3ds->list[flist_idx]->id ]->data);
      
      for(vidx=0; vidx < 3; vidx ++)
	{
	  curcol= cvert_3ds->list[  cface_3ds->list[flist_idx]->val[vidx]  ];

	  pdata->color[vidx].setRGBf(curcol->x(), curcol->y(), curcol->z() );
	};
    };
}


void ASEMesh::applyPolyVTextures(Mesh *mesh)
{
  int flist_idx, vidx;
  RV_Poly *pdata;
  Vector *curtex;

  for(flist_idx=0; flist_idx<tface_3ds->size; flist_idx++)
    {
      pdata= &(mesh->poly[ tface_3ds->list[flist_idx]->id ]->data);
      
      for(vidx=0; vidx < 3; vidx ++)
	{
	  curtex= tvert_3ds->list[  tface_3ds->list[flist_idx]->val[vidx]  ];

	  pdata->texcoord[vidx].u=curtex->x();
	  pdata->texcoord[vidx].v=1.0 - curtex->y();
	};
    };
}



void ASEMesh::applyFaceNormal(Mesh *mesh, ASEFaceNormalSection *fn)
{
  RV_Poly *pdata;
  int *curflist;
  int cur_vert_idx, id_to_find, vert_id, i;

  pdata=&(mesh->poly[fn->face_id]->data);
  curflist=faces_3ds[fn->face_id]->vert_idx_3ds;

  for(cur_vert_idx=0; cur_vert_idx<3; cur_vert_idx++)
    {
      // now find the vertex # in curflist
      id_to_find=fn->vert_id[cur_vert_idx];

      vert_id=-1;
      for(i=0;i<3;i++)
	if(curflist[i] == id_to_find)
	  vert_id=i;

      if(vert_id==-1)
	sxpanic("Vertex not found from Normal list");

      mesh->verts [ pdata->vertidx[vert_id] ]->normal
	= fn->vert_normal[cur_vert_idx];
    };
}

void flipPoly(RV_Poly &p)
{
  RVColor  hc;
  RV_UV    huv;
  rvushort hs;

  hc=p.color[0]; p.color[0]=p.color[2]; p.color[2]=hc;
  huv=p.texcoord[0]; p.texcoord[0]=p.texcoord[2]; p.texcoord[2]=huv;
  hs=p.vertidx[0]; p.vertidx[0]=p.vertidx[2]; p.vertidx[2]=hs;
}

Mesh *ASEMesh::createMesh(ASEMaterialList *mats, WorkMode wmode)
{
  Mesh *mesh=new Mesh;
  int cur_vert_idx, i;
  rvushort ii;
  ASEFaceNormalSection *curnormal;

  mesh->verts=new Vertex*[num_faces_3ds*4];
  mesh->poly =new Polygon*[num_faces_3ds];
  mesh->max_polys=num_faces_3ds;
  mesh->max_verts=num_faces_3ds*4;
  

  /* Build poly geometry: */
  cur_vert_idx=0;
  for(i=0;i<num_faces_3ds;i++)
    createPolyGeom(i, mesh, cur_vert_idx, mats, wmode);

  mesh->data.num_polys=(rvushort)num_faces_3ds;
  mesh->data.num_vecs =(rvushort)cur_vert_idx;

  /* Insert vertex normals: */
  curnormal=first_normal;
  while(curnormal != NULL)
    {
      applyFaceNormal(mesh,curnormal);
      curnormal=curnormal->next;
    };

  /* Insert vertex colors: */
  if(!cface_3ds->isEmpty())
    applyPolyVColors(mesh);

  /* Insert texture infos: */
  if(!tface_3ds->isEmpty())
    applyPolyVTextures(mesh);


  mesh->optimize();

  /* Flip textures: */
  for(ii=0; ii<mesh->data.num_polys; ii++)
    flipPoly( mesh->poly[ii]->data );

  for(ii=0; ii<mesh->data.num_vecs; ii++)
    if(fabs(mesh->verts[ii]->normal.length() - 1.0) > 0.1)
      sxpanic("Vertex normal%d isn't normal, but %g", ii,mesh->verts[ii]->normal.length());

  return mesh;
}


Box *ASEMesh::createBox()   // vag 2001-05-08
{
  // Well I'm not a linear algebra guru. There may be a more elegant way to
  // find out a box from eight corner vertices...
  Box *box=NULL;
  if(verts_3ds->size!=8)
    sxpanic("object '%s' has %d vertices; 8 is needed for a box",
      nname, verts_3ds->size);
  int i, j;
  for(i=0; i<8; i++)
    for(j=i+1; j<8; j++)
      if(verts_3ds->list[i]==verts_3ds->list[j])
        sxpanic("object '%s' has identical (or too close) vertices", nname);
  int i0, i1, i2, i3;
  Vector *c0, *c1, *c2, *c3;    // corner points
  Vector e1, e2, e3;            // edges meeting at c0
  bool touch[8];
  // Get four vertices (iterate over all combinations).
  for(i0=0; i0<8; i0++)
  {
    c0=verts_3ds->list[i0];
    for(i1=0; i1<8; i1++)
    {
      if(i1==i0)
        continue;
      c1=verts_3ds->list[i1];
      for(i2=0; i2<8; i2++)
      {
        if(i2==i0 || i2==i1)
          continue;
        c2=verts_3ds->list[i2];
        for(i3=0; i3<8; i3++)
        {
          if(i3==i0 || i3==i1 || i3==i2)
            continue;
          c3=verts_3ds->list[i3];
          // Now we have four points. Make three vectors (edges meeting in a
          // corner point) and check if these are a perpendicular right-handed
          // triplet.
          e1=*c1-*c0;
          e2=*c2-*c0;
          e3=*c3-*c0;
          if(Vector::right_perp_triplet(e1,e2,e3))
          {
            // Three perpendicular vectors. Check other corners.
            for(i=0; i<8; i++)
              touch[i]=false;
            touch[i0]=touch[i1]=touch[i2]=touch[i3]=true;
            if((j=verts_3ds->find(*c0+e1+e2))!=-1 && !touch[j])
            {
              touch[j]=true;
              if((j=verts_3ds->find(*c0+e1+e3))!=-1 && !touch[j])
              {
                touch[j]=true;
                if((j=verts_3ds->find(*c0+e2+e3))!=-1 && !touch[j])
                {
                  touch[j]=true;
                  if((j=verts_3ds->find(*c0+e1+e2+e3))!=-1 && !touch[j])
                  {
                    box=new Box; MEM(box);
                    box->id=(rvulong)-1;
                    box->flags=0;
                    box->center=((e1+e2+e3)/2.0) + *c0;
                    box->size=Vector(e1.length(),e2.length(), e3.length())/2.0;
                    if(!e1.normalize() || !e2.normalize() || !e3.normalize())
                      xpanic("can't normalize edges in object '%s'", nname);
                    box->rotation=Matrix_3x3(e1,e2,e3);
                    return box;    // bingo!
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  sxpanic("object '%s' does not seem to be a box", nname);
  /*NOTREACHED*/
  return NULL;
}



void ASEMesh::setName(const char *n)    // vag 2001-05-08
{
  if(nname)
    free(nname);
  nname=strdup(n); MEM(nname);
}



void dump_tazfile_syntax() // vag 2001-05-08
{
  /*       123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
  aprintf("Tazfile syntax:\n");
  aprintf("  * Empty lines are ignored.\n");
  aprintf("  * Lines beginning with # are ignored (use # for commenting).\n");
  aprintf("  * Otherwise, each line must contain name(s) of one or more box objects.\n");
  aprintf("  * Names containing whitespace must be enclosed in single or double quotes.\n");
  aprintf("  * Multiple names in the same line must be separated with whitespace.\n");
  aprintf("  * Track Zone id 0 is assigned to object(s) in the first line, and is\n");
  aprintf("    incremented with each line containing name(s). Names in the same line will\n");
  aprintf("    have identical ids; this defines alternate routes.\n");
}



void dump_visfile_syntax() // vag 2001-05-08
{
  /*       123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
  aprintf("Visfile syntax:\n");
  aprintf("  * Empty lines are ignored.\n");
  aprintf("  * Lines beginning with # are ignored (use # for commenting).\n");
  aprintf("  * Otherwise, each line must contain names of box objects and a colon.\n");
  aprintf("  * Names containing whitespace must be enclosed in single or double quotes.\n");
  aprintf("  * Multiple names in the same line must be separated with whitespace.\n");
  aprintf("  * Vizibox id 0 is assigned to objects in the first line, and is\n");
  aprintf("    incremented with each line containing names.\n");
  aprintf("    Names on the same line will have identical id-s (belong together).\n");
  aprintf("  * Names before the colon create Camera Boxes. Names after the colon\n");
  aprintf("    create Cube Boxes. There can be more than one Camera and/or Cube Box\n");
  aprintf("    with the same id (on the same line). Having a Camera Box without Cubes, or\n");
  aprintf("    Cubes without Cameras is allowed but makes no sense.\n");
}

// ASEMeshList by vag 2001-05-08

#define GRANULARITY 8

ASEMeshList::ASEMeshList()
{
  cnt=0;
  max=0;
  meshes=NULL;
}



ASEMeshList::~ASEMeshList()
{
  reset();
}



void ASEMeshList::add(ASEMesh *mesh)
{
  if(!mesh)
    xpanic("adding NULL mesh");
  if(cnt==max)
    grow();
  meshes[cnt++]=mesh;
}



ASEMesh *ASEMeshList::at( int index )
{
  if(index<0 || index>=cnt)
      xpanic("index out of range");
  return meshes[index];
}



ASEMesh *ASEMeshList::find(const char* name)
{
  for(int i=0; i<cnt; i++)
    if(!strcasecmp(name, meshes[i]->name()))
      return meshes[i];
  return NULL;
}



void ASEMeshList::reset()
{
  if(meshes)
  {
    while(--cnt>0)
      delete meshes[cnt];
    free(meshes);
    meshes=NULL;
    cnt=0;
    max=0;
  }
}



World* ASEMeshList::createWorld(ASEMaterialList *mats, WorkMode wmode)
{
  lprintf(1,"Converting objects\n");
  ASEMesh *aseMesh;
  Mesh *rvMesh;
  World *w=new World(); MEM(w);
  for(int i=0; i<cnt; i++)
  {
    aseMesh=meshes[i];
    lprintf(1, " (converting object '%s')\n", aseMesh->name());
    aseMesh->checkSanity();
    rvMesh=aseMesh->createMesh(mats, wmode);
    w->addMesh(rvMesh);
  }
  return w;
}



#define WS " \t\r\n"
#define QUOTES "'\""
#define LINELEN 256
#define COMMENTCHAR '#'
BoxList *ASEMeshList::createBoxes(const char *listFile, bool vis)
  {
  BoxList *boxes=NULL;
  ASEMesh *aseMesh;
  Box *box;
  char line[LINELEN];
  char *tok;
  rvulong id=0;
  unsigned char flags, check;
  int lineno=0;

  FILE *f=fopen( listFile, "rt" );
  if(!f)
    sxpanic("Error opening file '%s': %s\n", listFile, strerror(errno));
  lprintf(1,"Reading List File '%s'\n",listFile);
  boxes=new BoxList;
  while(fgets(line, LINELEN, f))
  {
    flags=CAMERA_NODE;  // Cameras first (before the colon)
    lineno++;
    check=0;
    if(line[0]==COMMENTCHAR)
      continue;
    if(!(tok=strqtok(line, WS, QUOTES)))
      continue;
    do
    {
      if(!strcmp(tok,":"))
      {
        if(flags==CAMERA_NODE)
          flags=CUBE_NODE;  // Cubes after the colon
        else
          fprintf(stderr, " *** Warning: %s:%d: multiple colon, ignored\n",
            listFile, lineno);
        continue;
      }
      aseMesh=find(tok);
      if(!aseMesh)
        sxpanic( "%s:%d: object '%s' not found", listFile, lineno, tok );
      lprintf(1, " ('%s' -> %s %d)\n", tok,
        vis ? ((flags & CUBE_NODE) ? "Cubes" : "Camera") : "Track Zone", id);
      box=aseMesh->createBox();
      if(vis && !box->axis_para())
        sxpanic("Box '%s' is not parallel to main axes", tok);
      box->id=id; // same id for objects listed on the same line.
      box->flags=flags;
      boxes->add(box);
      check |= flags;
    }
    while((tok=strqtok(NULL, WS, QUOTES))!=NULL);
    if(vis && check)
      {
      if(!(check & CAMERA_NODE))
        fprintf(stderr, " *** Warning: %s:%d: Cubes %d is useless without Camera\n",
          listFile, lineno, id);
      if(!(check & CUBE_NODE))
        fprintf(stderr, " *** Warning: %s:%d: Camera %d is useless without Cubes\n",
          listFile, lineno, id);
      }
    if(check)
      id++;  // increment only if there was something (not only a colon).
  }
  fclose(f);
  return boxes;
  }



void ASEMeshList::grow()
{
  int newmax=max+GRANULARITY;
  meshes=(ASEMesh**)realloc(meshes, newmax*sizeof(ASEMesh*)); MEM(meshes);
  max=newmax;
}

