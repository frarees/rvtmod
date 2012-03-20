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
 * $Id: mesh.cpp,v 1.2 2000/05/03 09:07:38 ali Exp ali $
 *
 * $Log: mesh.cpp,v $
 * Revision 1.2  2000/05/03 09:07:38  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvtypes.h"
#include "mesh.h"
#include "print.h"

#define MBBOX_SAFETY_DIST 2.0





void Mesh::deleteSomePolys(bool delUnsolids, bool delInvisibles)
{
  int i;
  bool dodel;

  for(i=0;i<data.num_polys;i++)
    if(poly[i] != NULL)
      {
	dodel=false;

	if(poly[i]->isNoColl() && delUnsolids)
	  dodel=true;
	else if(poly[i]->isInvisible() && delInvisibles)
	  dodel=true;
	else if(poly[i]->isNoColl() && poly[i]->isInvisible())
	  dodel=true;

	if(dodel)
	  {
	    delete poly[i];
	    poly[i]=NULL;
	  };
      };

  optimize();
}


void Mesh::optimize(void)
{
  cleanupPolyList();
  cleanupVertexList();
}

void Mesh::cleanupVertexList(void)
{
  bool *vused;
  int *ttable;
  int i, j, p, idx, numused;

  if(data.num_vecs == 0)
    return;

  vused=new bool[data.num_vecs]; MEM(vused);
  ttable=new int[data.num_vecs]; MEM(ttable);
      
  for(i=0;i<data.num_vecs;i++)
    vused[i]=false;
      
  // Step one: Find vertices listed multiple times in the list.
  ttable[0]=0;
  for(i=1;i<data.num_vecs;i++)
    {
      ttable[i]=i;
      for(j=0;j<i;j++)
	if( (verts[i]->pos == verts[j]->pos) && (verts[i]->normal == verts[j]->normal) )
	  { ttable[i]=j; break; }
    };
  
  // Check what vertex indices are in use and apply prior ttable[]
  for(p=0;p<data.num_polys;p++)
    if(poly[p] != NULL)
      for(i=0;i<poly[p]->numverts();i++)
	{
	  
	  idx=poly[p]->data.vertidx[i]= ttable[poly[p]->vertidx(i)];
	  
	  if((idx < 0) || (idx>data.num_vecs))
	    xpanic("Screwed up vertex indices\n");
	  
	  vused[idx] = true;
	};
  
  // Build transition table
  idx=0;
  for(i=0;i<data.num_vecs;i++)
    if(vused[i])
      {
	ttable[i] = idx;
	idx++;
      };
  numused=idx;
  
  // Transpose vertex indices in polys
  for(p=0;p<data.num_polys;p++)
    if(poly[p] != NULL)
      for(i=0;i<poly[p]->numverts();i++)
	poly[p]->data.vertidx[i] = ttable[poly[p]->data.vertidx[i]];
  
  // We do not create a new vertex list, since delete() knows the size for itself
  // and we use data.num_vecs
  for(i=0;i<data.num_vecs;i++)
    if(vused[i])
      verts[ttable[i]] = verts[i];
    else
      delete verts[i];
  data.num_vecs = numused;
  
  delete ttable;
  delete vused;
}

Mesh::Mesh(RV_PRM_Mesh *m, FILE *f, bool intermediate)
{
  data.num_polys=m->num_polys;
  data.num_vecs=m->num_vecs;
  load(f, intermediate, true/*is instance*/);
}

Mesh::Mesh(RV_W_Mesh *m, FILE *f, bool intermediate)
{
  memcpy(&data,m, RV_W_MESH_SIZE);
  load(f, intermediate, false/*isn't instance*/);
}



Mesh::Mesh(Mesh *m)
{
  int i;

  memcpy(&data, &m->data, RV_W_MESH_SIZE);
  poly=new Polygon*[data.num_polys]; MEM(poly);
  verts=new Vertex*[data.num_vecs]; MEM(verts);

  max_polys=data.num_polys;  // ces 2000-04-13
  max_verts=data.num_vecs;   // ces 2000-04-13

  for(i=0;i<data.num_polys;i++)
    if(m->poly[i] == NULL)
      poly[i] = NULL;
    else
      poly[i] = new Polygon(this, m->poly[i]);

  for(i=0;i<data.num_vecs;i++)
    verts[i] = new Vertex(m->verts[i]);
}


Mesh::Mesh()
{ }

Mesh::~Mesh()
{
  int i;

  for(i=0;i<data.num_polys;i++)
    if(poly[i] != NULL)
      delete poly[i];

  for(i=0;i<data.num_vecs;i++)
    delete verts[i];

  delete poly;
  delete verts;
}

void Mesh::add(Polygon *p)  // ces 2000-04-13
{
  if(data.num_polys >= max_polys)
    {
      rvulong   new_size  = max_polys * 2;
      Polygon **new_polys = new Polygon *[new_size]; MEM(new_polys);

      memcpy(new_polys, poly, data.num_polys * sizeof(Polygon *));

      delete poly;
	  poly      = new_polys;
	  max_polys = new_size;
    }

  poly[data.num_polys++]=p;
}

void Mesh::add(Vertex *v)  // ces 2000-04-13
{
  if(data.num_vecs >= max_verts)
    {
      rvulong  new_size  = max_verts * 2;
      Vertex **new_verts = new Vertex *[new_size]; MEM(new_verts);

      memcpy(new_verts, verts, data.num_vecs * sizeof(Vertex *));

      delete verts;
      verts     = new_verts;
      max_verts = new_size;
    }

  verts[data.num_vecs++]=v;
}


int Mesh::numPolys(void)
{
  return data.num_polys;
}

int Mesh::numSolidPolys(void)
{
  int i,res=0;

  for(i=0;i<data.num_polys;i++)
    if(!poly[i]->isNoColl())
      res++;
  return res;
}

Surface *Mesh::createPolySurface(int pidx)
{
  // Lowering collision data (thus messing up this method) by vag 2001-06-28.
  // The vertexes to be used for creating the surface are pointed by
  // pverts array. This is initialized with pointers to Mesh's own vertices.
  // If lowering is needed (and applies), those original vectors are copied
  // into a local array (lowvecs), lowered, then pverts is set to point to
  // lowvecs instead of Mesh's vertices. (->Mesh unchanged.)

  Vector *pverts[4];    // Pointer to vertices to be used (original or lowered)
  Vector lowvecs[4];    // Lowered vertex vectors (if lowering applies)
  Surface *res;
  int i;

  if(poly[pidx]->isNoColl())
    xpanic("Creation of Surface for non-solid polygon\n");

  // vertexes to be used are the mesh vertexes by default.
  pverts[3]=NULL; // may be tri poly, NULL fourth vector.
  for(i=0;i<poly[pidx]->numverts();i++)
    pverts[i]=&(verts[ poly[pidx]->vertidx(i)]->pos);

  // Check if this is a vertical axis-aligned quad rectangle poly, which should
  // have lowered collision data.
  if(poly[pidx]->lower_coll && pverts[3]) // Quad poly, to be lowered.
    {
      if (
           (FEQUAL(pverts[0]->x(), pverts[1]->x()) && FEQUAL(pverts[0]->x(), pverts[2]->x()) && FEQUAL(pverts[0]->x(), pverts[3]->x())) ||
           (FEQUAL(pverts[0]->z(), pverts[1]->z()) && FEQUAL(pverts[0]->z(), pverts[2]->z()) && FEQUAL(pverts[0]->z(), pverts[3]->z()))
         )
        {
          // Vertices are aligned with xy or zy plane (x or z coord is same).
          rvfloat ylo= +1e40;
          rvfloat yhi= -1e40;
          for(i=0;i<4;i++)
            {
              // Calculate low and high y values.
              if(ylo>pverts[i]->y())
                ylo=pverts[i]->y();
              if(yhi<pverts[i]->y())
                yhi=pverts[i]->y();
            }
          int v[4];
          int low=0, high=0;
          for(i=0;i<4;i++)
            {
              // 2-2 vertices must be low and high y; (or else this is not a
              // rectangle). Build an index array showing which is low and
              // which is high.
              if(FEQUAL(pverts[i]->y(), ylo))
                {
                  v[i]=-1;  // "low y" vertex
                  low++;
                }
              else if(FEQUAL(pverts[i]->y(), yhi))
                {
                  v[i]=1;   // "high y" vertex
                  high++;
                }
              else
                {
                  break;    // Something in between: not a rectangle.
                }
            }
          if(low==2 && high==2)
            {
              // We have 2 low and 2 high y. Copy original vectors and lower
              // the vertices. Since the coordinate system is +x/+z/-y
              // (vertical dir is y downwards), this means adding to low y.
              for(i=0;i<4;i++)
                {
                  lowvecs[i]=*(pverts[i]);  // copy original
                  pverts[i]=&(lowvecs[i]);  // use lowered (set ptr to that)
                  if(v[i]==-1)
                    lowvecs[i].y()+=poly[pidx]->lower_coll; // lower
                }
            }
        }
    }

  res=new Surface(poly[pidx]->surface, poly[pidx]->ncp_type,
		          pverts[0], pverts[1], pverts[2], pverts[3]);
  MEM(res);
  return res;
}

void Mesh::load(FILE *f, bool intermediate, bool is_Instance)
{
  int i;

  poly=new Polygon*[data.num_polys]; MEM(poly);
  verts=new Vertex*[data.num_vecs]; MEM(verts);

  max_polys=data.num_polys;  // ces 2000-04-13
  max_verts=data.num_vecs;   // ces 2000-04-13

  for(i=0;i<data.num_polys;i++)
    {
      poly[i] = new Polygon(this, f, intermediate, is_Instance);
      MEM(poly[i]);
    };

  for(i=0;i<data.num_vecs;i++)
    {
      verts[i] = new Vertex(f);
      MEM(verts[i]);
    };
}


void Mesh::translate(rvfloat dx, rvfloat dy, rvfloat dz)
{
  int i;

  for(i=0;i<data.num_vecs;i++)
    verts[i]->translate(dx,dy,dz);
}

void Mesh::rotate(Matrix_3x3 &mx)
{
  int i;

  for(i=0;i<data.num_vecs;i++)
    verts[i]->rotate(mx);
}

void Mesh::calc_bounds(void)
{
  int i;

  data.bbox.be_empty();

  for(i=0;i<data.num_vecs;i++)
    data.bbox.fit_into(verts[i]->pos);

  data.ball_center[0] = ( data.bbox.xlo+data.bbox.xhi ) / 2.0;
  data.ball_center[1] = ( data.bbox.ylo+data.bbox.yhi ) / 2.0;
  data.ball_center[2] = ( data.bbox.zlo+data.bbox.zhi ) / 2.0;

  data.ball_radius = 
    sqrt(
	 SQR( data.bbox.xhi-data.bbox.xlo)
	 +SQR(data.bbox.yhi-data.bbox.ylo)
	 +SQR(data.bbox.zhi-data.bbox.zlo)
	 )/2.0 + 1.0;

  data.bbox.add_safety();
}

void Mesh::calc_visible_bounds(AxisBox &abox)  // vag 2001-06-28
{
  int p,v;

  for(p=0;p<data.num_polys;p++)
    if(!poly[p]->isInvisible())
      for(v=0;v<poly[p]->numverts();v++)
        abox.fit_into(verts[poly[p]->data.vertidx[v]]->pos);
}

void Mesh::write_w(FILE *f, bool is_im_file)
{
  int i;

  calc_bounds();
  fwrite(&data,RV_W_MESH_SIZE,1,f);

  for(i=0;i<data.num_polys;i++)
    if(poly[i]==NULL)
      xpanic("Empty polys in Mesh writing stage");
    else
      poly[i]->write(f, is_im_file, false);

  for(i=0;i<data.num_vecs;i++)
    verts[i]->write(f);
}


void Mesh::cleanupPolyList(void)
{
  int cur=0;
  int i;

  while(cur < data.num_polys)
    {
      if(poly[cur] != NULL)
	cur++;
      else
	{
	  for(i=cur;i<data.num_polys-1;i++)
	    poly[i]=poly[i+1];
	  data.num_polys--;
	};
    };
}


bool Mesh::isDegenerated(void)
{
  return data.num_polys==0;
}

bool Mesh::apply(PolygonModifier *pmod)
{
  int i;

  for(i=0;i<data.num_polys;i++)
    if(poly[i] != NULL)
      if( !pmod->apply(poly[i]) )
	return false;
  return true;
}
