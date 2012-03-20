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
 * $Id: keys.h,v 1.2 2000/05/03 09:46:42 ali Exp ali $
 *
 * $Log: keys.h,v $
 * Revision 1.2  2000/05/03 09:46:42  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 */
#ifndef __KEYS_H
#define __KEYS_H


///////////// Section Names //////////////

#define KEY_CREATE_SECTION       "create"
#define KEY_GLOBAL_SECTION       "global"
#define KEY_DEFINE_SECTION       "define"

#define KEY_DEFAULT              "default"

///////// Modifier Names ///////////////

#define KEY_MOD_INVISIBLE        "invisible"
#define KEY_MOD_NOCOLL           "nocoll"
#define KEY_MOD_OFF              "off"
#define KEY_MOD_ENVMAP           "envmap"
#define KEY_MOD_TEXMAP           "texmap"
#define KEY_SURFACE_LONG         "surface"
#define KEY_SURFACE_SHORT        "s"
#define KEY_TRANS_LONG           "transparency"
#define KEY_TRANS_SHORT          "t"
#define KEY_LEGOSPLIT            "legosplit"
#define KEY_TEX_WALL_DOUBLE      "wall_double"
#define KEY_TEX_WALL_SINGLE      "wall_single"
#define KEY_TEX_FLOOR_LONG       "texturefloor"
#define KEY_TEX_FLOOR_SHORT      "floor"
#define KEY_SETCOLOR             "color"
#define KEY_SETSHADEDCOLOR       "colorize"
#define KEY_SETSHADE             "setshade"
#define KEY_SHADE                "shade"
#define KEY_TAZSPLIT             "tazsplit"
#define KEY_FIX_PIPE             "fixpipe"    // ces 2000-04-13
#define KEY_FIX_BRIDGE           "fixbridge"  // ces 2000-04-13
#define KEY_DOUBLE               "doublesided"
#define KEY_ROT                  "rot"        // vag 2001-05-08
#define KEY_LOWER_COLL           "lower_coll" // vag 2001-06-28

#endif
