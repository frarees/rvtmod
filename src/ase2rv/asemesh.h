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
#ifndef __ASE_MESH_H
#define __ASE_MESH_H

#include "rvgl/rvtypes.h"

#include "appopts.h"
#include "aseparser.h"
#include "asevlist.h"
#include "asefacevlist.h"


void dump_sg_tables(void);


struct ASEFace
{
  int vert_idx_3ds[3];
  rvulong sm_bits;
  int mat_id;
};

struct ASEFaceNormalSection
{
  int face_id;
  int vert_id[3];
  Vector vert_normal[3];

  ASEFaceNormalSection *next;
};


class ASEMaterialList;
class Mesh;
struct Box;

class ASEMesh
{
public:
  ASEMesh();
  ~ASEMesh();

  void parse(ASEParser &p);

  void checkSanity(void);

  Mesh *createMesh(ASEMaterialList *mats, WorkMode wmode);
  Box *createBox();                             // vag 2001-05-08

  const char *name() {return nname;}            // vag 2001-05-08
  void setName(const char *n);                  // vag 2001-05-08
  void setMasterMatId(int id) {master_mat=id;}  // vag 2001-05-08

private:
  void deleteVectorList(ASEVectorList &);
  void setNumFaces3ds(int);
  void readFaceList(ASEParser &);
  void readFaceVertices(ASEParser &, ASEFace *);
  void readNormalList(ASEParser &);
  void createPoly(int, Mesh *, int &);

  void createPolyGeom(int, Mesh *, int &, ASEMaterialList *mats, WorkMode wmode);
  void applyPolyVColors(Mesh *);
  void applyPolyVTextures(Mesh *);
  void applyFaceNormal(Mesh *, ASEFaceNormalSection *);

  int num_faces_3ds;

  ASEFace **faces_3ds;
  ASEFaceNormalSection *first_normal, *last_normal;

  ASEVectorList *verts_3ds;
  ASEVectorList *tvert_3ds;
  ASEVectorList *cvert_3ds;

  ASEFaceVList *tface_3ds;
  ASEFaceVList *cface_3ds;

  char *nname;      // vag 2001-05-08
  int master_mat;   // vag 2001-05-08
};



void dump_tazfile_syntax();    // vag 2001-05-08
void dump_visfile_syntax();    // vag 2001-05-08



class BoxList;
class World;

class ASEMeshList   // vag 2001-05-08
{
public:
  ASEMeshList();
  ~ASEMeshList();

  void add(ASEMesh *mesh);  // ownership taken
  ASEMesh *find(const char* name);
  ASEMesh *at(int index);
  void reset();
  int count() {return cnt;}
  World* createWorld(ASEMaterialList *mats, WorkMode wmode); // ownersip passed
  BoxList *createBoxes(const char *listFile, bool vis);   // ownersip passed

private:
  void grow();

private:
  int cnt;
  int max;
  ASEMesh **meshes;
};

#endif
