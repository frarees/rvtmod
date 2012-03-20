/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
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

/*
 * $Id: errdesc.h,v 1.2 2000/05/03 09:39:49 ali Exp ali $
 *
 * $Log: errdesc.h,v $
 * Revision 1.2  2000/05/03 09:39:49  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:48:41  ali
 * Initial revision
 */
#ifndef __ERRDESC_H
#define __ERRDESC_H

#include <stdarg.h>
#include "rvgl/print.h"


/*! checks whether a given error number is a warning or not */
#define ERROR_IS_WARNING(err)   (err>99)




/*! Error: Syntax at top level violated */
#define ERROR_ITOP_SYNTAX       1
/*! Error: Top-level filename is neither \c .w nor \c .fin */
#define ERROR_ITOP_FILETYPE     2
/*! Error: Can't open file for reading*/
#define ERROR_FILE_ROPEN        3
/*! Error: Encountered \c EOF while reading a block */
#define ERROR_EOF_IN_BLOCK      4
/*! Error: Filename expected but something else found */
#define ERROR_EXPECTED_FNAME    5
/*! Error: Limit MAXFOPTS exceeded, following statements unparseable */
#define ERROR_MAXFOPTS_EXCEEDED 6
/*! Error: Multiple command file arguments */
#define ERROR_DUPLICATE_IFILE   7
/*! Error: No command file specified */
#define ERROR_NO_IFILE          8
/*! Error: Multiple \c create sections */
#define ERROR_DUPLICATE_OFILE   9
/*! Error: Empty \c create section */
#define ERROR_EMPTY_OFILE       10
/*! Error: End-of-block expected, found trailing garbage */
#define ERROR_BEND_EXPECTED     11
/*! Error: Option isn't known */
#define ERROR_UNKNOWN_OPTION    12
/*! Error: Keyword expected */
#define ERROR_EXPECTED_KEY      13
/*! (defunct error) */
#define ERROR_EXPECTED_TED_KEY  14
/*! Error: Expected opening bracket */
#define ERROR_BBEGIN_EXPECTED   15
/*! Error: Number is out of range */
#define ERROR_NUMBER_RANGE      16
/*! Error: Output file has no supported suffix */
#define ERROR_OFILE_SUFFIX      17
/*! Error: Number expected */
#define ERROR_EXPECTED_NUMBER   18
/*! Error: Filename is ambiguous */
#define ERROR_AMBIGUOUS_NAME    19
/*! Error: duplicate W modifier */
#define ERROR_MULTIPLE_W_MOD    20  // ces 2000-04-13
/*! Error: texdef malformatted */
#define ERROR_TEXDEF_SYNTAX     21
/*! Error: texdef is small */
#define ERROR_TEXDEF_SMALL      22
/*! Error: texdef: sheet wildcard forbidden */
#define ERROR_TEXDEF_WILDCARD   23
/*! Error: .w both to be read and written */
#define ERROR_W_IN_AND_OUT      24


/*! Warning: Comment never closed */
#define ERROR_COMMENT_EOF       101
/*! Warning: Comment-closing bracket outside a comment */
#define ERROR_COMMENT_NOBEGIN   102
/*! Warning: File option list is empty, thus useless */
#define ERROR_EMPTY_FILEOPT     103
/*! Warning: File option list defined multiple times */
#define ERROR_DUPLICATE_FILEOPT 104
/*! Warning: Multiple \c global sections */
#define ERROR_DUPLICATE_GLOBAL  105
/*! Warning: Block started where no preceeding keyword required one */
#define ERROR_USELESS_BLOCK     106
/*! Warning: .w file contains non-empty stuff between funnyballs and envmap list, can't
 * parse that */
#define ERROR_W_ENV_NOT_FOUND   107
/*! Warning: .i-w or .i-p has non-zero within the first 16 bytes */
#define ERROR_IM_HEADER         108
/*! Warning: Texdef part is too long */
#define ERROR_TEXDEF_TRAIL      109
/*! Warning: .fin matches target track */
#define ERROR_FIN_MATCHES_W     110

/*! if not disabled by the user this functions prints out a brief description
 * on the error \a errnum and what it means */
void describe_error(int errnum);


/*! prints out an error message.
 * \param errnum error code: one of the \c ERROR_ defines
 * \param level message level for this error.
 * \param args text to print. Message is preceeded by either "Error:" or "Warning:"
 *
 * calls describe_error()
 */
void perror(int level, int errnum, const char *args, ...);


#endif
