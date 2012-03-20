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
 * $Id: print.h,v 1.2 2000/05/03 09:15:20 ali Exp ali $
 *
 * $Log: print.h,v $
 * Revision 1.2  2000/05/03 09:15:20  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 */
#ifndef __PRINT_H
#define __PRINT_H

#define APPVERSION "beta7"

#include <stdarg.h>

/*! verbosity level:
 * \arg 0 quiet
 * \arg 1 normal
 * \arg 2 verbose
 * \arg 3 debug
 */
extern int msglevel;

/*! string of symbols for the \c -XD argument.
 * \sa dprintf
 */
extern char debugsyms[30];


/*! Message printing
 *
 * \a args is printed on screen if \a level is lower or equal to \c msglevel
 * 
 * no LF is added
 */
void lprintf(int level, const char *args, ...);


/*! Message printing
 *
 * \a args is printed on screen if \a level is lower or equal to \c msglevel
 * 
 * no LF is added. output buffer will be flushed
 */
void lprintff(int level, const char *args, ...);









/*! prints out debugging info.
 * if any character in \a sym matches any in debugsyms (resp. \c -XD ), the string will
 * be printed to screen. No LF is added.
 *
 * used syms are:
 *  \arg \c '+'   any
 *  \arg \c 'p'   parser
 *  \arg \c 'o'   option parsing
 *  \arg \c 'f'   undone fixes/features
 *  \arg \c 'k'   option keeper
 *  \arg \c 'm'   memory
 *  \arg \c 'd'   file searching / directories
 */
void dprintf(const char *sym, const char *args, ...);





/*! panic exit
 *
 * Prints out the given \a args with preceeding "PANIC:" and a brief
 * description about what to do. Then exits.
 *
 * function never returns.
 */
void xpanic(const char *args, ...);


/*! small panic exit
 *
 * Prints out the given \a args with preceeding "PANIC:", but nothing
 * more.
 *
 * function never returns.
 */
void sxpanic(const char *args, ...);


/*! info about interesting things
 *
 * Prints out the given \a args with preceeding "Interesting:", but nothing
 * more.
 */
void interest(const char *args, ...);


void showPercent(int level, int now, int total, int &last);
void doneShowPercent(int level);



/*! assert macro
 *
 * if \a ptr is \c NULL, a "not enough memory" string is printed on screen,
 * and a brief description of the pointer is written if \c -XDm is present
 */
#ifndef linux
#define MEM(ptr) __pointer_test(ptr, "?" ,__FILE__,__LINE__)
#else
#define MEM(ptr) __pointer_test(ptr, __STRING(ptr) ,__FILE__,__LINE__)
#endif



/*! Use MEM() instead. \internal */
void __pointer_test(void *ptr, const char *ptrname, const char *sfile, int sline);

#endif
