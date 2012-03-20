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

#include "errdesc.h"
#include "texfloor.h"


MeshMod_TexFloor::MeshMod_TexFloor()
{
	parsed = false;
}


void MeshMod_TexFloor::dump(const char *pref)
{
	printf("%stex_floor: ",pref);

	if (parsed) {
		texdef.printtex(stdout);
	} else {
		printf("unparsed");
	}

	if (bad()) {
		printf(", BAD");
	}

	printf("\n");
}


bool MeshMod_TexFloor::bad(void)
{
	return !parsed;
}


bool MeshMod_TexFloor::parse(Parser &parser)
{
	bool haveopen;

	/* If there is an opening bracket, eat it up. The bracket is not required,
	thus the "false". If there is one, we also expect a closing one, so we remember
	that in "haveopen" */
	haveopen=parser.eatOpen(false);

	/* Now we expect a string in the current situation */
	if( parser.curtok != PT_TEXT )
	{
		/* No string there. Error and out. The $$ is replaced by the current token. The
		parser will write "Error:" and the line number in front of it. */
		parser.error(ERROR_EXPECTED_KEY, "Expected texture definition for texturefloor, not $$");
		return false;
	}
	else
	{
		/* String there. Cool. Now parser.tok_text contains the string. We feed it into
		the TextureDefinition. */

		if (!texdef.parseTexSpec( parser.tok_text, 
			&default_src_texdef, /* static object used as default values
								 * if "*" is used. See docs on this.
								 */
			parser,
			false)) /* we don't allow "*" for the texture sheet */
		{
			/* something is wrong. Error is already printed. Outta here! */
			return false;
		}

		texdef.x0 += (0.5/256.0);
		texdef.y0 += (0.5/256.0);
		texdef.x1 -= (0.5/256.0);
		texdef.y1 -= (0.5/256.0);

		/* now, the current token is through. Let's eat it up: */
		parser.gentoken();
	}

	/* If there was an opening bracket, we now expect the closing one. As it is required
	in the current position, we call eatClose with force=true. If there is no, the
	parser will print an error message and return false. */
	if (haveopen && !parser.eatClose(true)) {
		return false;
	}

	// Everything is fine
	parsed = true;

	return true;
}


bool MeshMod_TexFloor::apply(Mesh *mesh)
{
	bool floor = true;

	if (mesh->data.num_polys == 0 || mesh->data.num_vecs == 0) {
		return true;
	}

	for (int i = 0; i < mesh->data.num_polys; i++) {
		Polygon *p = mesh->poly[i];

		if (p->detTEPart() != TEP_FLOOR) {
			floor = false;
		}

		if (p->detTEPart() == TEP_FLOOR2) {
			p->data.texture = texdef.tex;

			p->data.texcoord[0].u = texdef.x0;
			p->data.texcoord[0].v = texdef.y0;
			p->data.texcoord[1].u = texdef.x1;
			p->data.texcoord[1].v = texdef.y0;
			p->data.texcoord[2].u = texdef.x1;
			p->data.texcoord[2].v = texdef.y1;
			p->data.texcoord[3].u = texdef.x0;
			p->data.texcoord[3].v = texdef.y1;
		}
	}

	if (!floor) {
		return true;
	}

	Vertex minXminZ = mesh->verts[0];  // ali 05/05/00 +ff
	Vertex maxXminZ = mesh->verts[0];
	Vertex minXmaxZ = mesh->verts[0];
	Vertex maxXmaxZ = mesh->verts[0];

	for (int i = 1; i < mesh->data.num_vecs; i++) {
		Vertex *v = mesh->verts[i];

		if (minXminZ.pos[0] > v->pos[0]
		||	minXminZ.pos[2] > v->pos[2]) {
			minXminZ = *v;
		}

		if (maxXminZ.pos[0] < v->pos[0]
		||	maxXminZ.pos[2] > v->pos[2]) {
			maxXminZ = *v;
		}

		if (minXmaxZ.pos[0] > v->pos[0]
		||	minXmaxZ.pos[2] < v->pos[2]) {
			minXmaxZ = *v;
		}

		if (maxXmaxZ.pos[0] < v->pos[0]
		||	maxXmaxZ.pos[2] < v->pos[2]) {
			maxXmaxZ = *v;
		}
	}

	RV_Poly polyData = {
		1, texdef.tex,
		{ 0, 1, 2, 3 },
		{ {0xFFFFFF}, {0xFFFFFF}, {0xFFFFFF}, {0xFFFFFF} },
		{
			{ texdef.x0, texdef.y0 },
			{ texdef.x0, texdef.y1 },
			{ texdef.x1, texdef.y1 },
			{ texdef.x1, texdef.y0 }
		}
	};

	// Delete all old polygons and vertices...

	for (int i = 0; i < mesh->data.num_polys; i++) {
		delete mesh->poly[i];
	}

	mesh->data.num_polys = 0;

	for (int i = 0; i < mesh->data.num_vecs; i++) {
		delete mesh->verts[i];
	}

	mesh->data.num_vecs = 0;

	// ...and add our new stuff

	mesh->add(new Vertex(&minXminZ));
	mesh->add(new Vertex(&minXmaxZ));
	mesh->add(new Vertex(&maxXmaxZ));
	mesh->add(new Vertex(&maxXminZ));

	mesh->add(new Polygon(mesh, &polyData));

	return true;
}
