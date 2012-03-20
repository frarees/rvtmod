/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
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
 * $Id: findfile.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: findfile.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __FIND_FILE_H
#define __FIND_FILE_H

//! Searches for files
/*!
 * 
 * \param base basename of the file
 * \param allow_longer whether files with longer names are also matched
 * \param match the first match will be stored here, by malloc()ing \c *match
 * \param suff Variable length argument list, terminated by \c NULL, containing all suffices to look for
 *
 * \return
 * number of matches found
 *
 * example:
 * \code
 * char *myresult;
 * if(  find_file("nhood1", true, &myresult, "i-w", "w", NULL)   > 0 )
 * {
 *   printf("found %s\n", myresult);
 *   free(myresult);
 * };
 * \endcode
 *
 * Function requires that either \c _GLUE_USE_POSIX_DIRS or \c _GLUE_USE_WIN_DIRS is defined.
 */
int find_file(const char *base, bool allow_longer, char **match, const char *suff, ...);

#endif
