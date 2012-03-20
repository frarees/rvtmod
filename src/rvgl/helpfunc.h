/*
 *  Part of rvglue source code,
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

/*
 * $Id: helpfunc.h,v 1.1 2000/05/03 08:49:53 ali Exp ali $
 *
 * $Log: helpfunc.h,v $
 * Revision 1.1  2000/05/03 08:49:53  ali
 * Initial revision
 *
 *
 */
#ifndef __HELPFUNC_H
#define __HELPFUNC_H

char *breakfname(char *fname);

bool filehastype(const char *fname, const char *suff);

/*! Replace extension in place */
void replaceExt(char *fname, const char *suff); // vag 2001-05-08

/*! Replace fname+extension in place */
void replaceFname(char *fname, const char *newname); // vag 2001-05-08

/*! Same as strtok, but quoted tokens can contain delimiters. */
char *strqtok(char *str, const char *delimiters, const char *quotes); // vag 2001-05-08

#endif
