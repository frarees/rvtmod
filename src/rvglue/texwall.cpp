/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (ali) (alilein@gmx.de)
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
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

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"
#include "rvgl/mesh.h"


#include "errdesc.h"
#include "texwall.h"



Mod_TexWall::Mod_TexWall(bool _double_sized)
{
  double_sized=_double_sized;
  if(double_sized)
    base_size=2000.0;
  else
    base_size=1000.0;
  parsed = false;
}


void Mod_TexWall::dump(const char *pref)
{
  printf("%stex_wall: (%c)",pref, double_sized?'d':'s');
  if(parsed)
    texdef.printtex(stdout);
  else
    printf("unparsed");

  if(bad())
    printf(", BAD");
  printf("\n");
}

bool Mod_TexWall::bad(void)
{
  return !parsed;
}


bool Mod_TexWall::parse(Parser &parser)
{
	// If there is an opening bracket, eat it up. The bracket is not required,
	// thus the "false". If there is one, we also expect a closing one, so we
	// remember that in "haveopen"
	bool haveopen=parser.eatOpen(false);

	// Now we expect a string in the current situation
	if (parser.curtok != PT_TEXT) {
		// No string there. Error and out. The $$ is replaced by the current
		// token. The parser will write "Error:" and the line number in front
		// of it.
		parser.error(ERROR_EXPECTED_KEY, "Expected texture definition for texturewall, not $$");
		return false;

	} else {
		// String there. Cool. Now parser.tok_text contains the string.
		// We feed it into the TextureDefinition.
		if (!texdef.parseTexSpec(
			parser.tok_text, 
			&default_src_texdef,	// static object used as default values
									// if "*" is used. See docs on this.
			parser,
			false))					// we don't allow "*" for the texture sheet
		{
			// something is wrong. Error is already printed. Outta here!
			return false;
		}

		texdef.x0 += (0.5/256.0);
		texdef.y0 += (0.5/256.0);
		texdef.x1 -= (0.5/256.0);
		texdef.y1 -= (0.5/256.0);


		// now, the current token is through. Let's eat it up:
		parser.gentoken();
    }

	// If there was an opening bracket, we now expect the closing one.
	// As it is required in the current position, we call eatClose with
	// force=true. If there is no, the parser will print an error message
	// and return false.
	if (haveopen && !parser.eatClose(true)) {
		return false;
	}

	// Everything is fine
	parsed = true;

	return true;
}


bool Mod_TexWall::apply(Polygon *p)
{
	if ((p->detTEPart() & TEP_PWALL) == 0) {
		return true;
	}

	Mesh *mesh = p->parent;

	// Find the "bottom left" corner of the mesh

	rvfloat meshMinX = mesh->verts[0]->pos[0];       // $ali 05/05/00 +ff
	rvfloat meshMinZ = mesh->verts[0]->pos[2];

	for (int i = 1; i < mesh->data.num_vecs; i++) {
		Vertex *v = mesh->verts[i];

		if (meshMinX > v->pos[0] - 0.5) {
			meshMinX = v->pos[0];
		}

		if (meshMinZ > v->pos[2] - 0.5) {
			meshMinZ = v->pos[2];
		}
	}

	// Find the X, Y and Z extents of this polygon

	rvfloat minX = 1.0e9f, maxX = -1.0e9f;
	rvfloat minY = 1.0e9f, maxY = -1.0e9f;
	rvfloat minZ = 1.0e9f, maxZ = -1.0e9f;

	for (int j = 0; j < p->numverts(); j++) {
		Vertex *v = mesh->verts[p->vertidx(j)]; // $ali 05/05/00 +ff

		if (minX > v->pos[0]) {
			minX = v->pos[0];
		}

		if (maxX < v->pos[0]) {
			maxX = v->pos[0];
		}

		if (minY > v->pos[1]) {
			minY = v->pos[1];
		}

		if (maxY < v->pos[1]) {
			maxY = v->pos[1];
		}

		if (minZ > v->pos[2]) {
			minZ = v->pos[2];
		}

		if (maxZ < v->pos[2]) {
			maxZ = v->pos[2];
		}
	}

	rvfloat xSize  = maxX - minX;
	rvfloat ySize  = maxY - minY;
	rvfloat zSize  = maxZ - minZ;

	rvfloat xzSize = xSize > zSize ? xSize : zSize;

	// Oops, this polygon is higher than 1000 units, we have to split it
	// (by creating a new polygon for the part above 1000)

	if (maxY > -0.5 && ySize > (base_size+ 0.5)) {
		RV_Poly polyData = p->data;

		for (int j=0; j<p->numverts(); j++) {
			Vertex *v = mesh->verts[p->vertidx(j)];  // $ali 05/05/00 +ff

			if (v->pos[1] < (-base_size - 0.5)) {
				Vertex c = *v;
				c.pos[1] = -base_size;

				p->data.vertidx[j] = mesh->data.num_vecs;
				mesh->add(new Vertex(&c));
			} else if (v->pos[1] > (-base_size + 0.5)) {
				Vertex c = *v;
				c.pos[1] = -base_size;

				polyData.vertidx[j] = mesh->data.num_vecs;
				mesh->add(new Vertex(&c));
			}
		}

		mesh->add(new Polygon(mesh, &polyData));

		ySize =  base_size;
		minY  = -base_size;
	}

	// Now put the texture on the polygon

	p->data.texture = texdef.tex;

	for (int j=0; j<p->numverts(); j++) {
		Vertex *v = mesh->verts[p->vertidx(j)];  // $ali 05/05/00

		rvfloat x = fabs(xSize) < 0.5 ? v->pos[2] - meshMinZ : v->pos[0] - meshMinX;
		rvfloat y = v->pos[1];

		p->data.texcoord[j].u = texdef.x0 + (texdef.x1 - texdef.x0) * x / base_size;
		p->data.texcoord[j].v = texdef.y1 - (texdef.y1 - texdef.y0) * fmod(-y, base_size)
		  / base_size;

		if (fabs(p->data.texcoord[j].v - texdef.y1) < 0.01 && fabs(y - minY) < 0.5) {
			p->data.texcoord[j].v = texdef.y0;
		}

		p->data.color[j].value = 0xFFFFFF;
	}

	return true;
}

