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
#include <math.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"

#include "errdesc.h"
#include "fixbridge.h"


void MeshMod_FixBridge::dump(const char *pref)
{
	printf("%sfix_bridge\n",pref);
}


bool MeshMod_FixBridge::apply(Mesh *mesh)
{
	rvfloat ceilY;
	bool bridge = false, bridgeUpper = false;

	for (int i = 0; i < mesh->data.num_polys; i++) {
		Polygon *p = mesh->poly[i];

		if (p->detTEPart() & TEP_BRAIL) {
			bridge = true;
		}

		if (p->detTEPart() & TEP_BCEIL) {
			bridgeUpper = true;

			ceilY = mesh->verts[p->vertidx(0)]->pos.y(); // $ali 05/05/00
		}
	}

	if (!bridge && !bridgeUpper) {
		return true;
	}

	for (int i = 0; i < mesh->data.num_polys; i++) {
		Polygon *p = mesh->poly[i];

		if (p->detTEPart() & TEP_BRAIL) {
			rvfloat minY = 1.0e9f, maxY = -1.0e9f;

			for (int j = 0; j < p->numverts(); j++) {
  			        Vertex *v = mesh->verts[p->vertidx(j)]; // $ali 05/05/00

				if (minY > v->pos.coord[1]) {
					minY = v->pos.coord[1];
				}

				if (maxY < v->pos.coord[1]) {
					maxY = v->pos.coord[1];
				}
			}

			rvfloat ySize = maxY - minY;

			// Remaps the bridge "ceiling" so that it doesn't use the
			// bridge rail texture

			if (bridgeUpper && fabs(ySize) < 0.5 && fabs(minY - ceilY) < 0.5) {
				p->data.texcoord[0].u = 0.000f;
				p->data.texcoord[0].v = 0.000f;
				p->data.texcoord[1].u = 0.000f;
				p->data.texcoord[1].v = 0.121f;
				p->data.texcoord[2].u = 0.246f;
				p->data.texcoord[2].v = 0.121f;
				p->data.texcoord[3].u = 0.246f;
				p->data.texcoord[3].v = 0.000f;
			}

			// Remaps the lower rails so that they don't use the
			// bridge rail texture

			if (bridge && !bridgeUpper) {
				p->data.texcoord[0].u = 0.000f;
				p->data.texcoord[0].v = 0.250f;
				p->data.texcoord[1].u = 0.000f;
				p->data.texcoord[1].v = 0.371f;
				p->data.texcoord[2].u = 0.246f;
				p->data.texcoord[2].v = 0.371f;
				p->data.texcoord[3].u = 0.246f;
				p->data.texcoord[3].v = 0.250f;
			}
		}
	}

	return true;
}
