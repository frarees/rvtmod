/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
 *  Copyright (C) 2000 Alexander Kroeller (ali) (alilein@gmx.de)
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
#include "fixpipe.h"

#ifndef M_PI
#define M_PI 3.14159265358979
#endif


void MeshMod_FixPipe::dump(const char *pref)
{
	printf("%sfix_pipe\n",pref);
}


bool MeshMod_FixPipe::apply(Mesh *mesh)
{
	int i, j;
	rvfloat minHeights[8] = { -1e9,-1e9,-1e9,-1e9,-1e9,-1e9,-1e9,-1e9 };
	Polygon *newPolys [8] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
	Vector polyCenters[8];

	bool pipe = false;

	for (i = 0; i < mesh->data.num_polys; i++) {
		if (mesh->poly[i]->detTEPart() & TEP_PIPE) {
			pipe = true;
			break;
		}
	}

	if (!pipe) {
		return true;
	}

	rvfloat meshMinX = mesh->verts[0]->pos.coord[0];
	rvfloat meshMaxX = mesh->verts[0]->pos.coord[0];
	rvfloat meshMinZ = mesh->verts[0]->pos.coord[2];
	rvfloat meshMaxZ = mesh->verts[0]->pos.coord[2];

	for (i = 1; i < mesh->data.num_vecs; i++) {
		Vertex *v = mesh->verts[i];

		if (meshMinX > v->pos.coord[0] - 0.5) {
			meshMinX = v->pos.coord[0];
		}

		if (meshMaxX < v->pos.coord[0] + 0.5) {
			meshMaxX = v->pos.coord[0];
		}

		if (meshMinZ > v->pos.coord[2] - 0.5) {
			meshMinZ = v->pos.coord[2];
		}

		if (meshMaxZ < v->pos.coord[2] + 0.5) {
			meshMaxZ = v->pos.coord[2];
		}
	}

	Vector meshCenter(
		floor((meshMinX + meshMaxX) / 2000 + 0.5) * 1000,
		0.0f,
		floor((meshMinZ + meshMaxZ) / 2000 + 0.5) * 1000);

	for (i = 0; i < mesh->data.num_polys; i++) {
		Polygon *p = mesh->poly[i];

		if (p->detTEPart() & (TEP_PWALL | TEP_PIPE)) {
			rvfloat minHeight = -1e9;
			Vector polyCenter(0.0, 0.0, 0.0);

			for (j = 0; j < p->numverts(); j++) {
				Vertex *v = mesh->verts[p->vertidx(j)];

				polyCenter = polyCenter + v->pos;

				if (minHeight < v->pos.coord[1]) {
					minHeight = v->pos.coord[1];
				}
			}

			Vector polyCoord = polyCenter * 0.25;

			polyCenter = polyCoord - meshCenter;

			int octant = (int) ((atan2(polyCenter.z(), polyCenter.x()) + M_PI) / (M_PI / 4));

			if (p->detTEPart() & TEP_PIPE) {
				if (minHeights[octant] < minHeight) {
					minHeights[octant] = minHeight;
				}
			}

			if ((p->detTEPart() & TEP_PWALL) && p->numverts() == 4) {
				Vector& v0 = mesh->verts[p->vertidx(0)]->pos;
				Vector& v1 = mesh->verts[p->vertidx(1)]->pos;
				Vector& v2 = mesh->verts[p->vertidx(2)]->pos;

				rvfloat w = (v0 - v1).length();
				rvfloat h = (v2 - v1).length();

				if (fabs(w - 125.0) < 0.5 && fabs(h - 125.0) < 0.5) {
					// Check if we've encountered an indentical polygon before,
					// and if so, delete the current one so we don't end up with
					// duplicates

					bool found = false;

					for (j = 0; j < polyList.size(); j++) {
						if ((polyList[j].center - polyCoord).length() < 1.0) {
							found = true;
							break;
						}
					}

					if (found) {
						delete mesh->poly[i];
						mesh->poly[i] = NULL;
					} else {
						polyList.push_back(PolyInfo(p, polyCoord));
					}
				}
			}
		}
	}

	for (i = 0; i < mesh->data.num_polys; i++) {
		Polygon *p = mesh->poly[i];

		if (p->isInTexture(0,  0.5,1.0,  0.0,0.375)) {
			rvfloat minX = 1.0e9f, maxX = -1.0e9f;
			rvfloat minZ = 1.0e9f, maxZ = -1.0e9f;
			Vector polyCenter(0.0, 0.0, 0.0);

			for (j = 0; j < p->numverts(); j++) {
				Vertex *v = mesh->verts[p->vertidx(j)];

				polyCenter = polyCenter + v->pos;

				if (minX > v->pos.coord[0]) {
					minX = v->pos.coord[0];
				}

				if (maxX < v->pos.coord[0]) {
					maxX = v->pos.coord[0];
				}

				if (minZ > v->pos.coord[2]) {
					minZ = v->pos.coord[2];
				}

				if (maxZ < v->pos.coord[2]) {
					maxZ = v->pos.coord[2];
				}
			}

			Vector polyCoord = polyCenter / (rvfloat) p->numverts();

			polyCenter = polyCoord - meshCenter;

			int octant = (int) ((atan2(polyCenter.z(), polyCenter.x()) + M_PI) / (M_PI / 4));

			if (newPolys[octant] == NULL
			&&	(fabs(maxX - minX) < 0.5 || fabs(maxZ - minZ) < 0.5)
			&&	fabs(polyCenter.x()) > 10.0 && fabs(polyCenter.z()) > 10.0) {
				Vector p[4];
				Vector norm;

				// Find the corners of the new polygon

				switch (octant) {

				case 0:
					p[3] = Vector(-500, minHeights[octant] + 125, -500);
					p[2] = Vector(-500, minHeights[octant]      , -500);
					p[1] = Vector(-500, minHeights[octant]      , -375);
					p[0] = Vector(-500, minHeights[octant] + 125, -375);

					norm = Vector(-1.0, 0.0, 0.0);
					break;

				case 1:
					p[0] = Vector(-375, minHeights[octant]      , -500);
					p[1] = Vector(-375, minHeights[octant] + 125, -500);
					p[2] = Vector(-500, minHeights[octant] + 125, -500);
					p[3] = Vector(-500, minHeights[octant]      , -500);

					norm = Vector(0.0, 0.0, -1.0);
					break;
				case 2:
					p[3] = Vector(375, minHeights[octant]      , -500);
					p[2] = Vector(375, minHeights[octant] + 125, -500);
					p[1] = Vector(500, minHeights[octant] + 125, -500);
					p[0] = Vector(500, minHeights[octant]      , -500);

					norm = Vector(0.0, 0.0, -1.0);
					break;

				case 3:
					p[0] = Vector(500, minHeights[octant] + 125, -500);
					p[1] = Vector(500, minHeights[octant]      , -500);
					p[2] = Vector(500, minHeights[octant]      , -375);
					p[3] = Vector(500, minHeights[octant] + 125, -375);

					norm = Vector(-1.0, 0.0, 0.0);
					break;

				case 4:
					p[3] = Vector(500, minHeights[octant] + 125, 500);
					p[2] = Vector(500, minHeights[octant]      , 500);
					p[1] = Vector(500, minHeights[octant]      , 375);
					p[0] = Vector(500, minHeights[octant] + 125, 375);

					norm = Vector(-1.0, 0.0, 0.0);
					break;

				case 5:
					p[0] = Vector(375, minHeights[octant]      , 500);
					p[1] = Vector(375, minHeights[octant] + 125, 500);
					p[2] = Vector(500, minHeights[octant] + 125, 500);
					p[3] = Vector(500, minHeights[octant]      , 500);

					norm = Vector(0.0, 0.0, -1.0);
					break;

				case 6:
					p[3] = Vector(-375, minHeights[octant]      , 500);
					p[2] = Vector(-375, minHeights[octant] + 125, 500);
					p[1] = Vector(-500, minHeights[octant] + 125, 500);
					p[0] = Vector(-500, minHeights[octant]      , 500);

					norm = Vector(0.0, 0.0, -1.0);
					break;

				case 7:
					p[0] = Vector(-500, minHeights[octant] + 125, 500);
					p[1] = Vector(-500, minHeights[octant]      , 500);
					p[2] = Vector(-500, minHeights[octant]      , 375);
					p[3] = Vector(-500, minHeights[octant] + 125, 375);

					norm = Vector(-1.0, 0.0, 0.0);
					break;
				}

				for (j = 0; j < 4; j++) {
					// Translate to the mesh position
	
					p[j] = p[j] + meshCenter;

					// Add the vertex

					Vertex vert(p[j], norm);

					mesh->add(new Vertex(&vert));
				}

				// add a new polygon

				RV_Poly polyData = {
					1, 0,
					{
						mesh->data.num_vecs - 4,
						mesh->data.num_vecs - 3,
						mesh->data.num_vecs - 2,
						mesh->data.num_vecs - 1
					},
					{ {0xFFFFFF}, {0xFFFFFF}, {0xFFFFFF}, {0xFFFFFF} },
					{ { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 } }
				};

				polyCenter = (p[0] + p[1] + p[2] + p[3]) / 4;

				// Check if we've encountered an indentical polygon before,
				// and if so, use that instead of creating a new one

				bool found = false;

				for (j = 0; j < polyList.size(); j++) {
					if ((polyList[j].center - polyCenter).length() < 1.0) {
						found = true;
						break;
					}
				}

				if (found) {
					newPolys[octant] = polyList[j].poly;
				} else {
					newPolys[octant] = new Polygon(mesh, &polyData);

					mesh->add(newPolys[octant]);

					polyList.push_back(PolyInfo(newPolys[octant], polyCenter));
				}

				polyCenters[octant] = polyCenter;
			}
		}
	}

	// Finally, put textures on the "missing pieces"

	for (i = 0; i < 8; i++) {
		if (newPolys[i]) {
			Polygon *p = newPolys[i];

			p->data.texture = 0;

			for (int j = 0; j < p->numverts(); j++) {
				Vertex *v = mesh->verts[p->vertidx(j)];

				Vector relPos = v->pos - Vector(meshMinX, 0, meshMinZ);
				rvfloat x = relPos.lengthXZ();
				rvfloat y = relPos.y();

				if ((relPos.x() < 0.5 || relPos.x() > 999.5)
				&&	(relPos.z() < 0.5 || relPos.z() > 999.5)) {
					p->data.texcoord[j].u = 0.504f;
				} else {
					p->data.texcoord[j].u = 0.625f;
				}

				if (relPos.y() > polyCenters[i].y() + 0.5) {
					p->data.texcoord[j].v = 0.371f;
				} else {
					p->data.texcoord[j].v = 0.250f;
				}

				p->data.color[j].value = 0xFFFFFF;
			}
		}
	}

	return true;
}
